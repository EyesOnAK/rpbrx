#include "autoshoot.h"
#include "../health/health.h"
#include "../distance/distance.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../menu/overlay/settings.h"
#include <Windows.h>
#include <cmath>
#include <chrono>
#include <algorithm>

namespace rpbrx::aim::autoshoot
{
    static inline node get_target_node(const node& character, int partIdx);

    struct CachedCrosshairTarget
    {
        uintptr_t playerAddr = 0;
        uintptr_t characterAddr = 0;
        uintptr_t humanoidAddr = 0;
        uintptr_t headPrim = 0;
        uintptr_t rootPrim = 0;
        uintptr_t targetPrim = 0;
        int partIdx = -1;
    };

    static std::vector<CachedCrosshairTarget> s_chTargets;
    static std::chrono::steady_clock::time_point s_chLastResolve{};

    static void resolve_crosshair_targets()
    {
        s_chTargets.clear();
        int wantPart = settings::aim_part;
        auto list = g_ctx.player_list();
        s_chTargets.reserve(list.size());
        for (auto& entry : list)
        {
            if (entry.addr == g_ctx.local_player.addr)
                continue;
            uintptr_t charAddr = g_proc->read<uintptr_t>(entry.addr + Offsets::Player::ModelInstance);
            if (!charAddr)
                continue;
            node character{ charAddr };
            node humanoid = find_live_humanoid(character);
            if (!humanoid)
                continue;
            node headNode = character.find_child("Head");
            node rootNode = character.find_child("HumanoidRootPart");
            if (!rootNode) rootNode = character.find_child("Torso");
            if (!headNode || !rootNode)
                continue;
            uintptr_t headPrim = headNode.primitive();
            uintptr_t rootPrim = rootNode.primitive();
            if (!headPrim || !rootPrim)
                continue;
            node targetPart = get_target_node(character, wantPart);
            uintptr_t targetPrim = targetPart ? targetPart.primitive() : 0;
            CachedCrosshairTarget t{};
            t.playerAddr = entry.addr;
            t.characterAddr = charAddr;
            t.humanoidAddr = humanoid.addr;
            t.headPrim = headPrim;
            t.rootPrim = rootPrim;
            t.targetPrim = targetPrim;
            t.partIdx = wantPart;
            s_chTargets.push_back(t);
        }
    }

    static inline node get_target_node(const node& character, int partIdx)
    {
        switch (partIdx)
        {
        case 0:
            return character.find_child("Head");
        case 1:
        {
            node t = character.find_child("UpperTorso");
            if (!t) t = character.find_child("Torso");
            if (!t) t = character.find_child("HumanoidRootPart");
            return t;
        }
        case 2:
        {
            node hrp = character.find_child("HumanoidRootPart");
            if (!hrp) hrp = character.find_child("Torso");
            if (!hrp) hrp = character.find_child("UpperTorso");
            return hrp;
        }
        case 3:
        {
            node a = character.find_child("LeftUpperArm");
            if (!a) a = character.find_child("Left Arm");
            if (!a) a = character.find_child("LeftLowerArm");
            return a;
        }
        case 4:
        {
            node a = character.find_child("RightUpperArm");
            if (!a) a = character.find_child("Right Arm");
            if (!a) a = character.find_child("RightLowerArm");
            return a;
        }
        case 5:
        {
            node l = character.find_child("LeftUpperLeg");
            if (!l) l = character.find_child("Left Leg");
            if (!l) l = character.find_child("LeftLowerLeg");
            return l;
        }
        case 6:
        {
            node l = character.find_child("RightUpperLeg");
            if (!l) l = character.find_child("Right Leg");
            if (!l) l = character.find_child("RightLowerLeg");
            return l;
        }
        default:
            return character.find_child("Head");
        }
    }

    bool is_target_in_crosshair(int w, int h)
    {
        if (!g_ctx.ready || !g_ctx.camera || w <= 0 || h <= 0)
            return false;

        auto now = std::chrono::steady_clock::now();
        int wantPart = settings::aim_part;
        if (s_chTargets.empty()
            || now - s_chLastResolve >= std::chrono::milliseconds(200)
            || s_chTargets[0].partIdx != wantPart)
        {
            resolve_crosshair_targets();
            s_chLastResolve = now;
            if (s_chTargets.empty())
                return false;
        }

        float cx = static_cast<float>(w) * 0.5f;
        float cy = static_cast<float>(h) * 0.5f;

        const math::view_matrix& view = g_ctx.view;
        math::vec3 camPos = g_proc->read<math::vec3>(g_ctx.camera.addr + Offsets::Camera::Position);

        for (auto& t : s_chTargets)
        {
            uintptr_t charAddr = g_proc->read<uintptr_t>(t.playerAddr + Offsets::Player::ModelInstance);
            if (charAddr != t.characterAddr)
                continue;
            float hp = g_proc->read<float>(t.humanoidAddr + Offsets::Humanoid::Health);
            if (!health::check(hp))
                continue;

            math::vec3 headPos = g_proc->read<math::vec3>(t.headPrim + Offsets::Primitive::Position);
            math::vec3 rootPos = g_proc->read<math::vec3>(t.rootPrim + Offsets::Primitive::Position);
            if (headPos.x == 0.0f && headPos.y == 0.0f && headPos.z == 0.0f) continue;
            if (rootPos.x == 0.0f && rootPos.y == 0.0f && rootPos.z == 0.0f) continue;
            if ((headPos - rootPos).length() > 10.0f) continue;

            if (!distance::check(headPos, camPos))
                continue;

            math::vec3 top3{ headPos.x, headPos.y + 0.6f, headPos.z };
            math::vec3 bot3{ rootPos.x, rootPos.y - 2.8f, rootPos.z };
            math::vec2 topScreen{}, botScreen{};
            if (!projection::to_screen(top3, view, static_cast<float>(w), static_cast<float>(h), topScreen))
                continue;
            if (!projection::to_screen(bot3, view, static_cast<float>(w), static_cast<float>(h), botScreen))
                continue;

            float hh = botScreen.y - topScreen.y;
            if (hh <= 2.0f || hh > static_cast<float>(h) * 1.5f) continue;
            float ww = hh * 0.55f;
            float x0 = topScreen.x - ww * 0.5f;
            float x1 = topScreen.x + ww * 0.5f;
            float y0 = topScreen.y;
            float y1 = botScreen.y;

            if (cx >= x0 && cx <= x1 && cy >= y0 && cy <= y1)
                return true;

            if (t.targetPrim)
            {
                math::vec3 tPos = g_proc->read<math::vec3>(t.targetPrim + Offsets::Primitive::Position);
                math::vec2 tScreen{};
                if (projection::to_screen(tPos, view, static_cast<float>(w), static_cast<float>(h), tScreen))
                {
                    float dx = tScreen.x - cx;
                    float dy = tScreen.y - cy;
                    float dist = sqrtf(dx * dx + dy * dy);
                    float hitRadius = fmaxf(hh * 0.18f, 12.0f);
                    if (dist <= hitRadius)
                        return true;
                }
            }
        }
        return false;
    }

    void run(int w, int h)
    {
        if (!settings::aim_autoshoot)
            return;

        using clock = std::chrono::steady_clock;

        static clock::time_point s_lastPoll{};
        auto pollNow = clock::now();
        if (pollNow - s_lastPoll < std::chrono::milliseconds(4))
            return;
        s_lastPoll = pollNow;

        static clock::time_point s_targetFirstSeen{};
        static clock::time_point s_lastShotTime{};
        static bool s_hasTarget = false;

        bool inCrosshair = is_target_in_crosshair(w, h);
        auto now = clock::now();

        if (inCrosshair)
        {
            if (!s_hasTarget)
            {
                s_hasTarget = true;
                s_targetFirstSeen = now;
            }

            auto elapsedSinceSeen = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_targetFirstSeen).count();
            auto elapsedSinceLastShot = std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastShotTime).count();

            float delayMs = settings::aim_autoshoot_delay;
            if (delayMs < 0.0f) delayMs = 0.0f;
            long long minCooldown = static_cast<long long>((delayMs > 60.0f) ? delayMs : 60.0f);

            if (elapsedSinceSeen >= static_cast<long long>(delayMs) && elapsedSinceLastShot >= minCooldown)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                s_lastShotTime = now;
            }
        }
        else
        {
            s_hasTarget = false;
        }
    }
}
