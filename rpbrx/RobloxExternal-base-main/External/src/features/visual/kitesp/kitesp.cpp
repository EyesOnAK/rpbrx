#include "kitesp.h"
#include <queue>
#include <algorithm>
#include <cmath>

namespace rpbrx::visual::kitesp
{
    // Catsix KitESP uses CollectionService:GetTagged(tag) to find objects.
    // External can't call CollectionService, so we BFS-scan Workspace by instance name.
    //
    // Each mapping: { instance name to match, display label, kit index, scanChildren flag }
    // When scanChildren is true, the matched container's children are the actual entities
    // (e.g. SheepModel is a folder holding individual sheep models).
    //
    // Kit indices: 0=alchemist, 1=beekeeper, 2=bigman(elder tree), 3=ghost_catcher,
    //              4=metal_detector, 5=sheep_herder, 6=sorcerer, 7=star_collector
    struct KitMapping {
        const char* matchName;
        const char* displayLabel;
        int kitIdx;
        bool scanChildren;
    };

    static const KitMapping kMappings[] = {
        // Alchemist — catsix tag: "alchemist_ingedients" (sic), icon: "wild_flower"
        { "alchemist_ingedients", "Ingredient",    0, false },
        { "alchemy_crystal",     "Crystal",        0, false },
        { "wild_flower",         "Flower",         0, false },
        // Beekeeper — catsix tag: "bee", icon: "bee"
        { "bee",                 "Bee",            1, false },
        { "beehive",             "Beehive",        1, false },
        { "BeeSwarm",            "Bee Swarm",      1, true  },
        // Bigman / Elder Tree — catsix tag: "treeOrb", icon: "natures_essence_1"
        { "treeOrb",             "Tree Orb",       2, false },
        { "natures_essence_1",   "Essence",        2, false },
        // Ghost Catcher — catsix tag: "ghost", icon: "ghost_orb"
        { "ghost",               "Ghost",          3, false },
        { "ghost_orb",           "Ghost Orb",      3, false },
        // Metal Detector — catsix tag: "hidden-metal", icon: "iron"
        { "hidden-metal",        "Metal",          4, false },
        { "iron",                "Iron",           4, false },
        // Sheep Herder — catsix tag: "SheepModel", icon: "purple_hay_bale"
        { "SheepModel",          "Sheep",          5, true  },
        { "purple_hay_bale",     "Hay Bale",       5, false },
        // Sorcerer — catsix tag: "alchemy_crystal", icon: "wild_flower"
        { "sorcerer_orb",        "Sorcerer Orb",   6, false },
        { "SorcererCrystal",     "Crystal",        6, false },
        // Star Collector — catsix tag: "stars", icon: "crit_star"
        { "stars",               "Star",           7, false },
        { "crit_star",           "Star",           7, false },
        { "FallingStar",         "Star",           7, false },
    };

    static std::vector<KitEntity> s_entities;
    static std::chrono::steady_clock::time_point s_lastScan{};
    static std::chrono::steady_clock::time_point s_lastFast{};

    static bool is_kit_enabled(int idx)
    {
        switch (idx) {
            case 0: return settings::kitesp_alchemist;
            case 1: return settings::kitesp_bee;
            case 2: return settings::kitesp_bigman;
            case 3: return settings::kitesp_ghost;
            case 4: return settings::kitesp_metal;
            case 5: return settings::kitesp_sheep;
            case 6: return settings::kitesp_sorcerer;
            case 7: return settings::kitesp_star;
            default: return false;
        }
    }

    static bool any_kit_enabled()
    {
        for (int i = 0; i < 8; ++i)
            if (is_kit_enabled(i)) return true;
        return false;
    }

    static math::vec3 get_entity_pos(node inst)
    {
        if (!inst) return {};

        uintptr_t prim = inst.primitive();
        if (prim) {
            math::vec3 p = g_proc->read<math::vec3>(prim + Offsets::Primitive::Position);
            if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f)) return p;
        }

        uintptr_t pp = g_proc->read<uintptr_t>(inst.addr + Offsets::Model::PrimaryPart);
        if (pp) {
            uintptr_t pprim = g_proc->read<uintptr_t>(pp + Offsets::BasePart::Primitive);
            if (pprim) {
                math::vec3 p = g_proc->read<math::vec3>(pprim + Offsets::Primitive::Position);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f)) return p;
            }
        }

        for (auto& c : inst.children()) {
            uintptr_t cp = c.primitive();
            if (cp) {
                math::vec3 p = g_proc->read<math::vec3>(cp + Offsets::Primitive::Position);
                if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f)) return p;
            }
        }
        return {};
    }

    static math::vec3 kit_local_anchor()
    {
        if (g_ctx.local_root) {
            math::vec3 p = g_ctx.local_root.position();
            if (!(p.x == 0.0f && p.y == 0.0f && p.z == 0.0f)) return p;
        }
        if (g_ctx.camera)
            return g_proc->read<math::vec3>(g_ctx.camera.addr + Offsets::Camera::Position);
        return {};
    }

    static bool kit_fetch_view(math::view_matrix& out)
    {
        if (!g_proc || !g_proc->connected()) return false;
        uint64_t vis = g_proc->read<uint64_t>(g_proc->base() + Offsets::VisualEngine::Pointer);
        if (!vis) return false;
        out = g_proc->read<math::view_matrix>(vis + Offsets::VisualEngine::ViewMatrix);
        return true;
    }

    static void full_scan()
    {
        if (!g_ctx.workspace || !g_proc || !g_proc->connected())
            return;
        if (!any_kit_enabled())
            return;

        std::vector<KitEntity> out;
        out.reserve(128);
        std::unordered_set<uintptr_t> seen;

        // BFS from Workspace — no class whitelist, no distance filter.
        // Catsix uses CollectionService which searches globally; we approximate
        // by walking the full Workspace tree up to depth/node limits.
        std::queue<std::pair<node, int>> q;
        q.push({ g_ctx.workspace, 0 });
        size_t scanned = 0;
        constexpr int kMaxDepth = 10;
        constexpr size_t kMaxNodes = 50000;

        while (!q.empty() && scanned < kMaxNodes) {
            auto [cur, depth] = q.front();
            q.pop();
            if (!cur) continue;
            ++scanned;

            std::string nm = cur.name();

            for (auto& m : kMappings) {
                if (nm != m.matchName) continue;
                if (!is_kit_enabled(m.kitIdx)) continue;

                if (m.scanChildren) {
                    for (auto& child : cur.children()) {
                        if (!child || seen.count(child.addr)) continue;
                        seen.insert(child.addr);
                        math::vec3 p = get_entity_pos(child);
                        if (p.x == 0.0f && p.y == 0.0f && p.z == 0.0f) continue;
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
                    KitEntity e{};
                    e.addr = cur.addr;
                    e.label = m.displayLabel;
                    e.pos = p;
                    e.kitIdx = m.kitIdx;
                    out.push_back(std::move(e));
                }
                break;
            }

            // Recurse into ALL children regardless of class.
            // CollectionService finds tagged instances anywhere — we can't filter
            // by class without risking missing objects nested inside unknown types.
            if (depth < kMaxDepth) {
                for (auto& k : cur.children()) {
                    if (q.size() > 40000) break;
                    q.push({ k, depth + 1 });
                }
            }
        }

        s_entities.swap(out);
    }

    static void fast_refresh()
    {
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
            std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastScan).count() >= 2000) {
            s_lastScan = now;
            full_scan();
        }

        if (s_entities.empty()) return;

        math::view_matrix view{};
        if (!kit_fetch_view(view)) return;

        float fw = (float)w;
        float fh = (float)h;
        ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
        float fontSize = 13.0f * settings::kitesp_scale;

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
            settings::kitesp_color[0], settings::kitesp_color[1],
            settings::kitesp_color[2], settings::kitesp_color[3]));
        ImU32 bgCol = settings::kitesp_background
            ? ImGui::ColorConvertFloat4ToU32(ImVec4(
                settings::kitesp_bg_color[0], settings::kitesp_bg_color[1],
                settings::kitesp_bg_color[2], settings::kitesp_bg_color[3]))
            : 0;

        math::vec3 lp = kit_local_anchor();
        bool showDist = settings::kitesp_show_distance;

        for (auto& e : s_entities) {
            if (!is_kit_enabled(e.kitIdx)) continue;

            math::vec2 sp{};
            if (!projection::to_screen(e.pos, view, fw, fh, sp)) continue;
            if (sp.x < -50.0f || sp.x > fw + 50.0f || sp.y < -50.0f || sp.y > fh + 50.0f) continue;

            char buf[64];
            if (showDist) {
                float dist = (e.pos - lp).length();
                snprintf(buf, sizeof(buf), "%s [%.0f]", e.label.c_str(), dist);
            } else {
                snprintf(buf, sizeof(buf), "%s", e.label.c_str());
            }

            ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
            float tx = sp.x - ts.x * 0.5f;
            float ty = sp.y - ts.y * 0.5f;

            if (settings::kitesp_background) {
                float pad = 3.0f;
                dl->AddRectFilled(
                    ImVec2(tx - pad, ty - pad),
                    ImVec2(tx + ts.x + pad, ty + ts.y + pad),
                    bgCol, 4.0f);
            }

            static const ImVec2 offsets[4] = {{-1,-1},{1,-1},{-1,1},{1,1}};
            for (int i = 0; i < 4; ++i)
                dl->AddText(font, fontSize, ImVec2(tx + offsets[i].x, ty + offsets[i].y), IM_COL32(0,0,0,200), buf);
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
