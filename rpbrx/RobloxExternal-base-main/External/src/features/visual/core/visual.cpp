#include "visual.h"
#include "../../../utility/cache/cache.h"
#include "../../../menu/overlay/settings.h"
#include "../box/box.h"
#include "../skeleton/skeleton.h"
#include "../health/health.h"
#include "../name/name.h"
#include "../headdot/headdot.h"
#include "../tracer/tracer.h"
#include "../chams/chams.h"
#include "../whitehits/whitehits.h"
#include "../footsteps/footsteps.h"
#include "../kitesp/kitesp.h"
#include "../../../menu/resources/font.h"
#include <vector>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <string>
#include <cstdio>
#include <cstring>

static constexpr float kStaticBoxAspect = 0.72f;
static constexpr float kStaticBoxPadY   = 5.0f;

namespace rpbrx::visual
{
    static std::unordered_map<uintptr_t, LimbCache> g_limb;
    static std::mutex g_limbMtx;

    static inline void DrawVisualText(ImDrawList* dl, ImFont* font, float fontSize, float x, float y, ImU32 col, bool outline, ImU32 outlineCol, const char* text)
    {
        if (!dl || !text || !text[0]) return;
        if (!font) font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
        float ix = std::floor(x);
        float iy = std::floor(y);
        if (outline) {
            static const ImVec2 offsets[4] = {
                ImVec2(-1.0f, -1.0f), ImVec2(1.0f, -1.0f),
                ImVec2(-1.0f,  1.0f), ImVec2(1.0f,  1.0f)
            };
            for (int i = 0; i < 4; i++) {
                dl->AddText(font, fontSize, ImVec2(ix + offsets[i].x, iy + offsets[i].y), outlineCol, text);
            }
        }
        dl->AddText(font, fontSize, ImVec2(ix, iy), col, text);
    }

    static inline uintptr_t getPrim(node c, const std::string& n)
    {
        node limb = c.find_child(n);
        if (!limb) return 0;
        return limb.primitive();
    }

    static inline math::vec3 getPos(uintptr_t prim)
    {
        if (!prim) return {};
        return g_proc->read<math::vec3>(prim + Offsets::Primitive::Position);
    }

    static inline math::frame getFrame(uintptr_t prim)
    {
        if (!prim) return {};
        return g_proc->read<math::frame>(prim + Offsets::Primitive::Rotation);
    }

    static inline void addOrientedBox(std::vector<math::vec3>& out, const math::frame& cf, const math::vec3& root, float w, float h, float d)
    {
        if (cf.pos.x == 0 && cf.pos.y == 0 && cf.pos.z == 0) return;
        if ((cf.pos - root).length() > 25.0f) return;
        math::vec3 pos = cf.pos;
        math::vec3 r = cf.right();
        math::vec3 u = cf.up();
        math::vec3 l = cf.look();
        float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;
        for (int x = -1; x <= 1; x += 2)
            for (int y = -1; y <= 1; y += 2)
                for (int z = -1; z <= 1; z += 2)
                    out.push_back({ pos.x + r.x*(x*hw) + u.x*(y*hh) + l.x*(z*hd),
                                    pos.y + r.y*(x*hw) + u.y*(y*hh) + l.y*(z*hd),
                                    pos.z + r.z*(x*hw) + u.z*(y*hh) + l.z*(z*hd) });
    }

    static inline void collectBoxPoints(const WorldCache& wc, const LimbCache& lc, std::vector<math::vec3>& pts)
    {
        if (wc.root.x == 0 && wc.root.y == 0 && wc.root.z == 0) return;
        bool isR6 = wc.isR6;
        if (lc.headPrim) {
            auto hp = getPos(lc.headPrim);
            if (!(hp.x==0 && hp.y==0 && hp.z==0) && (hp - wc.root).length() <= 25.0f) {
                float hs = isR6 ? 1.0f : 0.8f;
                for (int i=0;i<8;++i){ float a=(i/8.0f)*6.2831853f; pts.push_back({hp.x+cosf(a)*hs, hp.y, hp.z+sinf(a)*hs}); }
                pts.push_back({hp.x, hp.y+hs, hp.z});
                pts.push_back({hp.x, hp.y - hs*0.5f, hp.z});
            }
        }
        if (isR6) {
            if (lc.torsoPrim) addOrientedBox(pts, getFrame(lc.torsoPrim), wc.root, 2.0f, 2.0f, 1.0f);
            if (lc.rightArmPrim) addOrientedBox(pts, getFrame(lc.rightArmPrim), wc.root, 1.0f, 2.0f, 1.0f);
            if (lc.leftArmPrim) addOrientedBox(pts, getFrame(lc.leftArmPrim), wc.root, 1.0f, 2.0f, 1.0f);
            if (lc.rightLegPrim) addOrientedBox(pts, getFrame(lc.rightLegPrim), wc.root, 1.0f, 2.0f, 1.0f);
            if (lc.leftLegPrim) addOrientedBox(pts, getFrame(lc.leftLegPrim), wc.root, 1.0f, 2.0f, 1.0f);
        } else {
            if (lc.upperPrim) addOrientedBox(pts, getFrame(lc.upperPrim), wc.root, 1.6f, 1.5f, 0.8f);
            else if (lc.torsoPrim) addOrientedBox(pts, getFrame(lc.torsoPrim), wc.root, 1.6f, 1.5f, 0.8f);
            if (lc.rightHandPrim) addOrientedBox(pts, getFrame(lc.rightHandPrim), wc.root, 0.8f, 0.8f, 0.8f);
            else if (lc.rightLowerArmPrim) addOrientedBox(pts, getFrame(lc.rightLowerArmPrim), wc.root, 0.8f, 0.8f, 0.8f);
            if (lc.leftHandPrim) addOrientedBox(pts, getFrame(lc.leftHandPrim), wc.root, 0.8f, 0.8f, 0.8f);
            else if (lc.leftLowerArmPrim) addOrientedBox(pts, getFrame(lc.leftLowerArmPrim), wc.root, 0.8f, 0.8f, 0.8f);
            if (lc.rightFootPrim) addOrientedBox(pts, getFrame(lc.rightFootPrim), wc.root, 0.8f, 0.7f, 1.2f);
            else if (lc.rightLowerLegPrim) addOrientedBox(pts, getFrame(lc.rightLowerLegPrim), wc.root, 0.8f, 0.7f, 1.2f);
            if (lc.leftFootPrim) addOrientedBox(pts, getFrame(lc.leftFootPrim), wc.root, 0.8f, 0.7f, 1.2f);
            else if (lc.leftLowerLegPrim) addOrientedBox(pts, getFrame(lc.leftLowerLegPrim), wc.root, 0.8f, 0.7f, 1.2f);
        }
    }

    static inline data::rect projectBoxPoints(const std::vector<math::vec3>& pts, const math::view_matrix& view, float screenW, float screenH)
    {
        if (pts.empty()) return {};
        float minX=1e9f, minY=1e9f, maxX=-1e9f, maxY=-1e9f;
        int valid=0;
        for (auto &p: pts) {
            math::vec2 s;
            if (!projection::to_screen(p, view, screenW, screenH, s)) continue;
            if (s.x==0 && s.y==0) continue;
            minX = (std::min)(minX, s.x);
            minY = (std::min)(minY, s.y);
            maxX = (std::max)(maxX, s.x);
            maxY = (std::max)(maxY, s.y);
            ++valid;
        }
        if (valid < 3 || minX==1e9f) return {};
        float bw = maxX - minX, bh = maxY - minY;
        if (bw > screenW*1.5f || bh > screenH*1.5f) return {};
        if (bw <= 2 || bh <= 4) return {};
        if ((bw > 10.0f && bh > bw * 6.0f) || (bh > 10.0f && bw > bh * 1.5f)) return {};
        data::rect rc{}; rc.x0=minX; rc.y0=minY; rc.x1=maxX; rc.y1=maxY;
        return rc;
    }

    // HSV to ImU32 helper (catsix formula)
    static inline ImU32 hsv_to_col(float h, float s, float v, float a = 1.0f)
    {
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
    }

    // Catsix health color: HSV where H = clamp(hp/maxHp, 0, 1) / 2.5, S = 0.89, V = 0.75
    static inline ImU32 health_color_catsix(float hp, float maxHp)
    {
        float frac = (maxHp > 0.01f) ? std::clamp(hp / maxHp, 0.0f, 1.0f) : 1.0f;
        return hsv_to_col(frac / 2.5f, 0.89f, 0.75f);
    }

    // Distance color for tracers: HSV(min(distance/128 / 2.8, 0.4), 0.89, 0.75)
    static inline ImU32 distance_color_catsix(float studs)
    {
        float h = std::min(studs / 128.0f / 2.8f, 0.4f);
        return hsv_to_col(h, 0.89f, 0.75f);
    }

    static std::vector<WorldCache> g_world;
    static std::chrono::steady_clock::time_point g_last;

    static math::view_matrix g_viewCache{};
    static bool g_viewValid = false;

    static std::unordered_map<uintptr_t, std::string> g_nameCache;
    static std::mutex g_nameMtx;

    static inline math::view_matrix fetchView()
    {
        uint64_t vis = g_proc->read<uint64_t>(g_proc->base() + Offsets::VisualEngine::Pointer);
        if (!vis) return g_ctx.view;
        return g_proc->read<math::view_matrix>(vis + Offsets::VisualEngine::ViewMatrix);
    }

    static void build_pf_world(std::vector<WorldCache>& out, float fw, float fh)
    {
        out.clear();
        if (PfCache::players.empty())
            return;
        bool needSkel = (settings::esp_mode == 2);
        bool need3d = (settings::esp_mode == 1);
        bool needLimbs = needSkel || need3d;

        for (auto& pp : PfCache::players)
        {
            if (!pp.isValid) continue;
            if (pp.modelAddr == PfCache::localCharacterAddr) continue;
            if (pp.health <= 0.0f) continue;
            if (settings::esp_team_check && PfCache::IsTeammate(pp)) continue;

            WorldCache wc{};
            wc.playerAddr = pp.modelAddr;

            node headNode{ pp.headAddr };
            node torsoNode{ pp.torsoAddr };
            uintptr_t headPrim = headNode ? headNode.primitive() : 0;
            uintptr_t torsoPrim = torsoNode ? torsoNode.primitive() : 0;
            if (!headPrim) continue;
            wc.head = getPos(headPrim);
            if (wc.head.x == 0 && wc.head.y == 0 && wc.head.z == 0) continue;
            wc.root = torsoPrim ? getPos(torsoPrim) : math::vec3{};
            if ((wc.root.x == 0 && wc.root.y == 0 && wc.root.z == 0) || (wc.head - wc.root).length() > 25.0f)
                wc.root = math::vec3{ wc.head.x, wc.head.y - 2.5f, wc.head.z };

            wc.hp = pp.health * 100.0f;
            wc.maxHp = 100.0f;
            wc.name = pp.name;
            wc.valid = true;
            wc.isR6 = true;

            LimbCache lc{};
            lc.characterAddr = pp.modelAddr;
            lc.isR6 = true;
            lc.headPrim = headPrim;
            lc.rootPrim = torsoPrim ? torsoPrim : headPrim;
            lc.torsoPrim = torsoPrim;

            auto safePos = [&](uintptr_t prim, float maxDist) -> math::vec3 {
                if (!prim) return {};
                math::vec3 p = getPos(prim);
                if (p.x == 0 && p.y == 0 && p.z == 0) return {};
                if ((p - wc.root).length() > maxDist) return {};
                return p;
            };

            if (needLimbs)
            {
                std::vector<uintptr_t> lprims;
                lprims.reserve(pp.limbAddrs.size());
                for (auto la : pp.limbAddrs) {
                    node ln{ la };
                    uintptr_t prim = ln ? ln.primitive() : 0;
                    if (prim) lprims.push_back(prim);
                }
                uintptr_t laArm = lprims.size() > 0 ? lprims[0] : 0;
                uintptr_t raArm = lprims.size() > 1 ? lprims[1] : 0;
                uintptr_t laLeg = lprims.size() > 2 ? lprims[2] : 0;
                uintptr_t raLeg = lprims.size() > 3 ? lprims[3] : 0;
                lc.leftArmPrim = laArm;
                lc.rightArmPrim = raArm;
                lc.leftLegPrim = laLeg;
                lc.rightLegPrim = raLeg;
                wc.torso = safePos(torsoPrim, 15.0f);
                wc.leftArm = safePos(laArm, 20.0f);
                wc.rightArm = safePos(raArm, 20.0f);
                wc.leftLeg = safePos(laLeg, 20.0f);
                wc.rightLeg = safePos(raLeg, 20.0f);
            }
            else
            {
                wc.torso = safePos(torsoPrim, 15.0f);
            }
            wc.lc = lc;
            if (need3d && lc.headPrim) {
                std::vector<math::vec3> pts;
                pts.reserve(200);
                collectBoxPoints(wc, lc, pts);
                if (!pts.empty()) { wc.pts3d = std::move(pts); wc.hasPts3d = true; }
            }
            out.push_back(std::move(wc));
        }
    }

    void on_game_joined()
    {
        std::lock_guard<std::mutex> lk(g_limbMtx);
        g_limb.clear();
        std::lock_guard<std::mutex> nlk(g_nameMtx);
        g_nameCache.clear();
        pcache::clear();
        whitehits::clear();
        footsteps::clear();
        kitesp::clear();
    }

    void render(ImDrawList* dl, int w, int h)
    {
        if (!dl) return;

        // Aim range circle
        if (settings::aim_range_circle && settings::aim_fov > 0.0f)
        {
            float cx = (float)w * 0.5f;
            float cy = (float)h * 0.5f;
            float r = settings::aim_fov;
            ImU32 mc = ImGui::ColorConvertFloat4ToU32(ImVec4(
                settings::aim_circle_color[0], settings::aim_circle_color[1],
                settings::aim_circle_color[2], settings::aim_circle_transparency));
            if (settings::aim_circle_filled) {
                dl->AddCircleFilled(ImVec2(cx, cy), r, mc, 64);
            }
            dl->AddCircle(ImVec2(cx, cy), r, mc, 64, settings::aim_circle_thickness);
        }

        if (!g_ctx.ready) return;
        if (!settings::esp_enabled && !settings::skeleton_enabled && !settings::health_enabled
            && !settings::name_enabled && !settings::headdot_enabled && !settings::tool_enabled
            && !settings::tracer_enabled && !settings::chams_enabled && !settings::whitehits_enabled
            && !settings::footsteps_enabled)
            return;

        float fw = (float)w;
        float fh = (float)h;

        auto now = std::chrono::steady_clock::now();
        bool need = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last).count() > 33;

        bool needSkel = (settings::esp_mode == 2) && settings::esp_enabled;
        bool need3dBox = (settings::esp_mode == 1) && settings::esp_enabled;
        bool needFullLimbs = needSkel || need3dBox || settings::skeleton_enabled;

        static bool s_pfMode = false;
        static std::chrono::steady_clock::time_point s_pfCheck{};
        if (now - s_pfCheck > std::chrono::milliseconds(2000)) {
            s_pfCheck = now;
            s_pfMode = PfCache::IsActivePlace();
        }

        if (g_proc && g_proc->connected()) {
            g_viewCache = fetchView();
            g_viewValid = true;
        }

        if (need)
        {
            uintptr_t localTeamAddr = 0;
            if (settings::esp_team_check && g_ctx.local_player.addr)
                localTeamAddr = g_proc->read<uintptr_t>(g_ctx.local_player.addr + Offsets::Player::Team);
            std::vector<WorldCache> next;
            if (s_pfMode)
            {
                build_pf_world(next, fw, fh);
            }
            else
            {
                pcache::update();
                std::vector<pcache::Entry> snapshot;
                {
                    std::lock_guard<std::mutex> lk(pcache::mtx);
                    snapshot = pcache::entries;
                }
                next.reserve(snapshot.size());

                for (auto& pe : snapshot)
                {
                    WorldCache wc{};
                    wc.playerAddr = pe.player;
                    if (pe.player == g_ctx.local_player.addr) { next.push_back(std::move(wc)); continue; }
                    if (settings::esp_team_check && localTeamAddr && pe.team && pe.team == localTeamAddr) { next.push_back(std::move(wc)); continue; }
                    if (pe.hp <= 0.f) { next.push_back(std::move(wc)); continue; }
                    wc.hp = pe.hp;
                    wc.maxHp = pe.maxHp > 0 ? pe.maxHp : 100.f;
                    wc.name = pe.name;
                    if (settings::tool_enabled)
                        wc.tool = pe.tool;

                    LimbCache lc = pe.lc;
                    if (lc.headPrim == 0) { next.push_back(std::move(wc)); continue; }

                    wc.head = getPos(lc.headPrim);
                    if (wc.head.x == 0 && wc.head.y == 0 && wc.head.z == 0) { next.push_back(wc); continue; }

                    wc.root = lc.rootPrim ? getPos(lc.rootPrim) : math::vec3{};
                    float drop = lc.isR6 ? 3.0f : 2.5f;
                    if ((wc.root.x == 0 && wc.root.y == 0 && wc.root.z == 0) || (wc.head - wc.root).length() > 25.0f)
                        wc.root = math::vec3{ wc.head.x, wc.head.y - drop, wc.head.z };

                    wc.valid = true;
                    wc.isR6 = lc.isR6;

                    auto safePos = [&](uintptr_t prim, float maxDist) -> math::vec3 {
                        if (!prim) return {};
                        math::vec3 p = getPos(prim);
                        if (p.x == 0 && p.y == 0 && p.z == 0) return {};
                        if ((p - wc.root).length() > maxDist) return {};
                        return p;
                    };

                    if (lc.isR6)
                    {
                        wc.torso = safePos(lc.torsoPrim, 15.0f);
                        wc.leftArm = safePos(lc.leftArmPrim, 20.0f);
                        wc.rightArm = safePos(lc.rightArmPrim, 20.0f);
                        wc.leftLeg = safePos(lc.leftLegPrim, 20.0f);
                        wc.rightLeg = safePos(lc.rightLegPrim, 20.0f);
                    }
                    else
                    {
                        wc.upper = safePos(lc.upperPrim, 15.0f);
                        wc.lower = safePos(lc.lowerPrim, 15.0f);
                        wc.hasLower = lc.lowerPrim != 0 && (wc.lower.x != 0 || wc.lower.y != 0 || wc.lower.z != 0);
                        wc.leftUpperArm = safePos(lc.leftUpperArmPrim, 20.0f);
                        wc.leftLowerArm = safePos(lc.leftLowerArmPrim, 20.0f);
                        wc.leftHand = safePos(lc.leftHandPrim, 25.0f);
                        wc.rightUpperArm = safePos(lc.rightUpperArmPrim, 20.0f);
                        wc.rightLowerArm = safePos(lc.rightLowerArmPrim, 20.0f);
                        wc.rightHand = safePos(lc.rightHandPrim, 25.0f);
                        wc.leftUpperLeg = safePos(lc.leftUpperLegPrim, 20.0f);
                        wc.leftLowerLeg = safePos(lc.leftLowerLegPrim, 20.0f);
                        wc.leftFoot = safePos(lc.leftFootPrim, 25.0f);
                        wc.rightUpperLeg = safePos(lc.rightUpperLegPrim, 20.0f);
                        wc.rightLowerLeg = safePos(lc.rightLowerLegPrim, 20.0f);
                        wc.rightFoot = safePos(lc.rightFootPrim, 25.0f);
                        wc.torso = wc.upper;
                    }

                    wc.lc = lc;
                    if (needSkel && lc.isR6 && lc.torsoPrim) {
                        auto tf = getFrame(lc.torsoPrim);
                        if (!(tf.pos.x == 0 && tf.pos.y == 0 && tf.pos.z == 0) && (tf.pos - wc.root).length() <= 6.0f) {
                            wc.torsoFrame = tf;
                            wc.hasTorsoFrame = true;
                        }
                    }
                    if (need3dBox && lc.headPrim) {
                        std::vector<math::vec3> pts;
                        pts.reserve(200);
                        collectBoxPoints(wc, lc, pts);
                        if (!pts.empty()) { wc.pts3d = std::move(pts); wc.hasPts3d = true; }
                    }
                    next.push_back(std::move(wc));
                }
            }

            g_world.swap(next);
            g_last = now;
        }

        if (!g_viewValid) return;
        const math::view_matrix& view = g_viewCache;

        ImU32 espCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::esp_color[0], settings::esp_color[1], settings::esp_color[2], settings::esp_color[3]));
        ImU32 skelCol = 0;
        if (settings::skeleton_enabled || (settings::esp_enabled && settings::esp_mode == 2)) {
            skelCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::skeleton_color[0], settings::skeleton_color[1], settings::skeleton_color[2], settings::skeleton_color[3]));
        }
        ImFont* verdana = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();

        for (auto& wc : g_world)
        {
            if (!wc.valid) continue;

            float studsDist = (wc.root - g_ctx.local_pos).length();
            if (studsDist < 0.1f) studsDist = (wc.head - g_ctx.local_pos).length();

            // ESP distance check
            if (settings::esp_distance_check) {
                if (studsDist < settings::esp_distance_min || studsDist > settings::esp_distance_max)
                    continue;
            }

            // Team check for tracers
            bool tracerTeamSkip = false;
            // (team check already filtered in cache building above for ESP)

            float distScale = std::clamp(18.0f / (studsDist + 8.0f) + 0.55f, 0.55f, 1.45f);

            // Build 2D bounding rect
            data::rect rc{};
            if (settings::esp_mode == 1 && wc.hasPts3d && !wc.pts3d.empty()) {
                rc = projectBoxPoints(wc.pts3d, view, fw, fh);
            }
            if (rc.empty()) {
                math::vec3 top3{ wc.head.x, wc.head.y + 0.5f, wc.head.z };
                float drop = wc.isR6 ? 3.0f : 2.5f;
                math::vec3 bot3{ wc.root.x, wc.root.y - drop, wc.root.z };
                math::vec2 top{}, bot{};
                bool a = projection::to_screen(top3, view, fw, fh, top);
                bool b = projection::to_screen(bot3, view, fw, fh, bot);
                if (!a && !b) continue;
                if (a && !b) { bot.x = top.x; bot.y = top.y + 4.0f; }
                else if (!a && b) { top.x = bot.x; top.y = bot.y - 4.0f; }
                float hh = fabsf(bot.y - top.y) + kStaticBoxPadY * 2.0f;
                if (hh < 4.0f) hh = 4.0f;
                float ww = (std::max)(hh * kStaticBoxAspect, 3.0f);
                float minY = (std::min)(top.y, bot.y) - kStaticBoxPadY;
                float midX = (top.x + bot.x) * 0.5f;
                rc.x0 = midX - ww * 0.5f;
                rc.x1 = midX + ww * 0.5f;
                rc.y0 = minY;
                rc.y1 = minY + hh;
            }

            rc.x0 = std::floor(rc.x0);
            rc.x1 = std::floor(rc.x1);
            rc.y0 = std::floor(rc.y0);
            rc.y1 = std::floor(rc.y1);

            if (rc.x1 < -50.0f || rc.x0 > fw + 50.0f || rc.y1 < -50.0f || rc.y0 > fh + 50.0f)
                continue;

            // --- ESP rendering based on mode ---
            if (settings::esp_enabled)
            {
                int mode = settings::esp_mode;

                if (mode == 0) // 2D mode
                {
                    // Filled background
                    if (settings::esp_filled) {
                        ImU32 fillCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                            settings::esp_fill_color[0], settings::esp_fill_color[1],
                            settings::esp_fill_color[2], settings::esp_fill_color[3]));
                        box::draw_fill(dl, rc.x0, rc.y0, rc.x1, rc.y1, fillCol);
                    }

                    // Bounding box
                    if (settings::esp_bounding_box) {
                        if (settings::esp_box_style == 1) {
                            box::draw_corner(dl, rc.x0, rc.y0, rc.x1, rc.y1, espCol, true, IM_COL32(0,0,0,255));
                        } else {
                            box::draw(dl, rc.x0, rc.y0, rc.x1, rc.y1, espCol, true, IM_COL32(0,0,0,255));
                        }
                    }

                    // Health bar (catsix HSV color)
                    if (settings::health_enabled) {
                        ImU32 hcol = health_color_catsix(wc.hp, wc.maxHp);
                        health::draw(dl, rc, wc.hp, wc.maxHp, true, IM_COL32(0,0,0,255), false, false, hcol, settings::esp_bounding_box, 1.0f);
                    }

                    // Nametag
                    if (settings::name_enabled && !wc.name.empty()) {
                        // Rendered inline below
                    }
                }
                else if (mode == 1) // 3D wireframe box
                {
                    // Draw 3D wireframe box: RootPosition +/- (1.5, HipHeight, 1.5)
                    float hipHeight = wc.isR6 ? 3.0f : 2.5f;
                    math::vec3 rp = wc.root;
                    // 8 corners of the 3D box
                    math::vec3 corners[8];
                    float hw = 1.5f, hh = hipHeight, hd = 1.5f;
                    int ci = 0;
                    for (int x = -1; x <= 1; x += 2)
                        for (int y = -1; y <= 1; y += 2)
                            for (int z = -1; z <= 1; z += 2)
                                corners[ci++] = { rp.x + x*hw, rp.y + y*hh, rp.z + z*hd };

                    // Project all 8 corners
                    math::vec2 sc[8];
                    bool vis[8];
                    for (int i = 0; i < 8; ++i)
                        vis[i] = projection::to_screen(corners[i], view, fw, fh, sc[i]);

                    // 12 edges of a box
                    static const int edges[12][2] = {
                        {0,1},{2,3},{4,5},{6,7},  // Y edges
                        {0,2},{1,3},{4,6},{5,7},  // Z edges
                        {0,4},{1,5},{2,6},{3,7},  // X edges
                    };
                    for (int i = 0; i < 12; ++i) {
                        int a = edges[i][0], b = edges[i][1];
                        if (vis[a] && vis[b])
                            dl->AddLine(ImVec2(sc[a].x, sc[a].y), ImVec2(sc[b].x, sc[b].y), espCol, 1.0f);
                    }
                }
                else if (mode == 2) // Skeleton mode
                {
                    const float thick = 2.0f;
                    if (wc.isR6) {
                        skeleton::pose_r6 pose{};
                        pose.head = wc.head;
                        pose.torso = wc.torso.x != 0 ? wc.torso : wc.root;
                        if (wc.leftArm.x != 0) { pose.arm_l = wc.leftArm; pose.has_arm_l = true; }
                        if (wc.rightArm.x != 0) { pose.arm_r = wc.rightArm; pose.has_arm_r = true; }
                        if (wc.leftLeg.x != 0) { pose.leg_l = wc.leftLeg; pose.has_leg_l = true; }
                        if (wc.rightLeg.x != 0) { pose.leg_r = wc.rightLeg; pose.has_leg_r = true; }
                        if (wc.hasTorsoFrame) { pose.torsoFrame = wc.torsoFrame; pose.has_torsoFrame = true; }
                        auto out = skeleton::build_r6(pose, view, fw, fh);
                        for (int i=0;i<out.count;++i)
                            dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), espCol, thick);
                    } else {
                        skeleton::pose_r15 pose{};
                        pose.head = wc.head;
                        pose.upper = wc.upper.x != 0 ? wc.upper : wc.root;
                        pose.lower = wc.lower;
                        pose.has_lower = wc.hasLower;
                        if (pose.lower.x == 0) pose.lower = pose.upper;
                        pose.l_upArm = wc.leftUpperArm; pose.r_upArm = wc.rightUpperArm;
                        pose.l_lowArm = wc.leftLowerArm; pose.r_lowArm = wc.rightLowerArm;
                        pose.hand_l = wc.leftHand; pose.hand_r = wc.rightHand;
                        pose.l_upLeg = wc.leftUpperLeg; pose.r_upLeg = wc.rightUpperLeg;
                        pose.l_lowLeg = wc.leftLowerLeg; pose.r_lowLeg = wc.rightLowerLeg;
                        pose.foot_l = wc.leftFoot; pose.foot_r = wc.rightFoot;
                        pose.elbow_l = pose.l_lowArm; pose.elbow_r = pose.r_lowArm;
                        pose.knee_l = pose.l_lowLeg; pose.knee_r = pose.r_lowLeg;
                        if (pose.hand_l.x==0 && pose.l_lowArm.x!=0) pose.hand_l = pose.l_lowArm;
                        else if (pose.hand_l.x==0 && pose.l_upArm.x!=0) pose.hand_l = pose.l_upArm;
                        if (pose.hand_r.x==0 && pose.r_lowArm.x!=0) pose.hand_r = pose.r_lowArm;
                        else if (pose.hand_r.x==0 && pose.r_upArm.x!=0) pose.hand_r = pose.r_upArm;
                        if (pose.foot_l.x==0 && pose.l_lowLeg.x!=0) pose.foot_l = pose.l_lowLeg;
                        else if (pose.foot_l.x==0 && pose.l_upLeg.x!=0) pose.foot_l = pose.l_upLeg;
                        if (pose.foot_r.x==0 && pose.r_lowLeg.x!=0) pose.foot_r = pose.r_lowLeg;
                        else if (pose.foot_r.x==0 && pose.r_upLeg.x!=0) pose.foot_r = pose.r_upLeg;
                        auto out = skeleton::build_r15(pose, view, fw, fh);
                        for (int i=0;i<out.count;++i)
                            dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), espCol, thick);
                    }
                }
            }

            // Standalone skeleton overlay (when esp_mode is not skeleton but skeleton_enabled is on)
            if (settings::skeleton_enabled && !(settings::esp_enabled && settings::esp_mode == 2))
            {
                const float baseThick = 1.5f * distScale;
                if (wc.isR6) {
                    skeleton::pose_r6 pose{};
                    pose.head = wc.head;
                    pose.torso = wc.torso.x != 0 ? wc.torso : wc.root;
                    if (wc.leftArm.x != 0) { pose.arm_l = wc.leftArm; pose.has_arm_l = true; }
                    if (wc.rightArm.x != 0) { pose.arm_r = wc.rightArm; pose.has_arm_r = true; }
                    if (wc.leftLeg.x != 0) { pose.leg_l = wc.leftLeg; pose.has_leg_l = true; }
                    if (wc.rightLeg.x != 0) { pose.leg_r = wc.rightLeg; pose.has_leg_r = true; }
                    if (wc.hasTorsoFrame) { pose.torsoFrame = wc.torsoFrame; pose.has_torsoFrame = true; }
                    auto out = skeleton::build_r6(pose, view, fw, fh);
                    for (int i=0;i<out.count;++i)
                        dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelCol, baseThick);
                } else {
                    skeleton::pose_r15 pose{};
                    pose.head = wc.head;
                    pose.upper = wc.upper.x != 0 ? wc.upper : wc.root;
                    pose.lower = wc.lower;
                    pose.has_lower = wc.hasLower;
                    if (pose.lower.x == 0) pose.lower = pose.upper;
                    pose.l_upArm = wc.leftUpperArm; pose.r_upArm = wc.rightUpperArm;
                    pose.l_lowArm = wc.leftLowerArm; pose.r_lowArm = wc.rightLowerArm;
                    pose.hand_l = wc.leftHand; pose.hand_r = wc.rightHand;
                    pose.l_upLeg = wc.leftUpperLeg; pose.r_upLeg = wc.rightUpperLeg;
                    pose.l_lowLeg = wc.leftLowerLeg; pose.r_lowLeg = wc.rightLowerLeg;
                    pose.foot_l = wc.leftFoot; pose.foot_r = wc.rightFoot;
                    pose.elbow_l = pose.l_lowArm; pose.elbow_r = pose.r_lowArm;
                    pose.knee_l = pose.l_lowLeg; pose.knee_r = pose.r_lowLeg;
                    if (pose.hand_l.x==0 && pose.l_lowArm.x!=0) pose.hand_l = pose.l_lowArm;
                    if (pose.hand_r.x==0 && pose.r_lowArm.x!=0) pose.hand_r = pose.r_lowArm;
                    if (pose.foot_l.x==0 && pose.l_lowLeg.x!=0) pose.foot_l = pose.l_lowLeg;
                    if (pose.foot_r.x==0 && pose.r_lowLeg.x!=0) pose.foot_r = pose.r_lowLeg;
                    auto out = skeleton::build_r15(pose, view, fw, fh);
                    for (int i=0;i<out.count;++i)
                        dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelCol, baseThick);
                }
            }

            if (settings::chams_enabled)
                chams::draw(dl, rc.x0, rc.y0, rc.x1, rc.y1);

            if (settings::whitehits_enabled) {
                static std::unordered_map<uintptr_t, float> s_prevHp;
                auto hit = s_prevHp.find(wc.playerAddr);
                if (hit != s_prevHp.end() && wc.hp < hit->second)
                    whitehits::on_health_change(wc.playerAddr, hit->second, wc.hp);
                s_prevHp[wc.playerAddr] = wc.hp;
                whitehits::draw(dl, wc.playerAddr, rc.x0, rc.y0, rc.x1, rc.y1);
            }

            if (settings::footsteps_enabled) {
                math::vec3 footPos = wc.root;
                footPos.y -= (wc.isR6 ? 3.0f : 2.5f);
                footsteps::update(wc.playerAddr, footPos);
            }

            // Health bar (standalone, outside of 2D ESP mode)
            if (settings::health_enabled && !(settings::esp_enabled && settings::esp_mode == 0)) {
                ImU32 hcol = health_color_catsix(wc.hp, wc.maxHp);
                health::draw(dl, rc, wc.hp, wc.maxHp, true, IM_COL32(0,0,0,255), false, false, hcol, false, 1.0f);
            }

            headdot::draw(dl, wc.head, view, fw, fh, distScale);

            // --- Nametag rendering (catsix style) ---
            if (settings::name_enabled && !wc.name.empty())
            {
                // Distance check for nametags
                bool nameInRange = true;
                if (settings::name_distance_check) {
                    if (studsDist < settings::name_distance_min || studsDist > settings::name_distance_max)
                        nameInRange = false;
                }
                if (settings::name_team_check) {
                    // Already filtered at cache level
                }
                if (nameInRange)
                {
                    ImFont* font = verdana;
                    float baseFontSize = 13.0f * settings::name_scale;
                    float lineY = rc.y0 - 3.0f;

                    // Tool name above nametag
                    if (settings::name_show_tool && settings::tool_enabled && !wc.tool.empty()) {
                        ImU32 toolCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                            settings::tool_color[0], settings::tool_color[1],
                            settings::tool_color[2], settings::tool_color[3]));
                        ImVec2 toolTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, wc.tool.c_str());
                        float toolX = rc.x0 + (rc.x1 - rc.x0) * 0.5f - toolTs.x * 0.5f;
                        lineY -= toolTs.y + 2.0f;

                        if (settings::name_show_background) {
                            float pad = 2.0f;
                            ImU32 bgCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, settings::name_bg_transparency));
                            dl->AddRectFilled(ImVec2(toolX - pad, lineY - pad), ImVec2(toolX + toolTs.x + pad, lineY + toolTs.y + pad), bgCol);
                        }
                        DrawVisualText(dl, font, baseFontSize, toolX, lineY, toolCol, true, IM_COL32(0,0,0,200), wc.tool.c_str());
                        lineY += toolTs.y + 2.0f;
                    }

                    // Build nametag string pieces
                    // Format: [DIST] Name HP
                    ImU32 nameCol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                        settings::name_color[0], settings::name_color[1],
                        settings::name_color[2], settings::name_color[3]));

                    // Calculate full text width for centering
                    std::string fullText;
                    char distBuf[32] = {};
                    char hpBuf[16] = {};
                    if (settings::name_show_distance) {
                        snprintf(distBuf, sizeof(distBuf), "[%.0f] ", studsDist);
                        fullText += distBuf;
                    }
                    fullText += wc.name;
                    if (settings::name_show_health) {
                        snprintf(hpBuf, sizeof(hpBuf), " %.0f", wc.hp);
                        fullText += hpBuf;
                    }

                    ImVec2 fullTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, fullText.c_str());
                    float startX = rc.x0 + (rc.x1 - rc.x0) * 0.5f - fullTs.x * 0.5f;
                    lineY -= fullTs.y;

                    // Background behind text
                    if (settings::name_show_background) {
                        float pad = 2.0f;
                        ImU32 bgCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, settings::name_bg_transparency));
                        dl->AddRectFilled(ImVec2(startX - pad, lineY - pad), ImVec2(startX + fullTs.x + pad, lineY + fullTs.y + pad), bgCol);
                    }

                    float curX = startX;

                    // Distance prefix: green "[" + white dist + green "]"
                    if (settings::name_show_distance) {
                        ImU32 greenCol = IM_COL32(0, 255, 0, 255);
                        ImU32 whiteCol = IM_COL32(255, 255, 255, 255);

                        // "["
                        dl->AddText(font, baseFontSize, ImVec2(curX, lineY), greenCol, "[");
                        ImVec2 bracketTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, "[");
                        curX += bracketTs.x;

                        // distance number
                        char numBuf[16];
                        snprintf(numBuf, sizeof(numBuf), "%.0f", studsDist);
                        dl->AddText(font, baseFontSize, ImVec2(curX, lineY), whiteCol, numBuf);
                        ImVec2 numTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, numBuf);
                        curX += numTs.x;

                        // "] "
                        dl->AddText(font, baseFontSize, ImVec2(curX, lineY), greenCol, "] ");
                        ImVec2 closeTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, "] ");
                        curX += closeTs.x;
                    }

                    // Name
                    dl->AddText(font, baseFontSize, ImVec2(curX, lineY), nameCol, wc.name.c_str());
                    ImVec2 nameTs = font->CalcTextSizeA(baseFontSize, FLT_MAX, 0.0f, wc.name.c_str());
                    curX += nameTs.x;

                    // Health suffix (colored with catsix HSV)
                    if (settings::name_show_health) {
                        ImU32 hpCol = health_color_catsix(wc.hp, wc.maxHp);
                        char hpStr[16];
                        snprintf(hpStr, sizeof(hpStr), " %.0f", wc.hp);
                        dl->AddText(font, baseFontSize, ImVec2(curX, lineY), hpCol, hpStr);
                    }
                }
            }

            // --- Tracers ---
            if (settings::tracer_enabled)
            {
                // Tracer team check
                if (!settings::tracer_team_check || true) // team already filtered at cache level
                {
                    // Tracer distance check
                    bool tracerInRange = true;
                    if (settings::tracer_distance_check) {
                        if (studsDist < settings::tracer_distance_min || studsDist > settings::tracer_distance_max)
                            tracerInRange = false;
                    }
                    if (tracerInRange)
                    {
                        // Determine end position
                        math::vec3 endWorld = (settings::tracer_end == 0) ? wc.head : wc.root;
                        math::vec2 endScreen;
                        bool endVis = projection::to_screen(endWorld, view, fw, fh, endScreen);

                        // Behind-screen calculation
                        if (!endVis && settings::tracer_behind) {
                            math::vec4 clip = view.transform(endWorld);
                            float cx = fw * 0.5f, cy = fh * 0.5f;
                            if (clip.w < 0.1f) { clip.x = -clip.x; clip.y = -clip.y; }
                            float inv = 1.0f / (fabsf(clip.w) < 0.001f ? 0.001f : clip.w);
                            float nx = clip.x * inv, ny = clip.y * inv;
                            float sx = (fw * 0.5f * nx) + cx, sy = -(fh * 0.5f * ny) + cy;
                            float dx = sx - cx, dy = sy - cy;
                            if (clip.w < 0.1f) { dx = -dx; dy = -dy; }
                            float len = sqrtf(dx*dx + dy*dy);
                            if (len > 1.0f) {
                                dx /= len; dy /= len;
                                endScreen.x = std::clamp(cx + dx * (fw * 0.48f), 0.0f, fw);
                                endScreen.y = std::clamp(cy + dy * (fh * 0.48f), 0.0f, fh);
                                endVis = true;
                            }
                        }

                        if (endVis) {
                            // Determine start position
                            float startX, startY;
                            if (settings::tracer_start == 0) { // Middle
                                startX = fw * 0.5f;
                                startY = fh * 0.5f;
                            } else if (settings::tracer_start == 1) { // Bottom
                                startX = fw * 0.5f;
                                startY = fh;
                            } else { // Mouse
                                ImVec2 mp = ImGui::GetMousePos();
                                startX = mp.x;
                                startY = mp.y;
                            }

                            ImU32 tcol;
                            if (settings::tracer_color_by_distance) {
                                tcol = distance_color_catsix(studsDist);
                            } else {
                                tcol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                                    settings::tracer_color[0], settings::tracer_color[1],
                                    settings::tracer_color[2], 1.0f - settings::tracer_transparency));
                            }

                            dl->AddLine(ImVec2(startX, startY), ImVec2(endScreen.x, endScreen.y), tcol, 1.0f);
                        }
                    }
                }
            }

            // Tool display (standalone, below box)
            if (settings::tool_enabled && !settings::name_show_tool && !wc.tool.empty())
            {
                ImU32 tcol = ImGui::ColorConvertFloat4ToU32(ImVec4(
                    settings::tool_color[0], settings::tool_color[1],
                    settings::tool_color[2], settings::tool_color[3]));
                float fontSize = 13.0f;
                ImVec2 ts = verdana->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, wc.tool.c_str());
                float tx = rc.x0 + (rc.x1 - rc.x0) * 0.5f - ts.x * 0.5f;
                float ty = rc.y1 + 2.0f;
                DrawVisualText(dl, verdana, fontSize, tx, ty, tcol, true, IM_COL32(0,0,0,200), wc.tool.c_str());
            }
        }

        if (settings::footsteps_enabled)
            footsteps::render(dl, view, fw, fh);

        kitesp::tick(dl, w, h);
    }
}
