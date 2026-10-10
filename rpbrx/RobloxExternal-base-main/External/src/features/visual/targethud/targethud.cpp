#include "targethud.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/resources/font.h"
#include <cstdio>
#include <cmath>
#include <algorithm>

namespace rpbrx::visual::targethud
{
    static TargetInfo g_target;

    void set_target(const TargetInfo& info)
    {
        g_target = info;
    }

    void clear_target()
    {
        g_target.valid = false;
    }

    void render(ImDrawList* dl, int screenW, int screenH)
    {
        if (!dl || !settings::targethud_enabled || !g_target.valid) return;

        ImFont* font = g_verdanaFont ? g_verdanaFont : ImGui::GetFont();
        float fontSize = 14.0f;

        float px = settings::targethud_x;
        float py = settings::targethud_y;
        float panelW = 200.0f;
        float panelH = 60.0f;

        ImU32 bgCol = IM_COL32(20, 20, 20, 200);
        ImU32 borderCol = IM_COL32(80, 80, 80, 200);
        ImU32 nameCol = IM_COL32(255, 255, 255, 255);
        ImU32 distCol = IM_COL32(180, 180, 180, 255);

        dl->AddRectFilled(ImVec2(px, py), ImVec2(px + panelW, py + panelH), bgCol, 4.0f);
        dl->AddRect(ImVec2(px, py), ImVec2(px + panelW, py + panelH), borderCol, 4.0f);

        dl->AddText(font, fontSize, ImVec2(px + 8, py + 4), nameCol, g_target.name.c_str());

        char distBuf[32];
        snprintf(distBuf, sizeof(distBuf), "%.0fm", g_target.distance * 0.28f);
        ImVec2 dts = font->CalcTextSizeA(fontSize - 2, FLT_MAX, 0.0f, distBuf);
        dl->AddText(font, fontSize - 2, ImVec2(px + panelW - dts.x - 8, py + 5), distCol, distBuf);

        float barX = px + 8;
        float barY = py + 22;
        float barW = panelW - 16;
        float barH = 10;
        float hpFrac = std::clamp(g_target.hp / (g_target.maxHp > 0 ? g_target.maxHp : 100.f), 0.0f, 1.0f);

        float r = 1.0f - hpFrac;
        float g = hpFrac;
        ImU32 hpCol = ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, 0.0f, 1.0f));

        dl->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barW, barY + barH), IM_COL32(40, 40, 40, 255), 2.0f);
        dl->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barW * hpFrac, barY + barH), hpCol, 2.0f);

        char hpBuf[32];
        snprintf(hpBuf, sizeof(hpBuf), "%.0f / %.0f", g_target.hp, g_target.maxHp);
        ImVec2 hts = font->CalcTextSizeA(10.0f, FLT_MAX, 0.0f, hpBuf);
        dl->AddText(font, 10.0f, ImVec2(barX + barW * 0.5f - hts.x * 0.5f, barY - 1), IM_COL32(255, 255, 255, 255), hpBuf);

        if (!g_target.tool.empty()) {
            dl->AddText(font, fontSize - 2, ImVec2(px + 8, py + 36), IM_COL32(200, 200, 200, 255), g_target.tool.c_str());
        }
    }
}
