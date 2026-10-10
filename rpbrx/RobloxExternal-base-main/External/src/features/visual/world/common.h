#pragma once
#include "../../../menu/imgui/imgui.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/cache/math.h"
#include "../../../utility/struct/offsets.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/resources/font.h"
#include "../../../utility/logger/logger.h"
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <cmath>

namespace rpbrx::world
{
    struct Entry
    {
        std::string name;
        math::vec3 pos{};
        math::vec3 headPos{};
        math::vec3 rootPos{};
        float dist = 0.0f;
        int idx = -1;
        bool plant = false;
        bool bots = false;
        bool animal = false;
        bool tool = false;
        bool crate = false;
        bool keycard = false;
        float hp = 0.0f;
        float maxHp = 0.0f;
        std::uintptr_t humanoid = 0;
        std::uintptr_t inst = 0;
        std::uintptr_t headPart = 0;
        std::uintptr_t rootPart = 0;
        int zeroStreak = 0;
    };

    inline std::vector<Entry> g_entries;
    inline std::chrono::steady_clock::time_point g_lastScan{};
    inline std::chrono::steady_clock::time_point g_lastFast{};

    constexpr std::size_t kMaxScan = 12000;
    constexpr std::size_t kMaxQueue = 8000;
    constexpr float kMaxDist = 20000.0f;

    inline const char* kOreNames[3] = { "Stone_Node", "Phosphate Ore", "Metal_Node" };
    inline const char* kOreDisplay[3] = { "Stone", "Phosphate", "Metal" };
    inline const char* kPlantNames[7] = { "Wool Plant", "Blueberry Plant", "Raspberry Plant", "Lemon Plant", "Corn Plant", "Pumpkin Plant", "Tomato Plant" };
    inline const char* kBotsNames[3] = { "Boris", "Brutus", "Soldier" };
    inline const char* kAnimalNames[3] = { "PREFAB_ANIMAL_DEER", "PREFAB_ANIMAL_WILDBOAR", "PREFAB_ANIMAL_WOLF" };
    inline const char* kAnimalDisplay[3] = { "Deer", "WildBoar", "Wolf" };
    inline const char* kToolNames[7] = { "Base Cabinet", "Small Storage Box", "Large Storage Box", "Anvil", "Furnace", "Storage Cabinet", "Sleeping Bag" };
    inline const char* kCrateNames[8] = { "Care Package", "Food Crate", "Wooden Crate", "Locked Wooden Crate", "Locked Steel Crate", "Locked Metal Crate", "Timed Crate", "Brutus Locker" };
    inline const char* kKeycardNames[5] = { "Yellow", "Purple", "Red", "Pink", "Black" };

    inline bool match_node(const std::string& n, bool& plant, bool& bots, bool& animal, bool& tool, bool& crate, bool& keycard, int& idx)
    {
        plant = false;
        bots = false;
        animal = false;
        tool = false;
        crate = false;
        keycard = false;
        idx = -1;
        for (int i = 0; i < 3; ++i)
        {
            if (n != kOreNames[i])
                continue;
            bool on = (i == 0) ? settings::world_ore_stone
                : (i == 1) ? settings::world_ore_phosphate
                : settings::world_ore_metal;
            if (!on)
                return false;
            idx = i;
            return true;
        }
        for (int i = 0; i < 7; ++i)
        {
            if (n != kPlantNames[i])
                continue;
            if (!settings::world_plant_sel[i])
                return false;
            plant = true;
            idx = i;
            return true;
        }
        if (settings::world_bots_enabled)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (n != kBotsNames[i])
                    continue;
                if (!settings::world_bots_sel[i])
                    return false;
                bots = true;
                idx = i;
                return true;
            }
        }
        if (settings::world_animals_enabled)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (n != kAnimalNames[i])
                    continue;
                if (!settings::world_animals_sel[i])
                    return false;
                animal = true;
                idx = i;
                return true;
            }
        }
        if (settings::world_tools_enabled)
        {
            for (int i = 0; i < 7; ++i)
            {
                if (n != kToolNames[i])
                    continue;
                if (!settings::world_tools_sel[i])
                    return false;
                tool = true;
                idx = i;
                return true;
            }
        }
        if (settings::world_crates_enabled)
        {
            for (int i = 0; i < 8; ++i)
            {
                if (n != kCrateNames[i])
                    continue;
                if (!settings::world_crates_sel[i])
                    return false;
                crate = true;
                idx = i;
                return true;
            }
        }
        if (settings::world_keycards_enabled)
        {
            if (n != "Keycards")
            {
                for (int i = 0; i < 5; ++i)
                {
                    if (n.find(kKeycardNames[i]) == std::string::npos)
                        continue;
                    if (!settings::world_keycards_sel[i])
                        return false;
                    keycard = true;
                    idx = i;
                    return true;
                }
            }
        }
        return false;
    }

    inline math::vec3 instance_pos(node inst);

    inline math::vec3 keycard_pos(node inst)
    {
        if (!inst)
            return {};
        std::vector<std::pair<node, int>> st;
        st.emplace_back(inst, 0);
        int vis = 0;
        while (!st.empty() && vis < 60)
        {
            auto [nd, depth] = st.back();
            st.pop_back();
            ++vis;
            if (nd.name() == "HumanoidRootPart" &&
                (nd.class_name() == "Part" || nd.class_name() == "MeshPart"))
            {
                math::vec3 p = instance_pos(nd);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                    return p;
                return {};
            }
            if (depth < 3)
            {
                for (auto& c : nd.children())
                    st.emplace_back(c, depth + 1);
            }
        }
        return {};
    }

    inline math::vec3 instance_pos(node inst)
    {
        if (!inst)
            return {};
        uintptr_t prim = inst.primitive();
        if (prim)
        {
            math::vec3 p = g_proc->read<math::vec3>(prim + Offsets::Primitive::Position);
            if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                return p;
        }
        uintptr_t pp = g_proc->read<uintptr_t>(inst.addr + Offsets::Model::PrimaryPart);
        if (pp)
        {
            uintptr_t pprim = g_proc->read<uintptr_t>(pp + Offsets::BasePart::Primitive);
            if (pprim)
            {
                math::vec3 p = g_proc->read<math::vec3>(pprim + Offsets::Primitive::Position);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                    return p;
            }
        }
        for (auto& c : inst.children())
        {
            uintptr_t cp = c.primitive();
            if (cp)
            {
                math::vec3 p = g_proc->read<math::vec3>(cp + Offsets::Primitive::Position);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                    return p;
            }
            for (auto& cc : c.children())
            {
                uintptr_t ccp = cc.primitive();
                if (!ccp)
                    continue;
                math::vec3 p = g_proc->read<math::vec3>(ccp + Offsets::Primitive::Position);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                    return p;
            }
        }
        return {};
    }

    inline math::vec3 local_anchor()
    {
        if (g_ctx.local_root)
        {
            math::vec3 p = g_ctx.local_root.position();
            if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                return p;
        }
        if (g_ctx.camera)
            return g_proc->read<math::vec3>(g_ctx.camera.addr + Offsets::Camera::Position);
        return {};
    }

    inline math::vec3 part_pos(std::uintptr_t part)
    {
        if (!part)
            return {};
        uintptr_t prim = g_proc->read<std::uintptr_t>(part + Offsets::BasePart::Primitive);
        if (!prim)
            return {};
        return g_proc->read<math::vec3>(prim + Offsets::Primitive::Position);
    }

    inline void fast_refresh()
    {
        math::vec3 lp = local_anchor();
        for (auto it = g_entries.begin(); it != g_entries.end();)
        {
            std::uintptr_t par = g_proc->read<std::uintptr_t>(it->inst + Offsets::Instance::Parent);
            math::vec3 p = instance_pos(node{ it->inst });
            bool zero = (p.x == 0.0f && p.y == 0.0f && p.z == 0.0f);
            if (!par || zero)
            {
                if (++it->zeroStreak >= 5)
                {
                    it = g_entries.erase(it);
                    continue;
                }
                ++it;
                continue;
            }
            it->zeroStreak = 0;
            it->pos = p;
            it->dist = (p - lp).length();
            if (it->bots || it->animal)
            {
                math::vec3 hp = part_pos(it->headPart);
                math::vec3 rp = part_pos(it->rootPart);
                if (!(hp.x == 0.0f && hp.y == 0.0f && hp.z == 0.0f))
                    it->headPos = hp;
                if (!(rp.x == 0.0f && rp.y == 0.0f && rp.z == 0.0f))
                {
                    it->rootPos = rp;
                    it->pos = rp;
                    it->dist = (rp - lp).length();
                }
                if (it->animal && it->humanoid)
                {
                    float ahp = g_proc->read<float>(it->humanoid + Offsets::Humanoid::Health);
                    float amx = g_proc->read<float>(it->humanoid + Offsets::Humanoid::MaxHealth);
                    if (std::isfinite(ahp) && ahp > 0.0f)
                    {
                        it->hp = ahp;
                        if (std::isfinite(amx) && amx > 0.0f)
                            it->maxHp = amx;
                    }
                    else
                    {
                        it = g_entries.erase(it);
                        continue;
                    }
                }
            }
            ++it;
        }
    }

    inline void full_scan()
    {
        if (!g_ctx.workspace || !g_proc || !g_proc->connected())
            return;
        math::vec3 lp = local_anchor();
        std::vector<Entry> out;
        out.reserve(256);
        std::queue<node> q;
        q.push(g_ctx.workspace);
        std::size_t scanned = 0;
        while (!q.empty() && scanned < kMaxScan)
        {
            node cur = q.front();
            q.pop();
            ++scanned;
            if (!cur)
                continue;
            std::string cls = cur.class_name();
            std::string nm = cur.name();
            bool isPlant = false;
            bool isBots = false;
            bool isAnimal = false;
            bool isTool = false;
            bool isCrate = false;
            bool isKeycard = false;
            int idx = -1;
            if (cls == "Folder" && nm != "Keycards" && settings::world_keycards_enabled)
            {
                for (int i = 0; i < 5; ++i)
                {
                    if (nm.find(kKeycardNames[i]) == std::string::npos)
                        continue;
                    if (!settings::world_keycards_sel[i])
                        break;
                    math::vec3 kp = keycard_pos(cur);
                    if (kp.x == 0.0f && kp.y == 0.0f && kp.z == 0.0f)
                        break;
                    float kd = (kp - lp).length();
                    if (kd < kMaxDist)
                    {
                        Entry e{};
                        e.name = nm;
                        e.pos = kp;
                        e.dist = kd;
                        e.idx = i;
                        e.keycard = true;
                        e.inst = cur.addr;
                        out.push_back(std::move(e));
                    }
                    break;
                }
            }
            if ((cls == "Model" || cls == "MeshPart" || cls == "Part") && match_node(nm, isPlant, isBots, isAnimal, isTool, isCrate, isKeycard, idx))
            {
                math::vec3 p = instance_pos(cur);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f))
                {
                    float d = (p - lp).length();
                    if (d < kMaxDist)
                    {
                        Entry e{};
                        e.name = isPlant ? kPlantNames[idx] : (isBots ? kBotsNames[idx] : (isAnimal ? kAnimalDisplay[idx] : (isTool ? kToolNames[idx] : (isCrate ? kCrateNames[idx] : (isKeycard ? nm : kOreDisplay[idx])))));
                        e.pos = p;
                        e.dist = d;
                        e.idx = idx;
                        e.plant = isPlant;
                        e.bots = isBots;
                        e.animal = isAnimal;
                        e.tool = isTool;
                        e.crate = isCrate;
                        e.keycard = isKeycard;
                        e.inst = cur.addr;
                        if (isBots || isAnimal)
                        {
                            node head = cur.find_child("Head");
                            node hrp = cur.find_child("HumanoidRootPart");
                            e.headPart = head ? head.addr : 0;
                            e.rootPart = hrp ? hrp.addr : 0;
                            math::vec3 hp = part_pos(e.headPart);
                            math::vec3 rp = part_pos(e.rootPart);
                            if (!(hp.x == 0.0f && hp.y == 0.0f && hp.z == 0.0f))
                                e.headPos = hp;
                            if (!(rp.x == 0.0f && rp.y == 0.0f && rp.z == 0.0f))
                            {
                                e.rootPos = rp;
                                e.pos = rp;
                                e.dist = (rp - lp).length();
                            }
                        }
                        if (isAnimal)
                        {
                            node hum = cur.find_child_of_class("Humanoid");
                            if (hum)
                            {
                                e.humanoid = hum.addr;
                                float ahp = g_proc->read<float>(hum.addr + Offsets::Humanoid::Health);
                                float amx = g_proc->read<float>(hum.addr + Offsets::Humanoid::MaxHealth);
                                if (std::isfinite(ahp) && ahp > 0.0f)
                                {
                                    e.hp = ahp;
                                    e.maxHp = (std::isfinite(amx) && amx > 0.0f) ? amx : 100.0f;
                                }
                                else
                                {
                                    continue;
                                }
                            }
                            else
                            {
                                e.hp = 100.0f;
                                e.maxHp = 100.0f;
                            }
                        }
                        out.push_back(std::move(e));
                    }
                }
            }
            if (cls == "Workspace" || cls == "Folder" || cls == "Model")
            {
                for (auto& k : cur.children())
                {
                    if (q.size() > kMaxQueue)
                        break;
                    q.push(k);
                }
            }
        }
        int nOre = 0, nPlant = 0, nBots = 0, nAnimal = 0, nTool = 0, nCrate = 0, nKey = 0;
        for (auto& e : out)
        {
            if (e.bots)
                ++nBots;
            else if (e.animal)
                ++nAnimal;
            else if (e.tool)
                ++nTool;
            else if (e.crate)
                ++nCrate;
            else if (e.keycard)
                ++nKey;
            else if (e.plant)
                ++nPlant;
            else
                ++nOre;
        }
        static int s_lastOre = -1, s_lastPlant = -1, s_lastBots = -1, s_lastAnimal = -1, s_lastTool = -1, s_lastCrate = -1, s_lastKey = -1;
        if (nOre != s_lastOre || nPlant != s_lastPlant || nBots != s_lastBots || nAnimal != s_lastAnimal || nTool != s_lastTool || nCrate != s_lastCrate || nKey != s_lastKey)
        {
            s_lastOre = nOre;
            s_lastPlant = nPlant;
            s_lastBots = nBots;
            s_lastAnimal = nAnimal;
            s_lastTool = nTool;
            s_lastCrate = nCrate;
            s_lastKey = nKey;
            LOGF("world", "scan: %d ore %d plant %d bots %d animals %d tools %d crates %d keys", nOre, nPlant, nBots, nAnimal, nTool, nCrate, nKey);
        }
        g_entries.swap(out);
    }

    inline void refresh_entries()
    {
        auto now = std::chrono::steady_clock::now();
        if (g_lastFast == std::chrono::steady_clock::time_point{} ||
            std::chrono::duration_cast<std::chrono::milliseconds>(now - g_lastFast).count() >= 100)
        {
            g_lastFast = now;
            if (!g_entries.empty())
                fast_refresh();
        }
        if (g_lastScan == std::chrono::steady_clock::time_point{} ||
            std::chrono::duration_cast<std::chrono::milliseconds>(now - g_lastScan).count() >= 2000)
        {
            g_lastScan = now;
            full_scan();
        }
    }

    inline void clear_entries()
    {
        g_entries.clear();
        g_lastScan = std::chrono::steady_clock::time_point{};
        g_lastFast = std::chrono::steady_clock::time_point{};
    }

    inline bool fetch_view(math::view_matrix& out)
    {
        if (!g_proc || !g_proc->connected())
            return false;
        uint64_t vis = g_proc->read<uint64_t>(g_proc->base() + Offsets::VisualEngine::Pointer);
        if (!vis)
            return false;
        out = g_proc->read<math::view_matrix>(vis + Offsets::VisualEngine::ViewMatrix);
        return true;
    }

    inline void draw_text(ImDrawList* dl, ImFont* font, float x, float y, ImU32 col, const char* text)
    {
        if (!dl || !text || !text[0])
            return;
        if (!font)
            font = ImGui::GetFont();
        float ix = std::floor(x);
        float iy = std::floor(y);
        dl->AddText(font, 13.0f, ImVec2(ix - 1.0f, iy - 1.0f), IM_COL32(0, 0, 0, 255), text);
        dl->AddText(font, 13.0f, ImVec2(ix + 1.0f, iy - 1.0f), IM_COL32(0, 0, 0, 255), text);
        dl->AddText(font, 13.0f, ImVec2(ix - 1.0f, iy + 1.0f), IM_COL32(0, 0, 0, 255), text);
        dl->AddText(font, 13.0f, ImVec2(ix + 1.0f, iy + 1.0f), IM_COL32(0, 0, 0, 255), text);
        dl->AddText(font, 13.0f, ImVec2(ix, iy), col, text);
    }
}
