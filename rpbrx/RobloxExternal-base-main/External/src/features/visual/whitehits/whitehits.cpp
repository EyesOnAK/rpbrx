#include "whitehits.h"
#include "../../../menu/overlay/settings.h"
#include <unordered_map>
#include <chrono>

namespace rpbrx::visual::whitehits
{
    struct HitFlash {
        std::chrono::steady_clock::time_point start;
    };

    static std::unordered_map<uintptr_t, HitFlash> g_flashes;

    void on_health_change(uintptr_t playerAddr, float oldHp, float newHp)
    {
        if (!settings::whitehits_enabled) return;
        if (newHp < oldHp && oldHp > 0.0f)
            g_flashes[playerAddr] = { std::chrono::steady_clock::now() };
    }

    void draw(ImDrawList* dl, uintptr_t playerAddr, float x0, float y0, float x1, float y1)
    {
        if (!dl || !settings::whitehits_enabled) return;

        auto it = g_flashes.find(playerAddr);
        if (it == g_flashes.end()) return;

        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - it->second.start).count();
        float dur = settings::whitehits_duration;
        if (dur < 0.01f) dur = 0.01f;

        if (elapsed >= dur) {
            g_flashes.erase(it);
            return;
        }

        float alpha = (1.0f - elapsed / dur) * settings::whitehits_color[3];
        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
            settings::whitehits_color[0], settings::whitehits_color[1],
            settings::whitehits_color[2], alpha));

        dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), col);
    }

    void clear()
    {
        g_flashes.clear();
    }
}
