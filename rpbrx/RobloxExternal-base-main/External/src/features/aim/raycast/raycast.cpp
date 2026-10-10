#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "raycast.h"
#include "../core/aim.h"
#include "../pftarget/pftarget.h"
#include "../tracking/tracking.h"
#include "../health/health.h"
#include "../distance/distance.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../utility/struct/offsets.h"
#include "../../../menu/overlay/settings.h"
#include "../hitbox/hitbox.h"
#include <TlHelp32.h>
#include <Windows.h>
#include <vector>
#include <cstring>
#include <chrono>
#include <cstddef>

#ifndef CFG_CALL_TARGET_VALID
#define CFG_CALL_TARGET_VALID 0x00000001
#endif

namespace rpbrx::aim::raycast
{
    namespace
    {
        constexpr std::uintptr_t desc_rva_z = Offsets::WorldRoot::RaycastBoundDesc;
        constexpr std::uintptr_t bound_fn_offset = Offsets::WorldRoot::RaycastBoundFn;
        constexpr std::size_t k_stub_bytes = 0x200;

#pragma pack(push, 4)
        struct RaycastState
        {
            std::uint32_t active = 0;
            std::uint32_t reserved = 0;
            float target_x = 0.f;
            float target_y = 0.f;
            float target_z = 0.f;
            float scale = 1.15f;
            std::uint64_t calls = 0;
        };
#pragma pack(pop)

        static_assert(offsetof(RaycastState, active) == 0x00, "active");
        static_assert(offsetof(RaycastState, target_x) == 0x08, "target");
        static_assert(offsetof(RaycastState, calls) == 0x18, "calls");

        struct Hook
        {
            std::uintptr_t thunk = 0;
            std::uintptr_t state = 0;
            std::uintptr_t originalFunction = 0;
            std::uintptr_t module_base = 0;
            bool thunk_owned = false;
            bool installed = false;
            bool active = false;
        };

        Hook g_hook{};
        bool g_wallbang = false;
        auto g_lastFail = std::chrono::steady_clock::time_point{};
        char g_status[80] = "idle: hook not requested";

        void set_status(const char* s)
        {
            if (!s || !s[0]) return;
            size_t n = strlen(s);
            if (n >= sizeof(g_status)) n = sizeof(g_status) - 1;
            memcpy(g_status, s, n);
            g_status[n] = '\0';
        }

        bool ok_addr(std::uintptr_t a)
        {
            return a >= 0x10000ull && a < 0x00007FFFFFFFFFFFull;
        }

        bool connected()
        {
            return g_proc && g_proc->connected();
        }

        std::uintptr_t ray_alloc(std::size_t n, DWORD prot)
        {
            if (!connected())
                return 0;
            return (std::uintptr_t)VirtualAllocEx(g_proc->raw(), nullptr, n, MEM_COMMIT | MEM_RESERVE, prot);
        }

        bool ray_free(std::uintptr_t a)
        {
            if (!a || !connected())
                return false;
            return VirtualFreeEx(g_proc->raw(), (LPVOID)a, 0, MEM_RELEASE) != 0;
        }

        std::uintptr_t ray_module(const wchar_t* name)
        {
            if (!connected() || !name)
                return 0;
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, g_proc->pid());
            if (snap == INVALID_HANDLE_VALUE)
                return 0;
            MODULEENTRY32W me{};
            me.dwSize = sizeof(me);
            std::uintptr_t out = 0;
            if (Module32FirstW(snap, &me))
            {
                do
                {
                    if (_wcsicmp(me.szModule, name) == 0)
                    {
                        out = (std::uintptr_t)me.modBaseAddr;
                        break;
                    }
                } while (Module32NextW(snap, &me));
            }
            CloseHandle(snap);
            return out;
        }

        bool w_mem(std::uintptr_t a, const void* d, std::size_t s)
        {
            if (!ok_addr(a) || !d || !s || !connected())
                return false;
            return g_proc->write_buf(a, d, s);
        }

        std::size_t pg()
        {
            static const std::size_t p = []
            {
                SYSTEM_INFO i{};
                GetSystemInfo(&i);
                return i.dwPageSize ? static_cast<std::size_t>(i.dwPageSize) : 0x1000u;
            }();
            return p;
        }

        DWORD query_protect(std::uintptr_t a)
        {
            MEMORY_BASIC_INFORMATION mbi{};
            if (!VirtualQueryEx(g_proc->raw(), reinterpret_cast<void*>(a), &mbi, sizeof(mbi)))
                return 0;
            return mbi.Protect;
        }

        bool is_executable_protect(DWORD p)
        {
            const DWORD x = p & 0xFF;
            return x == PAGE_EXECUTE || x == PAGE_EXECUTE_READ || x == PAGE_EXECUTE_READWRITE || x == PAGE_EXECUTE_WRITECOPY;
        }

        bool protect_remote(std::uintptr_t address, std::size_t size, DWORD protection, DWORD* old_protect = nullptr)
        {
            if (!ok_addr(address) || !size || !connected())
                return false;
            const std::uintptr_t page_mask = ~(static_cast<std::uintptr_t>(pg()) - 1);
            const std::uintptr_t base = address & page_mask;
            const std::uintptr_t end = (address + size + pg() - 1) & page_mask;
            const std::size_t span = static_cast<std::size_t>(end - base);
            using NtProtectFn = LONG(WINAPI*)(HANDLE, PVOID*, PSIZE_T, ULONG, PULONG);
            static NtProtectFn nt_protect = []() -> NtProtectFn
            {
                HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
                if (!ntdll)
                    return nullptr;
                return reinterpret_cast<NtProtectFn>(GetProcAddress(ntdll, "NtProtectVirtualMemory"));
            }();
            auto try_one = [&](DWORD req) -> bool
            {
                DWORD old = 0;
                if (VirtualProtectEx(g_proc->raw(), reinterpret_cast<void*>(base), span, req, &old))
                {
                    if (old_protect)
                        *old_protect = old;
                    return true;
                }
                if (!nt_protect)
                    return false;
                PVOID nt_base = reinterpret_cast<void*>(base);
                SIZE_T nt_size = span;
                ULONG nt_old = 0;
                const LONG st = nt_protect(g_proc->raw(), &nt_base, &nt_size, req, &nt_old);
                if (st >= 0)
                {
                    if (old_protect)
                        *old_protect = static_cast<DWORD>(nt_old);
                    return true;
                }
                return false;
            };
            if (try_one(protection))
                return true;
            if (protection == PAGE_EXECUTE_READWRITE && try_one(PAGE_EXECUTE_WRITECOPY))
                return true;
            if (protection == PAGE_READWRITE && try_one(PAGE_WRITECOPY))
                return true;
            return false;
        }

        bool write_protected(std::uintptr_t address, const void* data, std::size_t size)
        {
            if (!ok_addr(address) || !data || !size)
                return false;
            DWORD old = 0;
            const bool changed = protect_remote(address, size, PAGE_EXECUTE_READWRITE, &old);
            const bool wrote = w_mem(address, data, size);
            if (changed)
                protect_remote(address, size, old, nullptr);
            return wrote;
        }

        bool mark_cfg(std::uintptr_t t)
        {
            auto resolve = []() -> FARPROC
            {
                const char* mods[] = { "kernelbase.dll", "kernel32.dll", "api-ms-win-core-memory-l1-1-3.dll" };
                for (auto* m : mods)
                {
                    HMODULE h = GetModuleHandleA(m);
                    if (!h)
                        h = LoadLibraryA(m);
                    if (!h)
                        continue;
                    if (auto p = GetProcAddress(h, "SetProcessValidCallTargets"))
                        return p;
                }
                return nullptr;
            };
            auto proc = resolve();
            if (!proc)
                return false;
            struct Info
            {
                ULONG_PTR Offset;
                ULONG Flags;
            } info{};
            info.Offset = t & (pg() - 1);
            info.Flags = CFG_CALL_TARGET_VALID;
            using Fn = BOOL(WINAPI*)(HANDLE, PVOID, SIZE_T, ULONG, void*);
            SetLastError(0);
            const BOOL ok = reinterpret_cast<Fn>(proc)(g_proc->raw(),
                reinterpret_cast<void*>(t & ~(static_cast<std::uintptr_t>(pg()) - 1)), pg(), 1, &info);
            return ok != 0;
        }

        void append_u64(std::vector<std::uint8_t>& c, std::uint64_t v)
        {
            const auto* b = reinterpret_cast<const std::uint8_t*>(&v);
            c.insert(c.end(), b, b + 8);
        }

        void patch_rel32(std::vector<std::uint8_t>& c, std::size_t o, std::size_t t)
        {
            const std::int32_t v =
                static_cast<std::int32_t>(static_cast<std::ptrdiff_t>(t) - static_cast<std::ptrdiff_t>(o + 4));
            std::memcpy(c.data() + o, &v, 4);
        }

        std::vector<std::uint8_t> make_hook_thunk(std::uintptr_t state, std::uintptr_t orig)
        {
            std::vector<std::uint8_t> c;
            c.reserve(384);
            std::vector<std::size_t> inactive;
            auto je_inactive = [&]
            {
                c.insert(c.end(), { 0x0F, 0x84 });
                inactive.push_back(c.size());
                c.insert(c.end(), { 0, 0, 0, 0 });
            };
            auto jbe_inactive = [&]
            {
                c.insert(c.end(), { 0x0F, 0x86 });
                inactive.push_back(c.size());
                c.insert(c.end(), { 0, 0, 0, 0 });
            };
            c.insert(c.end(), { 0x48, 0x83, 0xEC, 0x68 });
            c.insert(c.end(), { 0x49, 0xBA });
            append_u64(c, state);
            c.insert(c.end(), { 0x41, 0x83, 0x3A, 0x00 });
            je_inactive();
            c.insert(c.end(), { 0x4D, 0x85, 0xC0 });
            je_inactive();
            c.insert(c.end(), { 0x4D, 0x85, 0xC9 });
            je_inactive();
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x42, 0x08 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x5C, 0x00 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x44, 0x24, 0x40 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x4A, 0x0C });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x5C, 0x48, 0x04 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x4C, 0x24, 0x44 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x52, 0x10 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x5C, 0x50, 0x08 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x54, 0x24, 0x48 });
            c.insert(c.end(), { 0x0F, 0x28, 0xD8 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xDB });
            c.insert(c.end(), { 0x0F, 0x28, 0xE1 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xDC });
            c.insert(c.end(), { 0x0F, 0x28, 0xE2 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xDC });
            c.insert(c.end(), { 0xF3, 0x0F, 0x51, 0xDB });
            c.insert(c.end(), { 0x0F, 0x57, 0xED });
            c.insert(c.end(), { 0x0F, 0x2E, 0xDD });
            jbe_inactive();
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x21 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x69, 0x04 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xED });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xE5 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x69, 0x08 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xED });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xE5 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x51, 0xE4 });
            c.insert(c.end(), { 0x0F, 0x57, 0xED });
            c.insert(c.end(), { 0x0F, 0x2E, 0xE5 });
            jbe_inactive();
            c.insert(c.end(), { 0x41, 0x8B, 0x42, 0x04 });
            c.insert(c.end(), { 0xA8, 0x01 });
            c.insert(c.end(), { 0x0F, 0x85 });
            const std::size_t wallbang_jmp = c.size();
            c.insert(c.end(), { 0, 0, 0, 0 });
            c.insert(c.end(), { 0x0F, 0x28, 0xEC });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5E, 0xEB });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xC5 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xCD });
            c.insert(c.end(), { 0xF3, 0x0F, 0x59, 0xD5 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x11, 0x01 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x11, 0x49, 0x04 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x11, 0x51, 0x08 });
            c.insert(c.end(), { 0x49, 0xFF, 0x42, 0x18 });
            c.push_back(0xE9);
            const std::size_t to_call = c.size();
            c.insert(c.end(), { 0, 0, 0, 0 });
            const std::size_t wallbang_off = c.size();
            patch_rel32(c, wallbang_jmp, wallbang_off);
            c.insert(c.end(), { 0xF3, 0x0F, 0x5E, 0xC3 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5E, 0xCB });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5E, 0xD3 });
            c.insert(c.end(), { 0x0F, 0x28, 0xE0 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x59, 0x62, 0x14 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x6A, 0x08 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5C, 0xEC });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x6C, 0x24, 0x50 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x64, 0x24, 0x40 });
            c.insert(c.end(), { 0x0F, 0x28, 0xE1 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x59, 0x62, 0x14 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x6A, 0x0C });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5C, 0xEC });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x6C, 0x24, 0x54 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x64, 0x24, 0x44 });
            c.insert(c.end(), { 0x0F, 0x28, 0xE2 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x59, 0x62, 0x14 });
            c.insert(c.end(), { 0xF3, 0x41, 0x0F, 0x10, 0x6A, 0x10 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x5C, 0xEC });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x6C, 0x24, 0x58 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x58, 0xE4 });
            c.insert(c.end(), { 0xF3, 0x0F, 0x11, 0x64, 0x24, 0x48 });
            c.insert(c.end(), { 0x4C, 0x8D, 0x44, 0x24, 0x50 });
            c.insert(c.end(), { 0x4C, 0x8D, 0x4C, 0x24, 0x40 });
            c.insert(c.end(), { 0x49, 0xFF, 0x42, 0x18 });
            const std::size_t call_off = c.size();
            patch_rel32(c, to_call, call_off);
            const std::size_t inactive_off = c.size();
            for (auto o : inactive)
                patch_rel32(c, o, inactive_off);
            c.insert(c.end(), { 0x48, 0x8B, 0x84, 0x24, 0x90, 0x00, 0x00, 0x00 });
            c.insert(c.end(), { 0x48, 0x89, 0x44, 0x24, 0x20 });
            c.insert(c.end(), { 0x48, 0xB8 });
            append_u64(c, orig);
            c.insert(c.end(), { 0xFF, 0xD0 });
            c.insert(c.end(), { 0x48, 0x83, 0xC4, 0x68 });
            c.push_back(0xC3);
            return c;
        }

        bool region_is_padding(std::uintptr_t a, std::size_t n)
        {
            std::vector<std::uint8_t> buf(n);
            if (!g_proc->read_buf(a, buf.data(), n))
                return false;
            for (auto b : buf)
            {
                if (b != 0xCC && b != 0x00 && b != 0x90)
                    return false;
            }
            return true;
        }

        bool read_val(std::uintptr_t a, void* d, std::size_t s)
        {
            if (!connected())
                return false;
            return g_proc->read_buf(a, d, s);
        }

        std::uintptr_t find_cave_in_module(std::uintptr_t module_base, std::size_t need, std::uintptr_t min_offset,
            std::uintptr_t ignore)
        {
            if (!ok_addr(module_base) || !need)
                return 0;
            IMAGE_DOS_HEADER dos{};
            if (!read_val(module_base, &dos, sizeof(dos)) || dos.e_magic != IMAGE_DOS_SIGNATURE)
                return 0;
            IMAGE_NT_HEADERS64 nt{};
            const std::uintptr_t nt_addr = module_base + static_cast<std::uintptr_t>(dos.e_lfanew);
            if (!read_val(nt_addr, &nt, sizeof(nt)) || nt.Signature != IMAGE_NT_SIGNATURE)
                return 0;
            const std::uintptr_t section_base =
                nt_addr + offsetof(IMAGE_NT_HEADERS64, OptionalHeader) + nt.FileHeader.SizeOfOptionalHeader;
            for (WORD i = 0; i < nt.FileHeader.NumberOfSections; ++i)
            {
                IMAGE_SECTION_HEADER section{};
                if (!read_val(section_base + static_cast<std::uintptr_t>(i) * sizeof(section), &section, sizeof(section)))
                    break;
                if ((section.Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
                    continue;
                const std::uintptr_t section_off = section.VirtualAddress;
                std::size_t section_size = section.Misc.VirtualSize ? section.Misc.VirtualSize : section.SizeOfRawData;
                if (!section_off || section_size < need)
                    continue;
                std::uintptr_t scan_off = section_off;
                if (scan_off < min_offset)
                    scan_off = min_offset;
                if (scan_off >= section_off + section_size)
                    continue;
                const std::uintptr_t scan_start = module_base + scan_off;
                const std::size_t scan_size = static_cast<std::size_t>(section_off + section_size - scan_off);
                std::vector<std::uint8_t> buf(scan_size);
                if (!read_val(scan_start, buf.data(), buf.size()))
                    continue;
                std::size_t run_start = 0;
                std::size_t run_len = 0;
                for (std::size_t j = 0; j < buf.size(); ++j)
                {
                    const std::uint8_t b = buf[j];
                    if (b != 0x00 && b != 0xCC && b != 0x90)
                    {
                        run_len = 0;
                        run_start = j + 1;
                        continue;
                    }
                    ++run_len;
                    if (run_len < need)
                        continue;
                    std::uintptr_t cand = scan_start + run_start;
                    const std::uintptr_t aligned = (cand + 0x0F) & ~static_cast<std::uintptr_t>(0x0F);
                    const std::size_t loss = static_cast<std::size_t>(aligned - cand);
                    if (run_len < need + loss)
                        continue;
                    if (ignore && aligned == ignore)
                        continue;
                    if (g_hook.thunk && aligned == g_hook.thunk)
                        continue;
                    return aligned;
                }
            }
            return 0;
        }

        std::uintptr_t find_exec_cave(std::size_t need, std::uintptr_t, std::uintptr_t ignore = 0)
        {
            static const wchar_t* k_pref[] = {
                L"winsta.dll",
                L"win32u.dll",
                L"uxtheme.dll",
                L"dwmapi.dll",
                L"msctf.dll",
                L"TextInputFramework.dll",
                L"CoreMessaging.dll",
                L"user32.dll",
            };
            for (std::size_t mi = 0; mi < sizeof(k_pref) / sizeof(k_pref[0]); ++mi)
            {
                const wchar_t* name = k_pref[mi];
                const std::uintptr_t mod = ray_module(name);
                if (!mod)
                    continue;
                const std::uintptr_t min_off = (mi == 0) ? 0x2000u : 0x1000u;
                const std::uintptr_t cave = find_cave_in_module(mod, need, min_off, ignore);
                if (!cave)
                    continue;
                return cave;
            }
            MEMORY_BASIC_INFORMATION mbi{};
            std::uintptr_t addr = 0;
            std::uintptr_t fallback_rwx = 0;
            while (VirtualQueryEx(g_proc->raw(), reinterpret_cast<void*>(addr), &mbi, sizeof(mbi)))
            {
                const auto base = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
                const auto size = static_cast<std::size_t>(mbi.RegionSize);
                addr = base + size;
                if (addr < base)
                    break;
                if (mbi.State != MEM_COMMIT)
                    continue;
                if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
                    continue;
                if (!is_executable_protect(mbi.Protect))
                    continue;
                if (size < need)
                    continue;
                if (g_hook.thunk && base <= g_hook.thunk && g_hook.thunk < addr)
                    continue;
                for (std::size_t off = 0; off + need <= size; off += 0x10)
                {
                    const std::uintptr_t cand = base + off;
                    if (ignore && cand == ignore)
                        continue;
                    if (region_is_padding(cand, need))
                        return cand;
                }
                if (!fallback_rwx && mbi.Type == MEM_PRIVATE && (mbi.Protect & 0xFF) == PAGE_EXECUTE_READWRITE &&
                    size >= need + 0x40)
                {
                    const std::uintptr_t cand = base + size - need;
                    if (!ignore || cand != ignore)
                        fallback_rwx = cand;
                }
            }
            if (fallback_rwx)
                return fallback_rwx;
            return 0;
        }

        std::uintptr_t alloc_exec_page()
        {
            const std::uintptr_t p = ray_alloc(pg(), PAGE_EXECUTE_READWRITE);
            if (!p)
                return 0;
            const DWORD prot = query_protect(p);
            if (!is_executable_protect(prot))
            {
                ray_free(p);
                return 0;
            }
            return p;
        }

        bool install_hook()
        {
            if (g_hook.installed)
                return true;
            const std::uintptr_t base = ray_module(L"RobloxPlayerBeta.exe");
            if (!base)
            {
                set_status("install: no game module");
                return false;
            }
            const auto now = std::chrono::steady_clock::now();
            if (g_lastFail.time_since_epoch().count() != 0 && now - g_lastFail < std::chrono::milliseconds(1500))
                return false;
            const std::uintptr_t slot = base + desc_rva_z + bound_fn_offset;
            const std::uintptr_t fn = g_proc->read<std::uintptr_t>(slot);
            if (!ok_addr(fn))
            {
                set_status("install: bad slot fn (offsets stale?)");
                g_lastFail = now;
                return false;
            }
            if (!g_hook.state)
                g_hook.state = ray_alloc(pg(), PAGE_READWRITE);
            if (!g_hook.state)
            {
                set_status("install: no state mem");
                g_lastFail = now;
                return false;
            }
            auto thunk = make_hook_thunk(g_hook.state, fn);
            if (thunk.size() > k_stub_bytes)
            {
                set_status("install: thunk too big");
                g_lastFail = now;
                return false;
            }
            bool owned = false;
            std::uintptr_t stub = 0;
            std::uintptr_t ignore_cave = 0;
            for (int attempt = 0; attempt < 8 && !stub; ++attempt)
            {
                std::uintptr_t cand = find_exec_cave(k_stub_bytes, base, ignore_cave);
                if (!cand)
                    break;
                DWORD old_prot = 0;
                const bool prot_ok = protect_remote(cand, thunk.size(), PAGE_EXECUTE_READWRITE, &old_prot);
                if (!write_protected(cand, thunk.data(), thunk.size()))
                {
                    if (prot_ok)
                        protect_remote(cand, thunk.size(), old_prot, nullptr);
                    ignore_cave = cand;
                    continue;
                }
                stub = cand;
                owned = false;
            }
            if (!stub)
            {
                stub = alloc_exec_page();
                owned = stub != 0;
                if (stub)
                {
                    if (!write_protected(stub, thunk.data(), thunk.size()))
                    {
                        ray_free(stub);
                        stub = 0;
                        owned = false;
                    }
                }
            }
            if (!stub)
            {
                set_status("install: no exec cave");
                g_lastFail = now;
                return false;
            }
            RaycastState empty{};
            if (!w_mem(g_hook.state, &empty, sizeof(empty)))
            {
                set_status("install: state write fail");
                g_lastFail = now;
                if (owned)
                    ray_free(stub);
                return false;
            }
            FlushInstructionCache(g_proc->raw(), reinterpret_cast<void*>(stub), thunk.size());
            mark_cfg(stub);
            const DWORD prot = query_protect(stub);
            if (!is_executable_protect(prot))
            {
                set_status("install: cave not executable");
                g_lastFail = now;
                if (owned)
                    ray_free(stub);
                return false;
            }
            protect_remote(slot, 8, PAGE_READWRITE, nullptr);
            if (!write_protected(slot, &stub, sizeof(stub)) || g_proc->read<std::uintptr_t>(slot) != stub)
            {
                set_status("install: slot patch fail");
                g_lastFail = now;
                if (owned)
                    ray_free(stub);
                return false;
            }
            g_hook.module_base = base;
            g_hook.originalFunction = fn;
            g_hook.thunk = stub;
            g_hook.thunk_owned = owned;
            g_hook.installed = true;
            g_hook.active = false;
            set_status("installed: hook live, waiting for target");
            return true;
        }

        void remove_hook()
        {
            if (g_hook.installed)
                set_status("idle: hook removed");
            if (g_hook.installed && ok_addr(g_hook.originalFunction) && g_hook.module_base)
            {
                const std::uintptr_t slot = g_hook.module_base + desc_rva_z + bound_fn_offset;
                write_protected(slot, &g_hook.originalFunction, sizeof(g_hook.originalFunction));
            }
            if (g_hook.thunk && !g_hook.thunk_owned)
            {
                std::vector<std::uint8_t> pad(k_stub_bytes, 0xCC);
                write_protected(g_hook.thunk, pad.data(), pad.size());
            }
            if (g_hook.thunk && g_hook.thunk_owned)
                ray_free(g_hook.thunk);
            if (g_hook.state)
                ray_free(g_hook.state);
            g_hook = {};
            g_wallbang = false;
        }

        void ensure_hook(bool want)
        {
            using Clock = std::chrono::steady_clock;
            static std::uintptr_t s_cachedBase = 0;
            static auto s_lastQuery = Clock::now() - std::chrono::seconds(10);
            if (!want)
            {
                if (g_hook.installed)
                    remove_hook();
                return;
            }
            const auto now = Clock::now();
            if (!s_cachedBase || std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastQuery).count() >= 1500)
            {
                s_lastQuery = now;
                const std::uintptr_t base = ray_module(L"RobloxPlayerBeta.exe");
                if (base)
                {
                    if (s_cachedBase && base != s_cachedBase && g_hook.installed)
                        remove_hook();
                    s_cachedBase = base;
                }
                else if (!g_hook.installed)
                {
                    return;
                }
            }
            if (!g_hook.installed)
                install_hook();
        }

        void set_hook_active(bool on, const math::vec3& world_target, bool wallbang)
        {
            if (!on)
            {
                if (g_hook.active && g_hook.state)
                {
                    std::uint32_t v = 0;
                    w_mem(g_hook.state, &v, sizeof(v));
                    g_hook.active = false;
                }
                g_wallbang = false;
                if (g_hook.installed)
                    set_status("installed: hook live, waiting for target");
                return;
            }
            if (!g_hook.installed)
            {
                set_status("setactive: not installed");
                return;
            }
            float pos[3]{ world_target.x, world_target.y, world_target.z };
            const std::uint32_t flags = wallbang ? 1u : 0u;
            const float scale = 1.15f;
            const std::uint32_t one = 1;
            w_mem(g_hook.state + offsetof(RaycastState, reserved), &flags, sizeof(flags));
            w_mem(g_hook.state + offsetof(RaycastState, target_x), pos, sizeof(pos));
            w_mem(g_hook.state + offsetof(RaycastState, scale), &scale, sizeof(scale));
            w_mem(g_hook.state + offsetof(RaycastState, active), &one, sizeof(one));
            g_hook.active = true;
            g_wallbang = wallbang;
            set_status(wallbang ? "aiming: magic bullet (wallbang)" : "aiming: redirect active");
        }

        struct CachedTarget
        {
            uintptr_t playerAddr = 0;
            uintptr_t characterAddr = 0;
            uintptr_t humanoidAddr = 0;
            uintptr_t targetPrim = 0;
            int partIdx = -1;
        };

        std::vector<CachedTarget> s_targets;
        std::chrono::steady_clock::time_point s_lastResolve{};
        std::chrono::steady_clock::time_point s_lastScan{};

        void resolve_targets()
        {
            s_targets.clear();
            int wantPart = settings::aim_part;
            auto list = g_ctx.player_list();
            s_targets.reserve(list.size());
            for (auto& entry : list)
            {
                if (entry.addr == g_ctx.local_player.addr)
                    continue;
                node character = g_ctx.get_character(entry);
                if (!character)
                    continue;
                uintptr_t charAddr = character.addr;
                node humanoid = find_live_humanoid(character);
                if (!humanoid)
                    continue;
                node targetPart = resolve_part(character, wantPart);
                if (!targetPart)
                    targetPart = character.find_child("Head");
                if (!targetPart)
                    continue;
                uintptr_t prim = targetPart.primitive();
                if (!prim)
                    continue;
                CachedTarget t{};
                t.playerAddr = entry.addr;
                t.characterAddr = charAddr;
                t.humanoidAddr = humanoid.addr;
                t.targetPrim = prim;
                t.partIdx = wantPart;
                s_targets.push_back(t);
            }
        }
    }

    bool ready()
    {
        return g_hook.installed;
    }

    bool is_active()
    {
        return g_hook.active;
    }

    std::uint64_t calls()
    {
        if (!g_hook.installed || !g_hook.state || !connected())
            return 0;
        return g_proc->read<std::uint64_t>(g_hook.state + offsetof(RaycastState, calls));
    }

    const char* status()
    {
        return g_status;
    }

    void idle()
    {
        ensure_hook(false);
        s_targets.clear();
    }

    void shutdown()
    {
        ensure_hook(false);
        s_targets.clear();
    }

    void run(int w, int h)
    {
        if (!connected() || !g_ctx.ready || !g_ctx.camera || w <= 0 || h <= 0)
        {
            set_hook_active(false, math::vec3{}, false);
            rpbrx::aim::clear_debug_target();
            return;
        }

        auto now = std::chrono::steady_clock::now();
        if (now - s_lastScan < std::chrono::milliseconds(4))
            return;
        s_lastScan = now;

        if (pftarget::active())
        {
            pftarget::PfBest b{};
            if (!pftarget::select_best(w, h, b))
            {
                set_hook_active(false, math::vec3{}, false);
                rpbrx::aim::clear_debug_target();
                return;
            }
            rpbrx::aim::set_debug_target(b.world, b.model);
            if (!settings::aim_magicbullet)
            {
                set_hook_active(false, math::vec3{}, false);
                return;
            }
            ensure_hook(true);
            set_hook_active(true, b.world, true);
            return;
        }

        int wantPart = settings::aim_part;
        if (s_targets.empty()
            || now - s_lastResolve >= std::chrono::milliseconds(200)
            || s_targets[0].partIdx != wantPart)
        {
            resolve_targets();
            s_lastResolve = now;
            if (s_targets.empty())
            {
                set_hook_active(false, math::vec3{}, false);
                rpbrx::aim::clear_debug_target();
                return;
            }
        }

        float cx = static_cast<float>(w) * 0.5f;
        float cy = static_cast<float>(h) * 0.5f;
        const math::view_matrix& view = g_ctx.view;

        math::vec3 camPos = g_proc->read<math::vec3>(g_ctx.camera.addr + Offsets::Camera::Position);
        if (camPos.x == 0.0f && camPos.y == 0.0f && camPos.z == 0.0f)
            return;

        math::vec3 best{};
        bool found = false;
        float bestDist = settings::aim_fov;
        uintptr_t bestPlayer = 0;

        for (auto& t : s_targets)
        {
            uintptr_t charAddr = g_proc->read<uintptr_t>(t.playerAddr + Offsets::Player::ModelInstance);
            if (charAddr != t.characterAddr)
            {
                tracking::clear(t.playerAddr);
                continue;
            }
            float hp = g_proc->read<float>(t.humanoidAddr + Offsets::Humanoid::Health);
            if (!health::check(hp))
                continue;
            math::vec3 pos = g_proc->read<math::vec3>(t.targetPrim + Offsets::Primitive::Position);
            if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
                continue;

            tracking::push(t.playerAddr, pos.x, pos.y, pos.z);

            if (!distance::check(pos, camPos))
                continue;
            math::vec2 s{};
            if (!projection::to_screen(pos, view, static_cast<float>(w), static_cast<float>(h), s))
                continue;
            float dx = s.x - cx;
            float dy = s.y - cy;
            float d = sqrtf(dx * dx + dy * dy);
            { float hbs = hitbox::get_scale(); if (hbs > 1.0f) d /= hbs; }
            if (d < bestDist)
            {
                bestDist = d;
                best = pos;
                bestPlayer = t.playerAddr;
                found = true;
            }
        }

        if (!found)
        {
            set_hook_active(false, math::vec3{}, false);
            rpbrx::aim::clear_debug_target();
            return;
        }

        {
            float scanDt = 0.004f;
            float smoothLag = 0.0f;
            if (settings::aim_smoothing_enabled)
            {
                float sx = settings::aim_smoothing_x;
                if (sx < 1.0f) sx = 1.0f;
                smoothLag = (sx - 1.0f) * scanDt;
            }
            float leadT = scanDt + smoothLag;

            tracking::Velocity tv{};
            if (bestPlayer && tracking::sample(bestPlayer, tv))
            {
                float damp = tv.damp;
                float airDampY = tv.airborne ? 0.35f : 1.0f;
                math::vec3 lv = g_ctx.local_velocity;
                best.x += (tv.vx - lv.x) * leadT * damp;
                best.y += (tv.vy - lv.y) * leadT * damp * airDampY;
                best.z += (tv.vz - lv.z) * leadT * damp * airDampY;
            }
        }

        rpbrx::aim::set_debug_target(best, bestPlayer);

        if (!settings::aim_magicbullet)
        {
            set_hook_active(false, math::vec3{}, false);
            return;
        }

        ensure_hook(true);
        set_hook_active(true, best, true);
    }
}
