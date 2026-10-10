#include "aim.h"
#include "../mouse/mouse.h"
#include "../viewport/viewport.h"
#include "../raycast/raycast.h"
#include "../wraith/wraith.h"
#include "../autoshoot/autoshoot.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/core/gui/menu.h"
#include <Windows.h>
#include <chrono>

namespace rpbrx::aim
{
    namespace
    {
        math::vec3 s_debugTarget{};
        uintptr_t s_debugPlayer = 0;
        bool s_hasDebugTarget = false;
        std::chrono::steady_clock::time_point s_debugT{};

        uintptr_t s_prevPlayer = 0;
        std::chrono::steady_clock::time_point s_prevT{};
    }

    static bool debug_fresh()
    {
        if (!s_hasDebugTarget)
            return false;
        if (std::chrono::steady_clock::now() - s_debugT > std::chrono::milliseconds(300))
        {
            s_hasDebugTarget = false;
            s_debugPlayer = 0;
            return false;
        }
        return true;
    }

    void set_debug_target(const math::vec3& world, uintptr_t playerAddr)
    {
        auto now = std::chrono::steady_clock::now();
        if (s_hasDebugTarget && playerAddr != s_debugPlayer)
        {
            s_prevPlayer = s_debugPlayer;
            s_prevT = now;
        }
        s_debugTarget = world;
        s_debugPlayer = playerAddr;
        s_hasDebugTarget = true;
        s_debugT = now;
    }

    void clear_debug_target()
    {

        s_hasDebugTarget = false;
        s_debugPlayer = 0;
    }

    bool debug_target(math::vec3& out)
    {
        if (!debug_fresh())
            return false;
        out = s_debugTarget;
        return true;
    }

    bool debug_target_player(uintptr_t& out)
    {
        if (!debug_fresh())
            return false;
        out = s_debugPlayer;
        return true;
    }

    bool debug_recent_player(uintptr_t addr)
    {
        if (addr == 0)
            return false;
        auto now = std::chrono::steady_clock::now();
        if (s_hasDebugTarget && now - s_debugT <= std::chrono::milliseconds(300) && addr == s_debugPlayer)
            return true;
        if (s_prevPlayer != 0 && addr == s_prevPlayer && now - s_prevT <= std::chrono::milliseconds(750))
            return true;
        return false;
    }

    bool debug_in_use()
    {
        auto now = std::chrono::steady_clock::now();
        if (s_hasDebugTarget && now - s_debugT <= std::chrono::milliseconds(300))
            return true;
        if (s_prevPlayer != 0 && now - s_prevT <= std::chrono::milliseconds(750))
            return true;
        return false;
    }

    node resolve_part(const node& character, int partIdx)
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

    void run(int w, int h)
    {
        bool keyOn = settings::aim_enabled && Menu::IsMenuKeyActive("##aim_key", settings::aim_key, settings::aim_key_mode);
        if (keyOn && settings::aim_method == 0)
            mouse::run(w, h);
        else if (keyOn && settings::aim_method == 1)
            viewport::run(w, h);
        else if (keyOn && settings::aim_method == 2)
            raycast::run(w, h);
        else if (keyOn && settings::aim_method == 3)
            wraith::run(w, h);
        else
        {
            viewport::clear();
            clear_debug_target();
        }
        if (settings::aim_method != 1)
            viewport::clear();
        if (settings::aim_method != 2 || !keyOn)
            raycast::idle();
        if (settings::aim_method != 3 || !keyOn)
            wraith::idle();

        if (settings::aim_autoshoot)
        {
            bool keyActive = (settings::aim_autoshoot_key == 0) || Menu::IsMenuKeyActive("##autoshoot_key", settings::aim_autoshoot_key, settings::aim_autoshoot_key_mode);
            if (keyActive)
            {
                autoshoot::run(w, h);
            }
        }
    }
}
