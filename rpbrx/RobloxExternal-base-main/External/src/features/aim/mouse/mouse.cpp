#include "mouse.h"
#include "../core/aim.h"
#include "../health/health.h"
#include "../distance/distance.h"
#include "../tracking/tracking.h"
#include "../pftarget/pftarget.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../utility/global/globals.h"
#include "../../../menu/overlay/settings.h"
#include "../hitbox/hitbox.h"
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>

namespace rpbrx::aim::mouse
{
    static inline node get_target_node(const node& character, int partIdx);

    struct CachedTarget
    {
        uintptr_t playerAddr = 0;
        uintptr_t characterAddr = 0;
        uintptr_t humanoidAddr = 0;
        uintptr_t targetPrim = 0;
        int partIdx = -1;
    };

    static std::vector<CachedTarget> s_targets;
    static std::chrono::steady_clock::time_point s_lastResolve{};
    static std::chrono::steady_clock::time_point s_lastScan{};

    static float s_remX = 0.0f;
    static float s_remY = 0.0f;
    static bool s_inDeadzone = false;
    static uintptr_t s_dzPlayer = 0;
    static std::chrono::steady_clock::time_point s_lastSend{};

    static void resolve_targets()
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
            node targetPart = get_target_node(character, wantPart);
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

    static void send_mouse_move(float dx, float dy)
    {
        auto now = std::chrono::steady_clock::now();
        if (s_lastSend != std::chrono::steady_clock::time_point{} &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - s_lastSend).count() < 8)
        {
            s_remX += dx;
            s_remY += dy;
            if (fabsf(s_remX) > 32.0f) s_remX = 0.0f;
            if (fabsf(s_remY) > 32.0f) s_remY = 0.0f;
            return;
        }
        s_lastSend = now;

        dx += s_remX;
        dy += s_remY;

        int ix = static_cast<int>(std::round(dx));
        int iy = static_cast<int>(std::round(dy));

        s_remX = dx - static_cast<float>(ix);
        s_remY = dy - static_cast<float>(iy);

        if (fabsf(s_remX) > 8.0f) s_remX = 0.0f;
        if (fabsf(s_remY) > 8.0f) s_remY = 0.0f;

        if (ix == 0 && iy == 0)
            return;

        INPUT in{};
        in.type = INPUT_MOUSE;
        in.mi.dwFlags = MOUSEEVENTF_MOVE;
        in.mi.dx = ix;
        in.mi.dy = iy;
        SendInput(1, &in, sizeof(in));
    }

    static void pf_run(int w, int h)
    {
        pftarget::PfBest b{};
        if (!pftarget::select_best(w, h, b))
        {
            s_inDeadzone = false;
            s_dzPlayer = 0;
            s_remX = 0.0f;
            s_remY = 0.0f;
            rpbrx::aim::clear_debug_target();
            return;
        }
        if (b.model != s_dzPlayer)
        {
            s_inDeadzone = false;
            s_dzPlayer = b.model;
        }

        if (settings::aim_deadzone_enabled)
        {
            int dzMin = settings::aim_deadzone_min;
            int dzMax = settings::aim_deadzone_max;
            if (dzMin < 0) dzMin = 0;
            if (dzMax < dzMin) dzMax = dzMin;
            if (!s_inDeadzone && b.dist2d <= (float)dzMin)
                s_inDeadzone = true;
            else if (s_inDeadzone && b.dist2d > (float)dzMax)
                s_inDeadzone = false;
            if (s_inDeadzone)
                return;
        }
        else
        {
            s_inDeadzone = false;
        }

        rpbrx::aim::set_debug_target(b.world, b.model);

        float dx = b.screen.x - (float)w * 0.5f;
        float dy = b.screen.y - (float)h * 0.5f;

        if (!std::isfinite(dx) || !std::isfinite(dy))
            return;
        if (fabsf(dx) > (float)w || fabsf(dy) > (float)h)
            return;

        if (settings::aim_smoothing_enabled)
        {
            float sx = settings::aim_smoothing_x;
            float sy = settings::aim_smoothing_y;
            if (sx < 1.0f) sx = 1.0f;
            if (sy < 1.0f) sy = 1.0f;
            dx /= sx;
            dy /= sy;
        }

        send_mouse_move(dx, dy);
    }

    void run(int w, int h)
    {
        if (!g_ctx.ready)
            return;
        if (!g_ctx.camera)
            return;
        if (w <= 0 || h <= 0)
            return;
        if (g_menuOpen)
        {
            s_remX = 0.0f;
            s_remY = 0.0f;
            rpbrx::aim::clear_debug_target();
            return;
        }

        auto now = std::chrono::steady_clock::now();
        if (now - s_lastScan < std::chrono::milliseconds(4))
            return;
        s_lastScan = now;

        if (pftarget::active())
        {
            pf_run(w, h);
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
                return;
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
            float hbScale = hitbox::get_scale();
            if (hbScale > 1.0f) d /= hbScale;
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
            s_inDeadzone = false;
            s_dzPlayer = 0;
            s_remX = 0.0f;
            s_remY = 0.0f;
            rpbrx::aim::clear_debug_target();
            return;
        }
        if (bestPlayer != s_dzPlayer)
        {
            s_inDeadzone = false;
            s_dzPlayer = bestPlayer;
        }

        if (settings::aim_deadzone_enabled)
        {
            int dzMin = settings::aim_deadzone_min;
            int dzMax = settings::aim_deadzone_max;
            if (dzMin < 0) dzMin = 0;
            if (dzMax < dzMin) dzMax = dzMin;
            if (!s_inDeadzone && bestDist <= (float)dzMin)
                s_inDeadzone = true;
            else if (s_inDeadzone && bestDist > (float)dzMax)
                s_inDeadzone = false;
            if (s_inDeadzone)
                return;
        }
        else
        {
            s_inDeadzone = false;
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
                best.z += (tv.vz - lv.z) * leadT * damp;
            }
        }

        rpbrx::aim::set_debug_target(best, bestPlayer);

        math::vec2 targetScreen{};
        if (!projection::to_screen(best, view, static_cast<float>(w), static_cast<float>(h), targetScreen))
            return;

        float dx = targetScreen.x - cx;
        float dy = targetScreen.y - cy;

        if (!std::isfinite(dx) || !std::isfinite(dy))
            return;
        if (fabsf(dx) > (float)w || fabsf(dy) > (float)h)
            return;

        if (settings::aim_smoothing_enabled)
        {
            float sx = settings::aim_smoothing_x;
            float sy = settings::aim_smoothing_y;
            if (sx < 1.0f) sx = 1.0f;
            if (sy < 1.0f) sy = 1.0f;
            dx /= sx;
            dy /= sy;
        }

        send_mouse_move(dx, dy);
    }
}
