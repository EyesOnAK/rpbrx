#include "kitesp.h"
#include "../world/common.h"
#include <queue>
#include <algorithm>
#include <cmath>

namespace rpbrx::visual::kitesp
{
    // Kit entity containers found in workspace (from game dump analysis)
    // Maps workspace folder/model names to { display_label, settings_index }
    // Index: 0=beekeeper, 1=metal_detector, 2=sheep_herder, 3=eldertree, 4=ghost_catcher, 5=sorcerer, 6=star_collector
    struct KitMapping {
        const char* folderName;
        const char* displayLabel;
        int kitIdx;
        bool scanChildren;
    };

    static const KitMapping kMappings[] = {
        // Beekeeper — workspace folder "SheepModel" holds sheep, bees are tagged, beehives are models
        { "beehive",           "Beehive",      0, false },
        // Metal detector — hidden metal entities
        { "hidden-metal",      "Iron",         1, false },
        { "iron",              "Iron",         1, false },
        // Sheep herder — SheepModel folder children
        { "SheepModel",        "Sheep",        2, true  },
        // Eldertree — tree orbs
        { "TreeOrb",           "Tree Orb",     3, false },
        { "treeOrb",           "Tree Orb",     3, false },
        // Ghost catcher
        { "ghost_orb",         "Ghost Orb",    4, false },
        { "ghost",             "Ghost",        4, false },
        // Sorcerer
        { "alchemy_crystal",   "Crystal",      5, false },
        // Star collector
        { "stars",             "Star",         6, false },
        { "crit_star",         "Star",         6, false },
    };

    static std::vector<KitEntity> s_entities;
    static std::chrono::steady_clock::time_point s_lastScan{};
    static std::chrono::steady_clock::time_point s_lastFast{};

    static bool is_kit_enabled(int idx)
    {
        switch (idx) {
            case 0: return settings::kitesp_bee;
            case 1: return settings::kitesp_metal;
            case 2: return settings::kitesp_sheep;
            case 3: return settings::kitesp_elder;
            case 4: return settings::kitesp_ghost;
            case 5: return settings::kitesp_sorcerer;
            case 6: return settings::kitesp_star;
            default: return false;
        }
    }

    static math::vec3 get_entity_pos(node inst)
    {
        return world::instance_pos(inst);
    }

    static void full_scan()
    {
        if (!g_ctx.workspace || !g_proc || !g_proc->connected())
            return;

        math::vec3 lp = world::local_anchor();
        std::vector<KitEntity> out;
        out.reserve(128);
        std::unordered_set<uintptr_t> seen;

        std::queue<std::pair<node, int>> q;
        q.push({ g_ctx.workspace, 0 });
        size_t scanned = 0;

        while (!q.empty() && scanned < 15000) {
            auto [cur, depth] = q.front();
            q.pop();
            ++scanned;
            if (!cur) continue;

            std::string nm = cur.name();
            std::string cls = cur.class_name();

            for (auto& m : kMappings) {
                if (nm != m.folderName) continue;
                if (!is_kit_enabled(m.kitIdx)) continue;

                if (m.scanChildren) {
                    for (auto& child : cur.children()) {
                        if (!child || seen.count(child.addr)) continue;
                        seen.insert(child.addr);
                        math::vec3 p = get_entity_pos(child);
                        if (p.x == 0.0f && p.y == 0.0f && p.z == 0.0f) continue;
                        float d = (p - lp).length();
                        if (d > 500.0f) continue;
                        KitEntity e{};
                        e.addr = child.addr;
                        e.label = m.displayLabel;
                        e.pos = p;
                        e.kitIdx = m.kitIdx;
                        out.push_back(std::move(e));
                    }
                } else {
                    if (seen.count(cur.addr)) break;
                    seen.insert(cur.addr);
                    math::vec3 p = get_entity_pos(cur);
                    if (p.x == 0.0f && p.y == 0.0f && p.z == 0.0f) break;
                    float d = (p - lp).length();
                    if (d > 500.0f) break;
                    KitEntity e{};
                    e.addr = cur.addr;
                    e.label = m.displayLabel;
                    e.pos = p;
                    e.kitIdx = m.kitIdx;
                    out.push_back(std::move(e));
                }
                break;
            }

            if (depth < 4 && (cls == "Workspace" || cls == "Folder" || cls == "Model")) {
                for (auto& k : cur.children()) {
                    if (q.size() > 10000) break;
                    q.push({ k, depth + 1 });
                }
            }
        }

        s_entities.swap(out);
    }

    static void fast_refresh()
    {
        math::vec3 lp = world::local_anchor();
        for (auto it = s_entities.begin(); it != s_entities.end();) {
            uintptr_t par = g_proc->read<uintptr_t>(it->addr + Offsets::Instance::Parent);
            if (!par) {
                it = s_entities.erase(it);
                continue;
            }
            math::vec3 p = get_entity_pos(node{ it->addr });
            if (p.x == 0.0f && p.y == 0.0f && p.z == 0.0f) {
                it = s_entities.erase(it);
                continue;
            }
            it->pos = p;
            ++it;
        }
    }

    void tick(ImDrawList* dl, int w, int h)
    {
        if (!dl || !settings::kitesp_enabled) return;
        if (!g_proc || !g_proc->connected() || !g_ctx.workspace) return;

        auto now = std::chrono::steady_clock::now();
        if (s_lastFast == std::chrono::steady_clock::time_point{} ||
            std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastFast).count() >= 150) {
            s_lastFast = now;
            if (!s_entities.empty()) fast_refresh();
        }
        if (s_lastScan == std::chrono::steady_clock::time_point{} ||
            std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastScan).count() >= 3000) {
            s_lastScan = now;
            full_scan();
        }

        if (s_entities.empty()) return;

        math::view_matrix view{};
        if (!world::fetch_view(view)) return;

        float fw = (float)w;
        float fh = (float)h;
        ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
        float fontSize = 13.0f;

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
            settings::kitesp_color[0], settings::kitesp_color[1],
            settings::kitesp_color[2], settings::kitesp_color[3]));
        ImU32 ocol = settings::kitesp_outline ?
            ImGui::ColorConvertFloat4ToU32(ImVec4(
                settings::kitesp_outline_color[0], settings::kitesp_outline_color[1],
                settings::kitesp_outline_color[2], settings::kitesp_outline_color[3]))
            : 0;

        math::vec3 lp = world::local_anchor();

        for (auto& e : s_entities) {
            if (!is_kit_enabled(e.kitIdx)) continue;

            math::vec2 sp{};
            if (!projection::to_screen(e.pos, view, fw, fh, sp)) continue;
            if (sp.x < -50.0f || sp.x > fw + 50.0f || sp.y < -50.0f || sp.y > fh + 50.0f) continue;

            float dist = (e.pos - lp).length();
            char buf[64];
            snprintf(buf, sizeof(buf), "%s [%.0f]", e.label.c_str(), dist);

            ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
            float tx = sp.x - ts.x * 0.5f;
            float ty = sp.y - ts.y * 0.5f;

            if (settings::kitesp_outline) {
                static const ImVec2 offsets[4] = {{-1,-1},{1,-1},{-1,1},{1,1}};
                for (int i = 0; i < 4; ++i)
                    dl->AddText(font, fontSize, ImVec2(tx + offsets[i].x, ty + offsets[i].y), ocol, buf);
            }
            dl->AddText(font, fontSize, ImVec2(tx, ty), col, buf);
        }
    }

    void clear()
    {
        s_entities.clear();
        s_lastScan = {};
        s_lastFast = {};
    }
}
