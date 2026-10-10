#include "chams.h"
#include "../../../menu/overlay/settings.h"

namespace rpbrx::visual::chams
{
    void draw(ImDrawList* dl, float x0, float y0, float x1, float y1)
    {
        if (!dl || !settings::chams_enabled) return;

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
            settings::chams_color[0], settings::chams_color[1],
            settings::chams_color[2], settings::chams_color[3]));

        dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), col);

        ImU32 border = ImGui::ColorConvertFloat4ToU32(ImVec4(
            settings::chams_color[0], settings::chams_color[1],
            settings::chams_color[2], settings::chams_color[3] * 1.5f > 1.0f ? 1.0f : settings::chams_color[3] * 1.5f));
        dl->AddRect(ImVec2(x0, y0), ImVec2(x1, y1), border, 0.0f, 0, 1.5f);
    }
}
