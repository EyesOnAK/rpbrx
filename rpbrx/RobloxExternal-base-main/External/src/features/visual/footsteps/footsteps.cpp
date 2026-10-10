#include "footsteps.h"
#include "../../../menu/overlay/settings.h"
#include <vector>
#include <chrono>
#include <cmath>

namespace rpbrx::visual::footsteps
{
    struct FootprintMark {
        math::vec3 pos;
        std::chrono::steady_clock::time_point time;
    };

    static std::vector<FootprintMark> g_marks;
    static std::chrono::steady_clock::time_point g_lastAdd{};

    void update(uintptr_t playerAddr, const math::vec3& footPos)
    {
        if (!settings::footsteps_enabled) return;
        if (footPos.x == 0 && footPos.y == 0 && footPos.z == 0) return;

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration<float>(now - g_lastAdd).count() < 0.15f) return;
        g_lastAdd = now;

        if ((int)g_marks.size() >= settings::footsteps_max) {
            g_marks.erase(g_marks.begin());
        }
        g_marks.push_back({ footPos, now });
    }

    void render(ImDrawList* dl, const math::view_matrix& view, float screenW, float screenH)
    {
        if (!dl || !settings::footsteps_enabled || g_marks.empty()) return;

        auto now = std::chrono::steady_clock::now();
        float lifetime = settings::footsteps_lifetime;
        if (lifetime < 0.1f) lifetime = 0.1f;

        for (int i = (int)g_marks.size() - 1; i >= 0; --i) {
            float age = std::chrono::duration<float>(now - g_marks[i].time).count();
            if (age > lifetime) {
                g_marks.erase(g_marks.begin(), g_marks.begin() + i + 1);
                break;
            }
        }

        float radius = settings::footsteps_radius;
        for (auto& m : g_marks) {
            math::vec2 sp;
            if (!projection::to_screen(m.pos, view, screenW, screenH, sp)) continue;

            float age = std::chrono::duration<float>(now - m.time).count();
            float alpha = (1.0f - age / lifetime) * settings::footsteps_color[3];
            if (alpha <= 0.0f) continue;

            float screenR = radius * (screenH / 800.0f) * (1.0f - age / lifetime * 0.3f);
            if (screenR < 1.0f) screenR = 1.0f;

            ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
                settings::footsteps_color[0], settings::footsteps_color[1],
                settings::footsteps_color[2], alpha));
            dl->AddCircle(ImVec2(sp.x, sp.y), screenR, col, 16, 1.0f);
        }
    }

    void clear()
    {
        g_marks.clear();
    }
}
