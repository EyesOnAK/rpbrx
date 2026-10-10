#include "name.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/resources/font.h"
#include <cmath>

namespace rpbrx::visual::name
{
    void draw(ImDrawList* dl, const data::rect& rc, const std::string& str)
    {
        if (!dl || str.empty()) return;
        ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
        float fs = 13.0f * rpbrx::settings::name_scale;
        ImVec2 ts = font->CalcTextSizeA(fs, FLT_MAX, 0.0f, str.c_str());
        float tx = std::floor(rc.x0 + (rc.x1 - rc.x0) * 0.5f - ts.x * 0.5f);
        float ty = std::floor(rc.y0 - ts.y - 3.0f);

        // Background
        if (rpbrx::settings::name_show_background) {
            float pad = 2.0f;
            ImU32 bgCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, rpbrx::settings::name_bg_transparency));
            dl->AddRectFilled(ImVec2(tx - pad, ty - pad), ImVec2(tx + ts.x + pad, ty + ts.y + pad), bgCol);
        }

        ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(
            rpbrx::settings::name_color[0], rpbrx::settings::name_color[1],
            rpbrx::settings::name_color[2], rpbrx::settings::name_color[3]));

        // Outline (shadow)
        static const ImVec2 offsets[4] = {
            ImVec2(-1.0f, -1.0f), ImVec2(1.0f, -1.0f),
            ImVec2(-1.0f,  1.0f), ImVec2(1.0f,  1.0f)
        };
        ImU32 oc = IM_COL32(0, 0, 0, 200);
        for (int i = 0; i < 4; i++) {
            dl->AddText(font, fs, ImVec2(tx + offsets[i].x, ty + offsets[i].y), oc, str.c_str());
        }
        dl->AddText(font, fs, ImVec2(tx, ty), col, str.c_str());
    }
}
