#include "visual.h"
#include "../../aim/core/aim.h"
#include "../../../utility/cache/cache.h"
#include "../world/ore.h"
#include "../world/plants.h"
#include "../world/bots.h"
#include "../world/animals.h"
#include "../world/tools.h"
#include "../world/crates.h"
#include "../world/keycards.h"
#include "../../../menu/overlay/settings.h"
#include "../box/box.h"
#include "../skeleton/skeleton.h"
#include "../health/health.h"
#include "../name/name.h"
#include "../headdot/headdot.h"
#include "../tracer/tracer.h"
#include "../oof/oof.h"
#include "../chams/chams.h"
#include "../whitehits/whitehits.h"
#include "../footsteps/footsteps.h"
#include "../targethud/targethud.h"
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
            if (lc.rightLegPrim) { auto cf=getFrame(lc.rightLegPrim);
                if(!(cf.pos.x==0&&cf.pos.y==0&&cf.pos.z==0) && (cf.pos - wc.root).length() <= 25.0f){
                    math::vec3 p=cf.pos; math::vec3 r=cf.right(),u=cf.up(),l=cf.look();
                    float w=1.0f,h=2.0f,d=1.0f;
                    for(int x=-1;x<=1;x+=2) for(int y=-1;y<=1;y+=2) for(int z=-1;z<=1;z+=2)
                        pts.push_back({p.x+r.x*(x*w*0.5f)+u.x*(y*h*0.6f)+l.x*(z*d*0.5f), p.y+r.y*(x*w*0.5f)+u.y*(y*h*0.6f)+l.y*(z*d*0.5f), p.z+r.z*(x*w*0.5f)+u.z*(y*h*0.6f)+l.z*(z*d*0.5f)});
                }
            }
            if (lc.leftLegPrim) { auto cf=getFrame(lc.leftLegPrim);
                if(!(cf.pos.x==0&&cf.pos.y==0&&cf.pos.z==0) && (cf.pos - wc.root).length() <= 25.0f){
                    math::vec3 p=cf.pos; math::vec3 r=cf.right(),u=cf.up(),l=cf.look();
                    float w=1.0f,h=2.0f,d=1.0f;
                    for(int x=-1;x<=1;x+=2) for(int y=-1;y<=1;y+=2) for(int z=-1;z<=1;z+=2)
                        pts.push_back({p.x+r.x*(x*w*0.5f)+u.x*(y*h*0.6f)+l.x*(z*d*0.5f), p.y+r.y*(x*w*0.5f)+u.y*(y*h*0.6f)+l.y*(z*d*0.5f), p.z+r.z*(x*w*0.5f)+u.z*(y*h*0.6f)+l.z*(z*d*0.5f)});
                }
            }
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
        bool needSkel = settings::skeleton_enabled;
        bool need3d = (settings::box_mode == 1);
        bool needLimbs = needSkel || need3d;

        static std::unordered_map<uintptr_t, std::string> s_teamNames;
        for (auto& pp : PfCache::players)
        {
            if (!pp.isValid)
                continue;
            if (pp.modelAddr == PfCache::localCharacterAddr)
                continue;
            if (pp.health <= 0.0f)
                continue;
            if (settings::vis_team_check && PfCache::IsTeammate(pp))
                continue;
            WorldCache wc{};
            wc.playerAddr = pp.modelAddr;

            node headNode{ pp.headAddr };
            node torsoNode{ pp.torsoAddr };
            uintptr_t headPrim = headNode ? headNode.primitive() : 0;
            uintptr_t torsoPrim = torsoNode ? torsoNode.primitive() : 0;
            if (!headPrim)
                continue;
            wc.head = getPos(headPrim);
            if (wc.head.x == 0 && wc.head.y == 0 && wc.head.z == 0)
                continue;
            wc.root = torsoPrim ? getPos(torsoPrim) : math::vec3{};
            if ((wc.root.x == 0 && wc.root.y == 0 && wc.root.z == 0) || (wc.head - wc.root).length() > 25.0f)
                wc.root = math::vec3{ wc.head.x, wc.head.y - 2.5f, wc.head.z };

            wc.hp = pp.health * 100.0f;
            wc.maxHp = 100.0f;
            wc.name = pp.name;
            if (settings::flags_enabled && settings::flags_team && pp.teamFolderAddr)
            {
                std::lock_guard<std::mutex> lk(g_nameMtx);
                auto tit = s_teamNames.find(pp.teamFolderAddr);
                if (tit != s_teamNames.end())
                    wc.team = tit->second;
                else
                {
                    wc.team = node{ pp.teamFolderAddr }.name();
                    s_teamNames[pp.teamFolderAddr] = wc.team;
                }
            }

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
                for (auto la : pp.limbAddrs)
                {
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
            if (need3d && lc.headPrim)
            {
                std::vector<math::vec3> pts;
                pts.reserve(200);
                collectBoxPoints(wc, lc, pts);
                if (!pts.empty())
                {
                    wc.pts3d = std::move(pts);
                    wc.hasPts3d = true;
                }
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
        ore::clear();
        plants::clear();
        bots::clear();
        animals::clear();
        tools::clear();
        crates::clear();
        keycards::clear();
        whitehits::clear();
        footsteps::clear();
        kitesp::clear();
        targethud::clear_target();
    }

    void render(ImDrawList* dl, int w, int h)
    {
        if (!dl) return;

        if (settings::aim_show_fov && settings::aim_fov > 0.0f)
        {
            float cx = (float)w * 0.5f;
            float cy = (float)h * 0.5f;
            float r = settings::aim_fov;

            if (settings::aim_fov_fill) {
                ImU32 fc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::aim_fov_fill_color[0], settings::aim_fov_fill_color[1], settings::aim_fov_fill_color[2], settings::aim_fov_fill_color[3]));
                dl->AddCircleFilled(ImVec2(cx, cy), r, fc, 64);
            }
            ImU32 mc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::aim_fov_color[0], settings::aim_fov_color[1], settings::aim_fov_color[2], settings::aim_fov_color[3]));
            if (settings::aim_fov_style == 1)
            {

                int n = (int)(r * 0.9f);
                if (n < 12) n = 12;
                if (n > 64) n = 64;
                ImU32 oc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::aim_fov_outline_color[0], settings::aim_fov_outline_color[1], settings::aim_fov_outline_color[2], settings::aim_fov_outline_color[3]));
                for (int i = 0; i < n; ++i)
                {
                    float a = (float)i * 6.2831853f / (float)n;
                    ImVec2 p{ cx + cosf(a) * r, cy + sinf(a) * r };
                    if (settings::aim_fov_outline)
                        dl->AddCircleFilled(p, 2.5f, oc);
                    dl->AddCircleFilled(p, 1.5f, mc);
                }
            }
            else
            {
                if (settings::aim_fov_outline) {
                    ImU32 oc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::aim_fov_outline_color[0], settings::aim_fov_outline_color[1], settings::aim_fov_outline_color[2], settings::aim_fov_outline_color[3]));
                    dl->AddCircle(ImVec2(cx, cy), r - 1.0f, oc, 64, 1.0f);
                    dl->AddCircle(ImVec2(cx, cy), r + 1.0f, oc, 64, 1.0f);
                }
                dl->AddCircle(ImVec2(cx, cy), r, mc, 64, 1.0f);
            }
        }

        if (!g_ctx.ready) return;
        bool any_flags = settings::flags_enabled && (settings::flags_team || settings::flags_state || settings::flags_health);
        if (!settings::box_enabled && !settings::skeleton_enabled && !settings::health_enabled && !settings::name_enabled && !settings::headdot_enabled && !settings::distance_enabled && !settings::tool_enabled && !any_flags && !settings::tracer_enabled && !settings::oof_enabled && !settings::aim_tracer && !settings::chams_enabled && !settings::whitehits_enabled && !settings::footsteps_enabled && !settings::targethud_enabled) return;

        float fw = (float)w;
        float fh = (float)h;

        auto now = std::chrono::steady_clock::now();
        bool need = std::chrono::duration_cast<std::chrono::milliseconds>(now - g_last).count() > 33;

        bool needSkel = settings::skeleton_enabled;
        bool need3dBox = (settings::box_mode == 1);
        bool needFullLimbs = needSkel || need3dBox;

        static bool s_pfMode = false;
        static std::chrono::steady_clock::time_point s_pfCheck{};
        if (now - s_pfCheck > std::chrono::milliseconds(2000))
        {
            s_pfCheck = now;
            s_pfMode = PfCache::IsActivePlace();
        }

        if (g_proc && g_proc->connected())
        {
            g_viewCache = fetchView();
            g_viewValid = true;
        }

        if (need)
        {
            uintptr_t localTeamAddr = 0;
            if (settings::vis_team_check && g_ctx.local_player.addr)
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

                if (settings::vis_team_check && localTeamAddr && pe.team && pe.team == localTeamAddr) { next.push_back(std::move(wc)); continue; }

                if (pe.hp <= 0.f) { next.push_back(std::move(wc)); continue; }
                wc.hp = pe.hp;
                wc.maxHp = pe.maxHp > 0 ? pe.maxHp : 100.f;

                wc.name = pe.name;
                if (settings::tool_enabled)
                    wc.tool = pe.tool;
                if (settings::flags_enabled && settings::flags_team)
                    wc.team = pe.teamStr;

                LimbCache lc = pe.lc;

                if (lc.headPrim == 0) { next.push_back(std::move(wc)); continue; }

                wc.head = getPos(lc.headPrim);
                if (wc.head.x == 0 && wc.head.y == 0 && wc.head.z == 0) { next.push_back(wc); continue; }

                wc.root = lc.rootPrim ? getPos(lc.rootPrim) : math::vec3{};
                float drop = lc.isR6 ? 3.0f : 2.5f;
                if ((wc.root.x == 0 && wc.root.y == 0 && wc.root.z == 0) || (wc.head - wc.root).length() > 25.0f)
                {
                    wc.root = math::vec3{ wc.head.x, wc.head.y - drop, wc.head.z };
                }

                if (settings::flags_enabled && settings::flags_state && pe.humanoid)
                {
                    uintptr_t seat = g_proc->read<uintptr_t>(pe.humanoid + Offsets::Humanoid::SeatPart);
                    uint8_t plat = g_proc->read<uint8_t>(pe.humanoid + Offsets::Humanoid::PlatformStand);
                    if (seat != 0)
                        wc.state = "Seated";
                    else if (plat != 0)
                        wc.state = "Platform";
                    else
                    {
                        math::vec3 vel{};
                        if (lc.rootPrim)
                            vel = g_proc->read<math::vec3>(lc.rootPrim + Offsets::Primitive::AssemblyLinearVelocity);
                        float hSpeed = sqrtf(vel.x * vel.x + vel.z * vel.z);
                        if (fabsf(vel.y) > 8.0f)
                            wc.state = vel.y > 0.0f ? "Jumping" : "Falling";
                        else if (hSpeed > 4.0f)
                            wc.state = "Moving";
                        else
                            wc.state = "Standing";
                    }
                }

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
                if (needSkel && lc.isR6 && lc.torsoPrim)
                {
                    auto tf = getFrame(lc.torsoPrim);
                    if (!(tf.pos.x == 0 && tf.pos.y == 0 && tf.pos.z == 0) && (tf.pos - wc.root).length() <= 6.0f)
                    {
                        wc.torsoFrame = tf;
                        wc.hasTorsoFrame = true;
                    }
                }
                if (need3dBox && lc.headPrim)
                {
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

        ImU32 boxCol = 0, boxOutlineCol = 0, boxFill1 = 0, boxFill2 = 0;
        if (settings::box_enabled)
        {
            boxCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::box_color[0], settings::box_color[1], settings::box_color[2], 1.0f));
            boxOutlineCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::box_outline_color[0], settings::box_outline_color[1], settings::box_outline_color[2], 1.0f));
            if (settings::box_filled) {
                boxFill1 = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::box_fill_color[0], settings::box_fill_color[1], settings::box_fill_color[2], settings::box_fill_color[3]));
                if (settings::box_fill_gradient)
                    boxFill2 = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::box_fill_color2[0], settings::box_fill_color2[1], settings::box_fill_color2[2], settings::box_fill_color2[3]));
            }
        }
        ImU32 skelCol = 0, skelOutlineCol = 0;
        if (settings::skeleton_enabled)
        {
            skelCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::skeleton_color[0], settings::skeleton_color[1], settings::skeleton_color[2], 1.0f));
            skelOutlineCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::skeleton_outline_color[0], settings::skeleton_outline_color[1], settings::skeleton_outline_color[2], 1.0f));
        }
        ImFont* verdana = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();

        if (settings::targethud_enabled) {
            targethud::clear_target();
        }

        for (auto& wc : g_world)
        {
            if (!wc.valid) continue;

            data::rect rc{};
            if (rpbrx::settings::box_mode == 1 && wc.hasPts3d && !wc.pts3d.empty()) {
                rc = projectBoxPoints(wc.pts3d, view, fw, fh);
            }
            if (rc.empty()) {
                math::vec3 top3{ wc.head.x, wc.head.y + 0.5f, wc.head.z };
                float drop = wc.isR6 ? 3.0f : 2.5f;
                math::vec3 bot3{ wc.root.x, wc.root.y - drop, wc.root.z };
                math::vec2 top{};
                math::vec2 bot{};
                bool a = projection::to_screen(top3, view, fw, fh, top);
                bool b = projection::to_screen(bot3, view, fw, fh, bot);
                if (!a && !b) continue;
                if (a && !b) {
                    bot.x = top.x;
                    bot.y = top.y + 4.0f;
                } else if (!a && b) {
                    top.x = bot.x;
                    top.y = bot.y - 4.0f;
                }
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

            float studsDist = (wc.root - g_ctx.local_pos).length();
            if (studsDist < 0.1f) studsDist = (wc.head - g_ctx.local_pos).length();
            float metersDist = studsDist * 0.28f;

            float distScale = std::clamp(18.0f / (studsDist + 8.0f) + 0.55f, 0.55f, 1.45f);

            if (settings::box_enabled)
            {
                if (settings::box_filled) {
                    if (settings::box_fill_gradient) {
                        box::draw_fill_gradient(dl, rc.x0, rc.y0, rc.x1, rc.y1, boxFill1, boxFill2);
                    } else {
                        box::draw_fill(dl, rc.x0, rc.y0, rc.x1, rc.y1, boxFill1);
                    }
                }
                if (settings::box_style == 1) {
                    box::draw_corner(dl, rc.x0, rc.y0, rc.x1, rc.y1, boxCol, settings::box_outline, boxOutlineCol);
                } else {
                    box::draw(dl, rc.x0, rc.y0, rc.x1, rc.y1, boxCol, settings::box_outline, boxOutlineCol);
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

            if (settings::health_enabled) {
                ImU32 hoc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::health_outline_color[0], settings::health_outline_color[1], settings::health_outline_color[2], 1.0f));
                ImU32 hcol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::health_color[0], settings::health_color[1], settings::health_color[2], 1.0f));
                health::draw(dl, rc, wc.hp, wc.maxHp, settings::health_outline, hoc, settings::health_gradient, settings::health_text, hcol, settings::box_enabled && settings::box_outline, 1.0f);
            }

            if (settings::name_enabled && !wc.name.empty())
            {
                ImU32 nameCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::name_color[0], settings::name_color[1], settings::name_color[2], 1.0f));
                ImU32 nameOutlineCol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::name_outline_color[0], settings::name_outline_color[1], settings::name_outline_color[2], 1.0f));
                float fontSize = 13.0f;
                ImVec2 ts = verdana->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, wc.name.c_str());
                float tx = rc.x0 + (rc.x1 - rc.x0) * 0.5f - ts.x * 0.5f;
                float ty = rc.y0 - ts.y - 3.0f;
                DrawVisualText(dl, verdana, fontSize, tx, ty, nameCol, settings::name_outline, nameOutlineCol, wc.name.c_str());
            }

            if (settings::skeleton_enabled)
            {
                const float baseThick = 1.5f * distScale;
                const float outlineThick = baseThick + 1.0f * distScale;
                if (wc.isR6)
                {
                    skeleton::pose_r6 pose{};
                    pose.head = wc.head;
                    pose.torso = wc.torso.x != 0 ? wc.torso : wc.root;
                    if (wc.leftArm.x != 0) { pose.arm_l = wc.leftArm; pose.has_arm_l = true; }
                    if (wc.rightArm.x != 0) { pose.arm_r = wc.rightArm; pose.has_arm_r = true; }
                    if (wc.leftLeg.x != 0) { pose.leg_l = wc.leftLeg; pose.has_leg_l = true; }
                    if (wc.rightLeg.x != 0) { pose.leg_r = wc.rightLeg; pose.has_leg_r = true; }
                    if (wc.hasTorsoFrame) { pose.torsoFrame = wc.torsoFrame; pose.has_torsoFrame = true; }
                    auto out = skeleton::build_r6(pose, view, fw, fh);
                    if (settings::skeleton_outline) for (int i=0;i<out.count;++i) dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelOutlineCol, outlineThick);
                    for (int i=0;i<out.count;++i) dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelCol, baseThick);
                }
                else
                {
                    skeleton::pose_r15 pose{};
                    pose.head = wc.head;
                    pose.upper = wc.upper.x != 0 ? wc.upper : wc.root;
                    pose.lower = wc.lower;
                    pose.has_lower = wc.hasLower;
                    if (pose.lower.x == 0) pose.lower = pose.upper;
                    pose.l_upArm = wc.leftUpperArm;
                    pose.r_upArm = wc.rightUpperArm;
                    pose.l_lowArm = wc.leftLowerArm;
                    pose.r_lowArm = wc.rightLowerArm;
                    pose.hand_l = wc.leftHand;
                    pose.hand_r = wc.rightHand;
                    pose.l_upLeg = wc.leftUpperLeg;
                    pose.r_upLeg = wc.rightUpperLeg;
                    pose.l_lowLeg = wc.leftLowerLeg;
                    pose.r_lowLeg = wc.rightLowerLeg;
                    pose.foot_l = wc.leftFoot;
                    pose.foot_r = wc.rightFoot;
                    pose.elbow_l = pose.l_lowArm;
                    pose.elbow_r = pose.r_lowArm;
                    pose.knee_l = pose.l_lowLeg;
                    pose.knee_r = pose.r_lowLeg;
                    if (pose.hand_l.x==0 && pose.l_lowArm.x!=0) pose.hand_l = pose.l_lowArm;
                    else if (pose.hand_l.x==0 && pose.l_upArm.x!=0) pose.hand_l = pose.l_upArm;
                    if (pose.hand_r.x==0 && pose.r_lowArm.x!=0) pose.hand_r = pose.r_lowArm;
                    else if (pose.hand_r.x==0 && pose.r_upArm.x!=0) pose.hand_r = pose.r_upArm;
                    if (pose.foot_l.x==0 && pose.l_lowLeg.x!=0) pose.foot_l = pose.l_lowLeg;
                    else if (pose.foot_l.x==0 && pose.l_upLeg.x!=0) pose.foot_l = pose.l_upLeg;
                    if (pose.foot_r.x==0 && pose.r_lowLeg.x!=0) pose.foot_r = pose.r_lowLeg;
                    else if (pose.foot_r.x==0 && pose.r_upLeg.x!=0) pose.foot_r = pose.r_upLeg;
                    auto out = skeleton::build_r15(pose, view, fw, fh);
                    if (settings::skeleton_outline) for (int i=0;i<out.count;++i) dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelOutlineCol, outlineThick);
                    for (int i=0;i<out.count;++i) dl->AddLine(ImVec2(out.items[i].a.x,out.items[i].a.y), ImVec2(out.items[i].b.x,out.items[i].b.y), skelCol, baseThick);
                }
            }

            headdot::draw(dl, wc.head, view, fw, fh, distScale);

            float distY = rc.y1 + 2.0f;
            ImVec2 distTs{};
            if (settings::distance_enabled)
            {
                char buf[32];
                if (metersDist < 10.0f) snprintf(buf, sizeof(buf), "%.1fm", metersDist);
                else snprintf(buf, sizeof(buf), "%.0fm", metersDist);
                ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
                float fontSize = 13.0f;
                distTs = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, buf);
                float tx = rc.x0 + (rc.x1 - rc.x0) * 0.5f - distTs.x * 0.5f;
                float ty = distY;
                ImU32 dcol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::distance_color[0], settings::distance_color[1], settings::distance_color[2], 1.0f));
                ImU32 doc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::distance_outline_color[0], settings::distance_outline_color[1], settings::distance_outline_color[2], 1.0f));
                DrawVisualText(dl, font, fontSize, tx, ty, dcol, settings::distance_outline, doc, buf);
                distY += distTs.y + 2.0f;
            }
            if (settings::tool_enabled)
            {
                bool inRange = true;
                if (settings::tool_distance_check && studsDist > settings::tool_distance_value)
                    inRange = false;
                if (inRange)
                {
                    const char* toolTxt = wc.tool.empty() ? "None" : wc.tool.c_str();
                    ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
                    float fontSize = 13.0f;
                    ImVec2 ts = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, toolTxt);
                    float tx = rc.x0 + (rc.x1 - rc.x0) * 0.5f - ts.x * 0.5f;
                    float ty = distY;
                    ImU32 tcol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::tool_color[0], settings::tool_color[1], settings::tool_color[2], 1.0f));
                    ImU32 tocol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::tool_outline_color[0], settings::tool_outline_color[1], settings::tool_outline_color[2], 1.0f));
                    DrawVisualText(dl, font, fontSize, tx, ty, tcol, settings::tool_outline, tocol, toolTxt);
                }
            }
            if (settings::flags_enabled && (settings::flags_team || settings::flags_state || settings::flags_health))
            {
                ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
                float fontSize = 13.0f;
                float fx = rc.x1 + 4.0f;
                float fy = rc.y0;
                if (settings::flags_team && !wc.team.empty())
                {
                    ImU32 fcol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_color[0], settings::flags_color[1], settings::flags_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_team_color[0], settings::flags_team_color[1], settings::flags_team_color[2], 1.0f));
                    ImU32 focol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_outline_color[0], settings::flags_outline_color[1], settings::flags_outline_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_team_outline_color[0], settings::flags_team_outline_color[1], settings::flags_team_outline_color[2], 1.0f));
                    bool use_outline = settings::flags_global ? settings::flags_outline : settings::flags_team_outline;
                    DrawVisualText(dl, font, fontSize, fx, fy, fcol, use_outline, focol, wc.team.c_str());
                    fy += fontSize + 2.0f;
                }
                if (settings::flags_state && !wc.state.empty())
                {
                    ImU32 fcol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_color[0], settings::flags_color[1], settings::flags_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_state_color[0], settings::flags_state_color[1], settings::flags_state_color[2], 1.0f));
                    ImU32 focol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_outline_color[0], settings::flags_outline_color[1], settings::flags_outline_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_state_outline_color[0], settings::flags_state_outline_color[1], settings::flags_state_outline_color[2], 1.0f));
                    bool use_outline = settings::flags_global ? settings::flags_outline : settings::flags_state_outline;
                    DrawVisualText(dl, font, fontSize, fx, fy, fcol, use_outline, focol, wc.state.c_str());
                    fy += fontSize + 2.0f;
                }
                if (settings::flags_health)
                {
                    ImU32 fcol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_color[0], settings::flags_color[1], settings::flags_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_health_color[0], settings::flags_health_color[1], settings::flags_health_color[2], 1.0f));
                    ImU32 focol = settings::flags_global
                        ? ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_outline_color[0], settings::flags_outline_color[1], settings::flags_outline_color[2], 1.0f))
                        : ImGui::ColorConvertFloat4ToU32(ImVec4(settings::flags_health_outline_color[0], settings::flags_health_outline_color[1], settings::flags_health_outline_color[2], 1.0f));
                    bool use_outline = settings::flags_global ? settings::flags_outline : settings::flags_health_outline;
                    char buf[16];
                    snprintf(buf, sizeof(buf), "%.0f%%", (double)wc.hp);
                    DrawVisualText(dl, font, fontSize, fx, fy, fcol, use_outline, focol, buf);
                }
            }
            if (settings::targethud_enabled && studsDist < 30.0f) {
                static uintptr_t s_lastTarget = 0;
                static float s_closestDist = 999.0f;
                if (studsDist < s_closestDist) {
                    s_closestDist = studsDist;
                    s_lastTarget = wc.playerAddr;
                    targethud::TargetInfo ti;
                    ti.name = wc.name;
                    ti.hp = wc.hp;
                    ti.maxHp = wc.maxHp;
                    ti.distance = studsDist;
                    ti.tool = wc.tool;
                    ti.valid = true;
                    targethud::set_target(ti);
                }
            }

            if (settings::tracer_enabled) {
                ImU32 tcol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::tracer_color[0], settings::tracer_color[1], settings::tracer_color[2], 1.0f));
                ImU32 tocol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::tracer_outline_color[0], settings::tracer_outline_color[1], settings::tracer_outline_color[2], 1.0f));
                math::vec2 hsp;
                bool hasHead = projection::to_screen(wc.head, view, fw, fh, hsp);
                if (hasHead && hsp.x >= 0 && hsp.x <= w && hsp.y >= 0 && hsp.y <= h) {
                    tracer::draw(dl, hsp.x - 1, hsp.y - 1, hsp.x + 1, hsp.y + 1, tcol, settings::tracer_outline, tocol, settings::tracer_thickness * distScale, w, h, settings::tracer_origin);
                } else {
                    tracer::draw(dl, rc.x0, rc.y0, rc.x1, rc.y1, tcol, settings::tracer_outline, tocol, settings::tracer_thickness * distScale, w, h, settings::tracer_origin);
                }
            }
        }

        if (settings::footsteps_enabled)
            footsteps::render(dl, view, fw, fh);

        kitesp::tick(dl, w, h);

        if (settings::targethud_enabled)
            targethud::render(dl, w, h);

        if (settings::aim_tracer)
        {
            math::vec3 aw{};
            if (rpbrx::aim::debug_target(aw))
            {
                math::vec2 asp;
                if (projection::to_screen(aw, view, fw, fh, asp)
                    && asp.x >= -100.0f && asp.x <= (float)w + 100.0f
                    && asp.y >= -100.0f && asp.y <= (float)h + 100.0f)
                {
                    float thick = settings::aim_tracer_thickness;
                    if (thick < 0.5f) thick = 0.5f;
                    if (thick > 8.0f) thick = 8.0f;
                    ImVec2 a{ (float)w * 0.5f, (float)h * 0.5f };
                    ImVec2 b{ asp.x, asp.y };
                    if (settings::aim_tracer_outline)
                    {
                        ImU32 oc = ImGui::ColorConvertFloat4ToU32(ImVec4(
                            settings::aim_tracer_outline_color[0], settings::aim_tracer_outline_color[1],
                            settings::aim_tracer_outline_color[2], 1.0f));
                        dl->AddLine(a, b, oc, thick + 1.5f);
                    }
                    ImU32 tc = ImGui::ColorConvertFloat4ToU32(ImVec4(
                        settings::aim_tracer_color[0], settings::aim_tracer_color[1],
                        settings::aim_tracer_color[2], 1.0f));
                    dl->AddLine(a, b, tc, thick);
                    dl->AddCircleFilled(b, 3.0f, tc);
                }
            }
        }
        if (settings::oof_enabled) {
            auto drawOofExtra = [&](WorldCache& wc2){
                math::vec4 clip = view.transform(wc2.head);
                float cx = w*0.5f, cy = h*0.5f;
                if (clip.w < 0.1f) { clip.x = -clip.x; clip.y = -clip.y; }
                float inv = 1.0f / (fabsf(clip.w) < 0.001f ? 0.001f : clip.w);
                float nx = clip.x * inv; float ny = clip.y * inv;
                float sx = (w*0.5f * nx) + cx; float sy = -(h*0.5f * ny) + cy;
                float dx = sx - cx, dy = sy - cy;
                if (clip.w < 0.1f) { dx = -dx; dy = -dy; }
                float len = sqrtf(dx*dx + dy*dy); if (len < 1) return;
                dx/=len; dy/=len;
                float ex = cx + dx * settings::oof_radius;
                float ey = cy + dy * settings::oof_radius;
                ex = std::clamp(ex, 30.0f, (float)w-30.0f); ey = std::clamp(ey, 30.0f, (float)h-30.0f);
                ImFont* f = g_verdanaSmallFont ? g_verdanaSmallFont : (g_verdanaFont ? g_verdanaFont : ImGui::GetFont());
                float fs = 10.0f;
                float yOff = 0;
                float inwardX = -dx * 18.0f;
                float inwardY = -dy * 18.0f;
                float baseX = std::floor(ex + inwardX);
                float baseY = std::floor(ey + inwardY);
                if (settings::oof_avatar) {
                    dl->AddRectFilled(ImVec2(baseX-8, baseY -8), ImVec2(baseX+8, baseY +8), IM_COL32(80,80,80,255));
                    dl->AddRect(ImVec2(baseX-8, baseY -8), ImVec2(baseX+8, baseY +8), IM_COL32(0,0,0,255));
                    baseY += 18;
                }
                if (settings::oof_names && !wc2.name.empty()) {
                    ImU32 nc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::oof_name_color[0], settings::oof_name_color[1], settings::oof_name_color[2], 1.0f));
                    ImVec2 ts = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, wc2.name.c_str());
                    float tx = baseX - ts.x*0.5f, ty = baseY;
                    DrawVisualText(dl, f, fs, tx, ty, nc, true, IM_COL32(0,0,0,255), wc2.name.c_str());
                    baseY += 12;
                }
                if (settings::oof_health) {
                    ImU32 hc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::oof_health_color[0], settings::oof_health_color[1], settings::oof_health_color[2], 1.0f));
                    char buf[16]; int pct=(int)(wc2.hp/wc2.maxHp*100+0.5f); snprintf(buf,sizeof(buf),"%d%%",pct);
                    ImVec2 ts = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, buf);
                    float tx = baseX - ts.x*0.5f, ty = baseY;
                    DrawVisualText(dl, f, fs, tx, ty, hc, true, IM_COL32(0,0,0,255), buf);
                    baseY += 12;
                }
                if (settings::oof_distance) {
                    ImU32 dc = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::oof_distance_color[0], settings::oof_distance_color[1], settings::oof_distance_color[2], 1.0f));
                    float d = (wc2.root - g_ctx.local_pos).length()*0.28f;
                    char buf[16]; snprintf(buf,sizeof(buf),"%.0fm",d);
                    ImVec2 ts = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, buf);
                    float tx = baseX - ts.x*0.5f, ty = baseY;
                    DrawVisualText(dl, f, fs, tx, ty, dc, true, IM_COL32(0,0,0,255), buf);
                }
            };
            for (auto& wc2 : g_world) {
                math::vec2 sp; bool on = projection::to_screen(wc2.head, view, fw, fh, sp);
                bool off = !on || sp.x < 0 || sp.x > w || sp.y < 0 || sp.y > h || !wc2.valid;
                if (!off) continue;
                if (wc2.head.x==0 && wc2.head.y==0 && wc2.head.z==0) continue;
                ImU32 ocol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::oof_color[0], settings::oof_color[1], settings::oof_color[2], 1.0f));
                ImU32 oocol = ImGui::ColorConvertFloat4ToU32(ImVec4(settings::oof_outline_color[0], settings::oof_outline_color[1], settings::oof_outline_color[2], 1.0f));
                oof::draw(dl, wc2.head, view, w, h, ocol, settings::oof_outline, oocol, settings::oof_size, settings::oof_radius);
                drawOofExtra(wc2);
            }
        }
    }
}
