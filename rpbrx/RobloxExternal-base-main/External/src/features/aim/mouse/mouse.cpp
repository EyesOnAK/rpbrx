#include "mouse.h"
#include "../core/aim.h"
#include "../health/health.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../utility/global/globals.h"
#include "../../../menu/overlay/settings.h"
#include <Windows.h>
#include <cmath>
#include <algorithm>
#include <chrono>

namespace rpbrx::aim::mouse
{
    // -- sub-pixel accumulation & rate limiting --
    static float s_remX = 0.0f;
    static float s_remY = 0.0f;
    static std::chrono::steady_clock::time_point s_lastSend{};
    static std::chrono::steady_clock::time_point s_lastFrame{};

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

    // -- resolve target part using catsix mapping: 0=HumanoidRootPart, 1=Head --
    static node get_target_part(const node& character, int partIdx)
    {
        if (partIdx == 1)
            return character.find_child("Head");

        // 0 = RootPart
        node hrp = character.find_child("HumanoidRootPart");
        if (!hrp) hrp = character.find_child("Torso");
        if (!hrp) hrp = character.find_child("UpperTorso");
        return hrp;
    }

    void run(int w, int h)
    {
        if (!g_ctx.ready || !g_ctx.camera)
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

        // deltaTime
        auto now = std::chrono::steady_clock::now();
        float dt = 0.004f;
        if (s_lastFrame != std::chrono::steady_clock::time_point{})
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - s_lastFrame).count();
            dt = static_cast<float>(elapsed) / 1000000.0f;
            if (dt <= 0.0f || dt > 0.1f) dt = 0.004f;
        }
        s_lastFrame = now;

        float cx = static_cast<float>(w) * 0.5f;
        float cy = static_cast<float>(h) * 0.5f;

        const math::view_matrix& view = g_ctx.view;

        int wantPart = settings::aim_part;
        float fov = settings::aim_fov;

        // find closest target in FOV circle
        math::vec3 bestWorld{};
        math::vec2 bestScreen{};
        bool found = false;
        float bestDist = fov;
        uintptr_t bestPlayer = 0;

        auto list = g_ctx.player_list();
        for (auto& entry : list)
        {
            if (entry.addr == g_ctx.local_player.addr)
                continue;

            node character = g_ctx.get_character(entry);
            if (!character)
                continue;

            // team check
            if (settings::aim_team_check)
            {
                int localTeam = g_proc->read<int>(g_ctx.local_player.addr + Offsets::Player::TeamColor);
                int otherTeam = g_proc->read<int>(entry.addr + Offsets::Player::TeamColor);
                if (localTeam != 0 && localTeam == otherTeam)
                    continue;
            }

            // alive check
            node humanoid = find_live_humanoid(character);
            if (!humanoid)
                continue;
            float hp = g_proc->read<float>(humanoid.addr + Offsets::Humanoid::Health);
            if (!health::check(hp))
                continue;

            // get target part
            node part = get_target_part(character, wantPart);
            if (!part)
                continue;
            uintptr_t prim = part.primitive();
            if (!prim)
                continue;

            math::vec3 pos = g_proc->read<math::vec3>(prim + Offsets::Primitive::Position);
            if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
                continue;

            // project to screen
            math::vec2 s{};
            if (!projection::to_screen(pos, view, static_cast<float>(w), static_cast<float>(h), s))
                continue;

            float dx = s.x - cx;
            float dy = s.y - cy;
            float d = sqrtf(dx * dx + dy * dy);

            if (d < bestDist)
            {
                bestDist = d;
                bestWorld = pos;
                bestScreen = s;
                bestPlayer = entry.addr;
                found = true;
            }
        }

        if (!found)
        {
            s_remX = 0.0f;
            s_remY = 0.0f;
            rpbrx::aim::clear_debug_target();
            return;
        }

        rpbrx::aim::set_debug_target(bestWorld, bestPlayer);

        // compute pixel delta from center to target
        float dx = bestScreen.x - cx;
        float dy = bestScreen.y - cy;

        if (!std::isfinite(dx) || !std::isfinite(dy))
            return;
        if (fabsf(dx) > static_cast<float>(w) || fabsf(dy) > static_cast<float>(h))
            return;

        // catsix smoothing: scale by min(speed * dt, 1.0), divide by smoothing factor
        float speed = settings::aim_speed;
        float factor = (std::min)(speed * dt, 1.0f);
        dx *= factor;
        dy *= factor;

        send_mouse_move(dx, dy);
    }
}
