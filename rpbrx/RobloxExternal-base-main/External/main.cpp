#include "src/app/app.h"
#include "src/utility/logger/logger.h"
#include "src/utility/memory/syscall.h"
#include <Windows.h>
#include <shellapi.h>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>

bool g_menuOpen = false;
bool g_vsync = false;
bool g_streamProof = false;
int g_fpsCap = 0;
float g_localSpeed = -1.0f;

namespace {

bool is_admin() {
    BOOL admin = FALSE;
    SID_IDENTIFIER_AUTHORITY nt = SECURITY_NT_AUTHORITY;
    PSID group = nullptr;
    if (AllocateAndInitializeSid(&nt, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0, &group)) {
        CheckTokenMembership(nullptr, group, &admin);
        FreeSid(group);
    }
    return admin != FALSE;
}

void relaunch_as_admin() {
    wchar_t exe[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exe, MAX_PATH);
    SHELLEXECUTEINFOW sei{};
    sei.cbSize = sizeof(sei);
    sei.lpVerb = L"runas";
    sei.lpFile = exe;
    sei.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteExW(&sei))
        rpbrx::app::badge("priv", rpbrx::app::C_RED, "elevation declined - run rpbrx.exe as administrator");
}

void on_terminate() {
    Logger::log("CRASH", "std::terminate (unhandled C++ exception?)");
    Logger::write_crash(nullptr, "terminate");
    MessageBoxA(nullptr, "rpbrx terminated unexpectedly.\nSee the .crash file on your Desktop.", "rpbrx crash", MB_ICONERROR);
    abort();
}
void __cdecl on_invalid_param(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t) {
    Logger::log("CRASH", "invalid CRT parameter");
    Logger::write_crash(nullptr, "invalid_parameter");

}
void __cdecl on_purecall() {
    Logger::log("CRASH", "pure virtual function call");
    Logger::write_crash(nullptr, "purecall");
    MessageBoxA(nullptr, "rpbrx crashed (purecall).\nSee the .crash file on your Desktop.", "rpbrx crash", MB_ICONERROR);
    _exit(3);
}
void on_sigabrt(int) {
    Logger::log("CRASH", "abort() called");
    Logger::write_crash(nullptr, "abort");
    _exit(3);
}

}

int main()
{
    using namespace rpbrx::app;
    SetUnhandledExceptionFilter(CrashHandler);
    AddVectoredExceptionHandler(1, [](PEXCEPTION_POINTERS ep)->LONG {
        if (IsDebuggerPresent()) return EXCEPTION_CONTINUE_SEARCH;
        DWORD code = ep->ExceptionRecord->ExceptionCode;
        if (code == 0x406D1388 || code == 0x40010006 || code == 0x4001000A || code == 0xE06D7363 ||
            code == 0x80000003 || code == 0x80000004 ||
            (code >= 0x000006A4 && code <= 0x000007CF)) return EXCEPTION_CONTINUE_SEARCH;
        static long long lastMs=0; static int count=0;
        long long now = GetTickCount64();
        if (now - lastMs < 500) return EXCEPTION_CONTINUE_SEARCH;
        if (count++ > 20) return EXCEPTION_CONTINUE_SEARCH;
        lastMs = now;
        Logger::write_crash(ep, "VEH");
        return EXCEPTION_CONTINUE_SEARCH;
    });
    std::set_terminate(on_terminate);
    _set_invalid_parameter_handler(on_invalid_param);
    _set_purecall_handler(on_purecall);
    signal(SIGABRT, on_sigabrt);
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
    Logger::quiet = true;
    Logger::init();

    rpbrx::syscalls::init();
    LOG("boot", rpbrx::syscalls::ready()
        ? "memory path: direct syscalls (NtReadVirtualMemory/NtWriteVirtualMemory)"
        : "memory path: RPM/WPM fallback (syscall init failed)");
    SetConsoleTitleA("rpbrx");

    std::cout << "\n";
    badge("rpbrx", C_CYAN, "external - INSERT to open menu");
    std::cout << "\n";

    HANDLE mutex = CreateMutexW(nullptr, FALSE, L"rpbrx-overlay-mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        badge("fail", C_RED, "rpbrx is already running");
        MessageBoxW(nullptr, L"rpbrx is already running.", L"rpbrx", MB_OK | MB_ICONINFORMATION);
        if (mutex) CloseHandle(mutex);
        return 0;
    }

    if (!is_admin()) {
        badge("priv", C_YELLOW, "administrator required - relaunching elevated...");
        relaunch_as_admin();
        if (mutex) CloseHandle(mutex);
        return 0;
    }
    badge("priv", C_GREEN, "running as administrator");
    badge("attaching", C_GREEN, "waiting for RobloxPlayerBeta.exe...");
    LOG("boot", "session start");

    int rc = rpbrx::app::run();
    if (mutex) CloseHandle(mutex);
    return rc;
}
