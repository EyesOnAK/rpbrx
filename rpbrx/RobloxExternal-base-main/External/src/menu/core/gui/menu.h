#pragma once
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shlobj.h>
#include "../../imgui/imgui.h"
#include "../../../utility/global/globals.h"
#include "../../../utility/logger/logger.h"
#include "../overlay/overlay.h"
#include "resources/icons.h"
#include "../../../menu/overlay/settings.h"
#include "../../../utility/cache/cache.h"
// removed: player_preview, hitsound, explorer, raycast, cache includes
namespace S = rpbrx::settings;
namespace Menu
{
inline int g_activeTab = 0;
inline bool g_navBarHov = false;
inline bool g_navHidden = false;
inline bool g_settingsOpen = false;
inline float g_setAnim = 0.0f;
inline ImVec2 g_setPos = ImVec2(-1.0f, -1.0f);
inline ImVec2 g_setSize = ImVec2(340.0f, 348.0f);
inline bool g_setDragging = false;
inline ImVec2 g_setDragOff = ImVec2(0.0f, 0.0f);
inline bool g_themeOpen = false;
inline float g_themeAnim = 0.0f;
inline ImVec2 g_themePos = ImVec2(-1.0f, -1.0f);
inline ImVec2 g_themeSize = ImVec2(340.0f, 330.0f);
inline bool g_themeDragging = false;
inline ImVec2 g_themeDragOff = ImVec2(0.0f, 0.0f);
inline bool g_explorerOpen = false;
inline ImVec2 g_explorerPos = ImVec2(-1.0f, -1.0f);
inline ImVec2 g_explorerSize = ImVec2(680.0f, 560.0f);
inline bool g_explorerDragging = false;
inline ImVec2 g_explorerDragOff = ImVec2(0.0f, 0.0f);
inline bool g_playerPreviewOpen = false;
inline ImVec2 g_playerPreviewPos = ImVec2(-1.0f, -1.0f);
inline ImVec2 g_playerPreviewSize = ImVec2(280.0f, 380.0f);
inline bool g_playerPreviewDragging = false;
inline ImVec2 g_playerPreviewDragOff = ImVec2(0.0f, 0.0f);
inline bool g_previewSpin = false;
inline bool g_previewAnimateHp = true;
inline ImVec2 g_pos = ImVec2(-1.0f, -1.0f);
inline float g_menuAnim = 0.0f;
inline bool g_dragging = false;
inline ImVec2 g_dragOff = ImVec2(0.0f, 0.0f);
inline ImVec2 g_menuSize = ImVec2(540.0f, 740.0f);
inline bool g_watermark = true;
inline bool g_watermarkFps = true;
inline bool g_watermarkPlayers = true;
inline bool g_watermarkDate = false;
inline bool g_keybinds = false;
inline ImVec2 g_bindsPos = ImVec2(20.0f, 260.0f);
inline ImVec2 g_bindsSize = ImVec2(160.0f, 20.0f);
inline bool g_bindsDragging = false;
inline ImVec2 g_bindsDragOff = ImVec2(0.0f, 0.0f);
inline bool g_internalDebug = false;
inline double g_dbgMsRefresh = 0.0;
inline double g_dbgMsPf = 0.0;
inline double g_dbgMsMenu = 0.0;
inline double g_dbgMsAim = 0.0;
inline double g_dbgMsVisual = 0.0;
inline double g_dbgMsMisc = 0.0;
inline ImVec2 g_debugPos = ImVec2(20.0f, 520.0f);
inline ImVec2 g_debugSize = ImVec2(270.0f, 180.0f);
inline bool g_debugDragging = false;
inline ImVec2 g_debugDragOff = ImVec2(0.0f, 0.0f);
inline bool g_movement = false;
inline bool g_movementVelocityOpt = true;
inline bool g_movementGraphOpt = true;
inline bool g_movementKeysOpt = true;
inline float g_movementVelocity = -1.0f;
inline ImVec2 g_movementPos = ImVec2(20.0f, 320.0f);
inline ImVec2 g_movementSize = ImVec2(190.0f, 190.0f);
inline bool g_movementDragging = false;
inline ImVec2 g_movementDragOff = ImVec2(0.0f, 0.0f);
inline char g_textinput1[64] = "";
inline char g_configName[64] = "default";
inline int g_themeMask = (1 << 0);
inline ImVec4 g_accent1 = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
inline ImVec4 g_colFrame = ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f);
inline ImVec4 g_colGroup = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.0f);
inline ImVec4 g_colCtrl = ImVec4(5.0f / 255.0f, 5.0f / 255.0f, 5.0f / 255.0f, 1.0f);
inline ImVec4 g_colCtrlHov = ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f);
inline ImVec4 g_colBorder = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
inline ImVec4 g_colText = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
inline ImVec4 g_colTextDim = ImVec4(160.0f / 255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f);
inline const char* g_activeBindId = nullptr;
inline int g_bindStartFrame = 0;
inline bool g_inQuestionPopup = false;
inline bool g_modalAtFrameStart = false;
inline bool IsModalActive()
{
    if (g_inQuestionPopup) return false;
    if (g_modalAtFrameStart) return true;
    if (g_activeBindId != nullptr) return true;
    return ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
}
inline ImU32 AccentU32() { return ImGui::ColorConvertFloat4ToU32(g_accent1); }
inline ImU32 AccentDimU32()
{
    int r = (int)(g_accent1.x * 178.0f);
    int g = (int)(g_accent1.y * 178.0f);
    int b = (int)(g_accent1.z * 178.0f);
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return IM_COL32(r, g, b, 255);
}
inline ImU32 FrameU32() { return ImGui::ColorConvertFloat4ToU32(g_colFrame); }
inline ImU32 GroupU32() { return ImGui::ColorConvertFloat4ToU32(g_colGroup); }
inline ImU32 CtrlBgU32() { return ImGui::ColorConvertFloat4ToU32(g_colCtrl); }
inline ImU32 CtrlHovU32() { return ImGui::ColorConvertFloat4ToU32(g_colCtrlHov); }
inline ImU32 BorderU32() { return ImGui::ColorConvertFloat4ToU32(g_colBorder); }
inline ImU32 TextU32() { return ImGui::ColorConvertFloat4ToU32(g_colText); }
inline ImU32 TextDimU32() { return ImGui::ColorConvertFloat4ToU32(g_colTextDim); }
inline ImU32 BrightenU32(const ImVec4& c, float f)
{
    int r = (int)(c.x * 255.0f * f);
    int g = (int)(c.y * 255.0f * f);
    int b = (int)(c.z * 255.0f * f);
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return IM_COL32(r, g, b, 255);
}
inline ImU32 BorderHovU32() { return BrightenU32(g_colBorder, 1.2f); }
inline ImU32 FadeU32(ImU32 col, float mul)
{
    if (mul < 0.0f) mul = 0.0f;
    if (mul > 1.0f) mul = 1.0f;
    ImVec4 c = ImGui::ColorConvertU32ToFloat4(col);
    c.w *= mul;
    if (c.w < 0.0f) c.w = 0.0f;
    if (c.w > 1.0f) c.w = 1.0f;
    return ImGui::ColorConvertFloat4ToU32(c);
}
inline void DrawGlowLine(ImDrawList* dl, float x0, float x1, float y, ImU32 col)
{
    float xi0 = floorf(x0);
    float xi1 = floorf(x1);
    if (xi1 <= xi0 + 1.0f || !dl)
        return;
    ImVec4 c = ImGui::ColorConvertU32ToFloat4(col);
    ImU32 soft0 = ImGui::ColorConvertFloat4ToU32(ImVec4(c.x, c.y, c.z, 0.10f));
    ImU32 soft1 = ImGui::ColorConvertFloat4ToU32(ImVec4(c.x, c.y, c.z, 0.18f));
    dl->AddRectFilled(ImVec2(xi0, floorf(y - 2.0f)), ImVec2(xi1, floorf(y + 3.0f)), soft0);
    dl->AddRectFilled(ImVec2(xi0, floorf(y - 1.0f)), ImVec2(xi1, floorf(y + 2.0f)), soft1);
    const int segs = 24;
    float w = xi1 - xi0;
    float yc = floorf(y);
    for (int i = 0; i < segs; i++)
    {
        float t0 = (float)i / (float)segs;
        float t1 = (float)(i + 1) / (float)segs;
        float a0 = sinf(t0 * 3.14159265f);
        float a1 = sinf(t1 * 3.14159265f);
        float am = (a0 + a1) * 0.5f;
        am *= am;
        if (am < 0.02f)
            continue;
        dl->AddRectFilled(ImVec2(xi0 + t0 * w, yc), ImVec2(xi0 + t1 * w, yc + 1.0f), FadeU32(col, am));
    }
}
inline ImU32 AccentLightU32() { return BrightenU32(g_accent1, 1.18f); }
inline ImU32 AccentAlphaU32(int a)
{
    int r = (int)(g_accent1.x * 255.0f);
    int g = (int)(g_accent1.y * 255.0f);
    int b = (int)(g_accent1.z * 255.0f);
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    if (a < 0) a = 0; if (a > 255) a = 255;
    return IM_COL32(r, g, b, a);
}
inline void ResetThemeColors()
{
    g_accent1 = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    g_colFrame = ImVec4(10.0f / 255.0f, 10.0f / 255.0f, 10.0f / 255.0f, 1.0f);
    g_colGroup = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.0f);
    g_colCtrl = ImVec4(5.0f / 255.0f, 5.0f / 255.0f, 5.0f / 255.0f, 1.0f);
    g_colCtrlHov = ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f);
    g_colBorder = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f);
    g_colText = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f);
    g_colTextDim = ImVec4(160.0f / 255.0f, 160.0f / 255.0f, 160.0f / 255.0f, 1.0f);
}
inline void ApplyThemePalette(int sel)
{
    ResetThemeColors();
    if (sel == 1)
    {
        g_accent1 = ImVec4(56.0f / 255.0f, 189.0f / 255.0f, 248.0f / 255.0f, 1.0f);
        g_colFrame = ImVec4(40.0f / 255.0f, 48.0f / 255.0f, 64.0f / 255.0f, 1.0f);
        g_colGroup = ImVec4(50.0f / 255.0f, 60.0f / 255.0f, 80.0f / 255.0f, 1.0f);
        g_colCtrl = ImVec4(26.0f / 255.0f, 32.0f / 255.0f, 44.0f / 255.0f, 1.0f);
        g_colCtrlHov = ImVec4(36.0f / 255.0f, 44.0f / 255.0f, 60.0f / 255.0f, 1.0f);
        g_colBorder = ImVec4(95.0f / 255.0f, 120.0f / 255.0f, 160.0f / 255.0f, 1.0f);
        g_colText = ImVec4(235.0f / 255.0f, 240.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        g_colTextDim = ImVec4(175.0f / 255.0f, 175.0f / 255.0f, 180.0f / 255.0f, 1.0f);
    }
    else if (sel == 2)
    {
        g_accent1 = ImVec4(170.0f / 255.0f, 130.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        g_colFrame = ImVec4(48.0f / 255.0f, 40.0f / 255.0f, 64.0f / 255.0f, 1.0f);
        g_colGroup = ImVec4(60.0f / 255.0f, 50.0f / 255.0f, 80.0f / 255.0f, 1.0f);
        g_colCtrl = ImVec4(32.0f / 255.0f, 26.0f / 255.0f, 44.0f / 255.0f, 1.0f);
        g_colCtrlHov = ImVec4(44.0f / 255.0f, 36.0f / 255.0f, 60.0f / 255.0f, 1.0f);
        g_colBorder = ImVec4(120.0f / 255.0f, 95.0f / 255.0f, 160.0f / 255.0f, 1.0f);
        g_colText = ImVec4(240.0f / 255.0f, 235.0f / 255.0f, 255.0f / 255.0f, 1.0f);
        g_colTextDim = ImVec4(170.0f / 255.0f, 150.0f / 255.0f, 190.0f / 255.0f, 1.0f);
    }
    else if (sel == 3)
    {
        g_accent1 = ImVec4(90.0f / 255.0f, 210.0f / 255.0f, 140.0f / 255.0f, 1.0f);
        g_colFrame = ImVec4(40.0f / 255.0f, 60.0f / 255.0f, 50.0f / 255.0f, 1.0f);
        g_colGroup = ImVec4(50.0f / 255.0f, 78.0f / 255.0f, 62.0f / 255.0f, 1.0f);
        g_colCtrl = ImVec4(26.0f / 255.0f, 42.0f / 255.0f, 32.0f / 255.0f, 1.0f);
        g_colCtrlHov = ImVec4(36.0f / 255.0f, 58.0f / 255.0f, 44.0f / 255.0f, 1.0f);
        g_colBorder = ImVec4(95.0f / 255.0f, 160.0f / 255.0f, 125.0f / 255.0f, 1.0f);
        g_colText = ImVec4(235.0f / 255.0f, 250.0f / 255.0f, 240.0f / 255.0f, 1.0f);
        g_colTextDim = ImVec4(150.0f / 255.0f, 190.0f / 255.0f, 165.0f / 255.0f, 1.0f);
    }
    else if (sel == 4)
    {
        g_accent1 = ImVec4(240.0f / 255.0f, 115.0f / 255.0f, 125.0f / 255.0f, 1.0f);
        g_colFrame = ImVec4(64.0f / 255.0f, 40.0f / 255.0f, 44.0f / 255.0f, 1.0f);
        g_colGroup = ImVec4(80.0f / 255.0f, 50.0f / 255.0f, 56.0f / 255.0f, 1.0f);
        g_colCtrl = ImVec4(44.0f / 255.0f, 26.0f / 255.0f, 30.0f / 255.0f, 1.0f);
        g_colCtrlHov = ImVec4(60.0f / 255.0f, 36.0f / 255.0f, 40.0f / 255.0f, 1.0f);
        g_colBorder = ImVec4(160.0f / 255.0f, 95.0f / 255.0f, 105.0f / 255.0f, 1.0f);
        g_colText = ImVec4(255.0f / 255.0f, 235.0f / 255.0f, 238.0f / 255.0f, 1.0f);
        g_colTextDim = ImVec4(190.0f / 255.0f, 150.0f / 255.0f, 158.0f / 255.0f, 1.0f);
    }
    else if (sel == 5)
    {
        g_accent1 = ImVec4(245.0f / 255.0f, 175.0f / 255.0f, 90.0f / 255.0f, 1.0f);
        g_colFrame = ImVec4(62.0f / 255.0f, 50.0f / 255.0f, 38.0f / 255.0f, 1.0f);
        g_colGroup = ImVec4(80.0f / 255.0f, 62.0f / 255.0f, 48.0f / 255.0f, 1.0f);
        g_colCtrl = ImVec4(44.0f / 255.0f, 34.0f / 255.0f, 26.0f / 255.0f, 1.0f);
        g_colCtrlHov = ImVec4(60.0f / 255.0f, 46.0f / 255.0f, 36.0f / 255.0f, 1.0f);
        g_colBorder = ImVec4(160.0f / 255.0f, 130.0f / 255.0f, 95.0f / 255.0f, 1.0f);
        g_colText = ImVec4(255.0f / 255.0f, 242.0f / 255.0f, 230.0f / 255.0f, 1.0f);
        g_colTextDim = ImVec4(190.0f / 255.0f, 170.0f / 255.0f, 150.0f / 255.0f, 1.0f);
    }
}
inline float Clamp(float v, float mn, float mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }

inline void DrawPixelBox(ImDrawList* dl, ImVec2 min, ImVec2 max, ImU32 bg_col, ImU32 border_light, ImU32 border_dark = IM_COL32(0, 0, 0, 255))
{
    float x0 = floorf(min.x);
    float y0 = floorf(min.y);
    float x1 = floorf(max.x);
    float y1 = floorf(max.y);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), bg_col);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y0 + 1.0f), border_dark);
    dl->AddRectFilled(ImVec2(x0, y1 - 1.0f), ImVec2(x1, y1), border_dark);
    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + 1.0f, y1), border_dark);
    dl->AddRectFilled(ImVec2(x1 - 1.0f, y0), ImVec2(x1, y1), border_dark);

    dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y0 + 2.0f), border_light);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y1 - 2.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x0 + 2.0f, y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(x1 - 2.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);
}

inline void DrawOutlinedText(ImDrawList* dl, ImFont* font, float font_size, ImVec2 pos, ImU32 text_col, ImU32 outline_col, const char* text)
{
    ImVec2 p(floorf(pos.x), floorf(pos.y));
    dl->AddText(font, font_size, ImVec2(p.x - 1.0f, p.y), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x + 1.0f, p.y), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x, p.y - 1.0f), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x, p.y + 1.0f), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x - 1.0f, p.y - 1.0f), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x + 1.0f, p.y - 1.0f), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x - 1.0f, p.y + 1.0f), outline_col, text);
    dl->AddText(font, font_size, ImVec2(p.x + 1.0f, p.y + 1.0f), outline_col, text);
    dl->AddText(font, font_size, p, text_col, text);
}

inline void DrawOutlinedText(ImDrawList* dl, ImVec2 pos, ImU32 text_col, ImU32 outline_col, const char* text)
{
    DrawOutlinedText(dl, nullptr, 0.0f, pos, text_col, outline_col, text);
}

inline void DrawWaveText(ImDrawList* dl, ImFont* font, float font_size, ImVec2 pos, ImU32 text_col, ImU32 outline_col, const char* text)
{
    if (!text || text[0] == '\0') return;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : ImGui::GetFontSize();
    float time = (float)ImGui::GetTime() * 2.8f;

    char chars[64];
    ImVec2 pts[64];
    int count = 0;
    float cur_x = pos.x;

    for (int i = 0; text[i] != '\0' && count < 63; i++)
    {
        chars[count] = text[i];
        float y_off = sinf(time - (float)i * 0.65f) * 1.5f;
        pts[count] = ImVec2(floorf(cur_x), pos.y + y_off);
        char buf[2] = { text[i], '\0' };
        cur_x += f->CalcTextSizeA(fs, FLT_MAX, 0.0f, buf).x;
        count++;
    }

    for (int i = 0; i < count; i++)
    {
        char buf[2] = { chars[i], '\0' };
        ImVec2 p = pts[i];
        dl->AddText(f, fs, ImVec2(p.x - 1.0f, p.y), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x + 1.0f, p.y), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x, p.y - 1.0f), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x, p.y + 1.0f), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x - 1.0f, p.y - 1.0f), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x + 1.0f, p.y - 1.0f), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x - 1.0f, p.y + 1.0f), outline_col, buf);
        dl->AddText(f, fs, ImVec2(p.x + 1.0f, p.y + 1.0f), outline_col, buf);
    }

    for (int i = 0; i < count; i++)
    {
        char buf[2] = { chars[i], '\0' };
        dl->AddText(f, fs, pts[i], text_col, buf);
    }
}

inline void DrawWaveText(ImDrawList* dl, ImVec2 pos, ImU32 text_col, ImU32 outline_col, const char* text)
{
    DrawWaveText(dl, nullptr, 0.0f, pos, text_col, outline_col, text);
}

inline void DrawTabbedContainer(ImDrawList* dl, ImVec2 min, ImVec2 max, float tab_min_x, float tab_max_x, float tab_top_y, ImU32 bg_col, ImU32 border_light, ImU32 border_dark, ImU32 accent_col)
{
    float x0 = floorf(min.x);
    float y0 = floorf(min.y);
    float x1 = floorf(max.x);
    float y1 = floorf(max.y);
    float tx0 = floorf(tab_min_x);
    float tx1 = floorf(tab_max_x);
    float ty0 = floorf(tab_top_y);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), bg_col);
    dl->AddRectFilled(ImVec2(tx0, ty0), ImVec2(tx1, y0 + 2.0f), bg_col);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + 1.0f, y1), border_dark);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x0 + 2.0f, y1 - 1.0f), border_light);

    dl->AddRectFilled(ImVec2(x1 - 1.0f, y0), ImVec2(x1, y1), border_dark);
    dl->AddRectFilled(ImVec2(x1 - 2.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);

    dl->AddRectFilled(ImVec2(x0, y1 - 1.0f), ImVec2(x1, y1), border_dark);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y1 - 2.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);

    if (tx0 > x0)
    {
        dl->AddRectFilled(ImVec2(x0, y0), ImVec2(tx0 + 1.0f, y0 + 1.0f), border_dark);
        dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(tx0 + 1.0f, y0 + 2.0f), border_light);
    }
    if (tx1 < x1)
    {
        dl->AddRectFilled(ImVec2(tx1 - 1.0f, y0), ImVec2(x1, y0 + 1.0f), border_dark);
        dl->AddRectFilled(ImVec2(tx1 - 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y0 + 2.0f), border_light);
    }

    dl->AddRectFilled(ImVec2(tx0, ty0), ImVec2(tx0 + 1.0f, y0 + 1.0f), border_dark);
    dl->AddRectFilled(ImVec2(tx0 + 1.0f, ty0 + 1.0f), ImVec2(tx0 + 2.0f, y0 + 2.0f), border_light);

    dl->AddRectFilled(ImVec2(tx1 - 1.0f, ty0), ImVec2(tx1, y0 + 1.0f), border_dark);
    dl->AddRectFilled(ImVec2(tx1 - 2.0f, ty0 + 1.0f), ImVec2(tx1 - 1.0f, y0 + 2.0f), border_light);

    dl->AddRectFilled(ImVec2(tx0, ty0), ImVec2(tx1, ty0 + 1.0f), border_dark);
    dl->AddRectFilled(ImVec2(tx0 + 1.0f, ty0 + 1.0f), ImVec2(tx1 - 1.0f, ty0 + 2.0f), accent_col);
}

inline void DrawGroupBox(ImDrawList* dl, ImVec2 min, ImVec2 max, const char* title, ImFont* font, float font_size, ImU32 bg_col, ImU32 border_light, ImU32 border_dark = IM_COL32(0, 0, 0, 255), ImU32 text_col = TextU32())
{
    float x0 = floorf(min.x);
    float y0 = floorf(min.y);
    float x1 = floorf(max.x);
    float y1 = floorf(max.y);

    dl->AddRectFilled(ImVec2(x0, y0 + 1.0f), ImVec2(x1, y1), bg_col);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + 1.0f, y1), border_dark);
    dl->AddRectFilled(ImVec2(x1 - 1.0f, y0), ImVec2(x1, y1), border_dark);
    dl->AddRectFilled(ImVec2(x0, y1 - 1.0f), ImVec2(x1, y1), border_dark);

    dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x0 + 2.0f, y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(x1 - 2.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y1 - 2.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);

    if (title && title[0] != '\0')
    {
        ImFont* f = font ? font : ImGui::GetFont();
        float fs = (font_size > 0.0f) ? font_size : 13.0f;
        ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, title);

        float text_offset_x = 12.0f;
        float text_x = x0 + text_offset_x;
        float cut_start = text_x - 4.0f;
        float cut_end = text_x + text_sz.x + 4.0f;
        float text_y = floorf(y0 - text_sz.y * 0.5f);

        if (cut_start > x0)
        {
            dl->AddRectFilled(ImVec2(x0, y0), ImVec2(cut_start, y0 + 1.0f), border_dark);
            dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(cut_start, y0 + 2.0f), border_light);
        }
        if (cut_end < x1)
        {
            dl->AddRectFilled(ImVec2(cut_end, y0), ImVec2(x1, y0 + 1.0f), border_dark);
            dl->AddRectFilled(ImVec2(cut_end, y0 + 1.0f), ImVec2(x1 - 1.0f, y0 + 2.0f), border_light);
        }

        DrawOutlinedText(dl, f, fs, ImVec2(text_x, text_y), text_col, border_dark, title);
    }
    else
    {
        dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y0 + 1.0f), border_dark);
        dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y0 + 2.0f), border_light);
    }
}

inline constexpr float kCheckboxLabelGap = 3.0f;

inline constexpr float kLabelFontSize = 13.0f;
inline constexpr float kLabelBandH    = kLabelFontSize;

inline constexpr float kLabeledCtrlH = kLabelBandH + 14.0f;
inline constexpr float kLabeledBoxH  = kLabelBandH + 20.0f;

inline constexpr float kCheckRowH     = 14.0f;
inline constexpr float kRowGapCb      = 4.0f;
inline constexpr float kCheckRowPitch = kCheckRowH + kRowGapCb;

inline constexpr float kLabelTextRise = 2.0f;

inline bool DrawCheckbox(ImDrawList* dl, ImVec2 pos, bool* value, const char* label = nullptr, ImFont* font = nullptr, float font_size = 13.0f, bool ignore_modal = false)
{
    float x0 = floorf(pos.x);
    float y0 = floorf(pos.y);
    float w = 14.0f;
    float h = 14.0f;
    float x1 = x0 + w;
    float y1 = y0 + h;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;

    float total_w = w;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;
    ImVec2 text_sz(0.0f, 0.0f);
    if (label && label[0] != '\0')
    {
        text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, label);
        total_w += kCheckboxLabelGap + text_sz.x;
    }

    bool is_hov = (m.x >= x0 && m.x <= x0 + total_w && m.y >= y0 && m.y <= y1);
    if (!ignore_modal && IsModalActive()) is_hov = false;
    if (is_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0))
        {
            *value = !*value;
        }
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), border_dark);

    if (*value)
    {
        dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), AccentU32());
        dl->AddRectFilled(ImVec2(x0 + 2.0f, y0 + 2.0f), ImVec2(x1 - 2.0f, y1 - 2.0f), AccentLightU32());
    }
    else
    {
        dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_light);
        dl->AddRectFilled(ImVec2(x0 + 2.0f, y0 + 2.0f), ImVec2(x1 - 2.0f, y1 - 2.0f), CtrlBgU32());
    }

    if (label && label[0] != '\0')
    {
        float text_x = x1 + kCheckboxLabelGap;
        float text_y = floorf(y0 + (h - text_sz.y) * 0.5f);
        ImU32 col_label = *value ? TextU32() : TextDimU32();
        DrawOutlinedText(dl, f, fs, ImVec2(text_x, text_y), col_label, border_dark, label);
    }

    return is_hov;
}

inline int HexVal(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

inline bool DrawColorPicker(const char* id, ImDrawList* dl, ImVec2 pos, ImVec2 size, ImVec4* col, bool* outline_opt = nullptr, ImVec4* outline_col = nullptr)
{
    bool out_res = false;
    if (outline_col != nullptr && outline_opt != nullptr && *outline_opt)
    {
        char out_id[64];
        snprintf(out_id, sizeof(out_id), "%s_out", id ? id : "##cp");
        float out_x = pos.x - (size.x + 6.0f);
        float out_y = pos.y;
        out_res = DrawColorPicker(out_id, dl, ImVec2(out_x, out_y), size, outline_col, outline_opt, nullptr);
    }
    float x0 = floorf(pos.x);
    float y0 = floorf(pos.y);
    float x1 = floorf(x0 + size.x);
    float y1 = floorf(y0 + size.y);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;

    char ctx_popup_id[64];
    snprintf(ctx_popup_id, sizeof(ctx_popup_id), "%s##cp_ctx", id);

    bool is_hov = (m.x >= x0 && m.x <= x1 && m.y >= y0 && m.y <= y1);
    bool own_open_cp = ImGui::IsPopupOpen(id) || ImGui::IsPopupOpen(ctx_popup_id);
    if (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup) && !own_open_cp && !g_inQuestionPopup)
        is_hov = false;
    if (is_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0))
        {
            ImGui::OpenPopup(id);
        }
        else if (ImGui::IsMouseClicked(1))
        {
            ImGui::OpenPopup(ctx_popup_id);
        }
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();
    float inline_factor = is_hov ? 0.90f : 0.75f;
    int inline_floor = is_hov ? 40 : 30;
    int in_r = (int)(col->x * 255.0f * inline_factor);
    int in_g = (int)(col->y * 255.0f * inline_factor);
    int in_b = (int)(col->z * 255.0f * inline_factor);
    if (in_r < inline_floor) in_r = inline_floor; if (in_r > 255) in_r = 255;
    if (in_g < inline_floor) in_g = inline_floor; if (in_g > 255) in_g = 255;
    if (in_b < inline_floor) in_b = inline_floor; if (in_b > 255) in_b = 255;
    const ImU32 border_inline = IM_COL32(in_r, in_g, in_b, 255);
    ImU32 col_u32 = ImGui::ColorConvertFloat4ToU32(*col);

    dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), border_dark);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, y0 + 1.0f), ImVec2(x1 - 1.0f, y1 - 1.0f), border_inline);
    dl->AddRectFilled(ImVec2(x0 + 2.0f, y0 + 2.0f), ImVec2(x1 - 2.0f, y1 - 2.0f), col_u32);

    const float sv_w = 160.0f;
    const float sv_h = 160.0f;
    const float bar_h = 10.0f;
    const float pad = 8.0f;
    const float margin = 8.0f;

    float popup_w = sv_w + margin * 2.0f;
    float popup_h = sv_h + pad + bar_h + pad + bar_h + margin * 2.0f;

    ImVec2 popup_pos(floorf(x1 - popup_w), floorf(y1 + 4.0f));
    ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(popup_w, popup_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(30.0f / 255.0f, 36.0f / 255.0f, 48.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(margin, margin));

    if (ImGui::BeginPopup(id, ImGuiWindowFlags_NoMove))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + popup_w, w_min.y + popup_h);

        DrawPixelBox(pdl, w_min, w_max, CtrlBgU32(), border_light, border_dark);

        ImGui::Dummy(ImVec2(sv_w, sv_h + pad + bar_h + pad + bar_h));

        float sv_x0 = w_min.x + margin;
        float sv_y0 = w_min.y + margin;
        float sv_x1 = sv_x0 + sv_w;
        float sv_y1 = sv_y0 + sv_h;

        float hue_y0 = sv_y1 + pad;
        float hue_y1 = hue_y0 + bar_h;

        float alpha_y0 = hue_y1 + pad;
        float alpha_y1 = alpha_y0 + bar_h;

        float h = 0.0f, s = 0.0f, v = 0.0f;
        ImGui::ColorConvertRGBtoHSV(col->x, col->y, col->z, h, s, v);
        static std::unordered_map<std::string, float> s_savedH;
        std::string hkey = id ? id : "##cp";
        auto hIt = s_savedH.find(hkey);
        if (hIt == s_savedH.end())
        {
            s_savedH[hkey] = h;
            hIt = s_savedH.find(hkey);
        }
        if (s > 0.001f)
        {
            hIt->second = h;
        }
        else
        {
            h = hIt->second;
        }

        static std::string s_activePid;
        static int s_activeHandle = 0;
        if (ImGui::IsMouseClicked(0))
        {
            if (m.x >= sv_x0 && m.x <= sv_x1 && m.y >= sv_y0 && m.y <= sv_y1)
            {
                s_activePid = id ? id : "";
                s_activeHandle = 1;
            }
            else if (m.x >= sv_x0 && m.x <= sv_x1 && m.y >= hue_y0 && m.y <= hue_y1)
            {
                s_activePid = id ? id : "";
                s_activeHandle = 2;
            }
            else if (m.x >= sv_x0 && m.x <= sv_x1 && m.y >= alpha_y0 && m.y <= alpha_y1)
            {
                s_activePid = id ? id : "";
                s_activeHandle = 3;
            }
        }
        if (!ImGui::IsMouseDown(0))
        {
            s_activePid.clear();
            s_activeHandle = 0;
        }
        bool own_drag = (!s_activePid.empty() && id != nullptr && s_activePid == id);
        int active_drag = own_drag ? s_activeHandle : 0;

        if (active_drag == 1)
        {
            s = Clamp((m.x - sv_x0) / sv_w, 0.0f, 1.0f);
            v = Clamp(1.0f - (m.y - sv_y0) / sv_h, 0.0f, 1.0f);
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
            col->x = r;
            col->y = g;
            col->z = b;
        }
        else if (active_drag == 2)
        {
            h = Clamp((m.x - sv_x0) / sv_w, 0.0f, 0.999f);
            hIt->second = h;
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
            col->x = r;
            col->y = g;
            col->z = b;
        }
        else if (active_drag == 3)
        {
            col->w = Clamp((m.x - sv_x0) / sv_w, 0.0f, 1.0f);
        }

        float pure_r, pure_g, pure_b;
        ImGui::ColorConvertHSVtoRGB(h, 1.0f, 1.0f, pure_r, pure_g, pure_b);
        ImU32 col_pure_hue = ImGui::ColorConvertFloat4ToU32(ImVec4(pure_r, pure_g, pure_b, 1.0f));
        ImU32 col_white = IM_COL32(255, 255, 255, 255);
        pdl->AddRectFilledMultiColor(ImVec2(sv_x0, sv_y0), ImVec2(sv_x1, sv_y1), col_white, col_pure_hue, col_pure_hue, col_white);
        pdl->AddRectFilledMultiColor(ImVec2(sv_x0, sv_y0), ImVec2(sv_x1, sv_y1), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255));
        pdl->AddRectFilled(ImVec2(sv_x0, sv_y0), ImVec2(sv_x1, sv_y0 + 1.0f), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, sv_y1 - 1.0f), ImVec2(sv_x1, sv_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, sv_y0), ImVec2(sv_x0 + 1.0f, sv_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x1 - 1.0f, sv_y0), ImVec2(sv_x1, sv_y1), border_dark);

        float cur_x = Clamp(sv_x0 + s * sv_w, sv_x0, sv_x1);
        float cur_y = Clamp(sv_y0 + (1.0f - v) * sv_h, sv_y0, sv_y1);
        pdl->AddCircle(ImVec2(cur_x, cur_y), 4.5f, IM_COL32(0, 0, 0, 255), 16, 2.5f);
        pdl->AddCircle(ImVec2(cur_x, cur_y), 4.5f, IM_COL32(255, 255, 255, 255), 16, 1.5f);

        float seg_w = sv_w / 6.0f;
        for (int i = 0; i < 6; i++)
        {
            float x_a = floorf(sv_x0 + (float)i * seg_w);
            float x_b = (i == 5) ? sv_x1 : floorf(sv_x0 + (float)(i + 1) * seg_w);
            float r0, g0, b0, r1, g1, b1;
            ImGui::ColorConvertHSVtoRGB((float)i / 6.0f, 1.0f, 1.0f, r0, g0, b0);
            ImGui::ColorConvertHSVtoRGB((float)(i + 1) / 6.0f, 1.0f, 1.0f, r1, g1, b1);
            ImU32 c0 = ImGui::ColorConvertFloat4ToU32(ImVec4(r0, g0, b0, 1.0f));
            ImU32 c1 = ImGui::ColorConvertFloat4ToU32(ImVec4(r1, g1, b1, 1.0f));
            pdl->AddRectFilledMultiColor(ImVec2(x_a, hue_y0), ImVec2(x_b, hue_y1), c0, c1, c1, c0);
        }
        pdl->AddRectFilled(ImVec2(sv_x0, hue_y0), ImVec2(sv_x1, hue_y0 + 1.0f), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, hue_y1 - 1.0f), ImVec2(sv_x1, hue_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, hue_y0), ImVec2(sv_x0 + 1.0f, hue_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x1 - 1.0f, hue_y0), ImVec2(sv_x1, hue_y1), border_dark);

        float hue_cur_x = Clamp(sv_x0 + h * sv_w, sv_x0, sv_x1);
        float hue_cur_y = floorf((hue_y0 + hue_y1) * 0.5f);
        pdl->AddCircle(ImVec2(hue_cur_x, hue_cur_y), 4.5f, IM_COL32(0, 0, 0, 255), 16, 2.5f);
        pdl->AddCircle(ImVec2(hue_cur_x, hue_cur_y), 4.5f, IM_COL32(255, 255, 255, 255), 16, 1.5f);

        pdl->AddRectFilled(ImVec2(sv_x0, alpha_y0), ImVec2(sv_x1, alpha_y1), IM_COL32(20, 24, 32, 255));
        ImU32 c_a0 = ImGui::ColorConvertFloat4ToU32(ImVec4(col->x, col->y, col->z, 0.0f));
        ImU32 c_a1 = ImGui::ColorConvertFloat4ToU32(ImVec4(col->x, col->y, col->z, 1.0f));
        pdl->AddRectFilledMultiColor(ImVec2(sv_x0, alpha_y0), ImVec2(sv_x1, alpha_y1), c_a0, c_a1, c_a1, c_a0);
        pdl->AddRectFilled(ImVec2(sv_x0, alpha_y0), ImVec2(sv_x1, alpha_y0 + 1.0f), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, alpha_y1 - 1.0f), ImVec2(sv_x1, alpha_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x0, alpha_y0), ImVec2(sv_x0 + 1.0f, alpha_y1), border_dark);
        pdl->AddRectFilled(ImVec2(sv_x1 - 1.0f, alpha_y0), ImVec2(sv_x1, alpha_y1), border_dark);

        float alpha_cur_x = Clamp(sv_x0 + col->w * sv_w, sv_x0, sv_x1);
        float alpha_cur_y = floorf((alpha_y0 + alpha_y1) * 0.5f);
        pdl->AddCircle(ImVec2(alpha_cur_x, alpha_cur_y), 4.5f, IM_COL32(0, 0, 0, 255), 16, 2.5f);
        pdl->AddCircle(ImVec2(alpha_cur_x, alpha_cur_y), 4.5f, IM_COL32(255, 255, 255, 255), 16, 1.5f);

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    const float ctx_w = (outline_opt != nullptr) ? 96.0f : 85.0f;
    const float ctx_item_h = 18.0f;
    const float ctx_pad_y = 2.0f;
    int ctx_count = (outline_opt != nullptr) ? 3 : 2;
    float ctx_h = ctx_pad_y * 2.0f + ctx_item_h * (float)ctx_count;

    ImVec2 ctx_pos(floorf(x0), floorf(y1 + 4.0f));
    ImVec2 win_pos = ImGui::GetWindowPos();
    ImVec2 win_sz = ImGui::GetWindowSize();
    if (ctx_pos.x + ctx_w > win_pos.x + win_sz.x - 10.0f)
    {
        ctx_pos.x = floorf(x1 - ctx_w);
    }
    if (ctx_pos.y + ctx_h > win_pos.y + win_sz.y - 10.0f)
    {
        ctx_pos.y = floorf(y0 - ctx_h - 4.0f);
    }

    ImGui::SetNextWindowPos(ctx_pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(ctx_w, ctx_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(30.0f / 255.0f, 36.0f / 255.0f, 48.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginPopup(ctx_popup_id, ImGuiWindowFlags_NoMove))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + ctx_w, w_min.y + ctx_h);

        DrawPixelBox(pdl, w_min, w_max, CtrlBgU32(), border_light, border_dark);
        ImGui::Dummy(ImVec2(ctx_w, ctx_h));

        const char* ctx_items[] = { "Copy hex", "Paste hex" };
        ImFont* f = g_fontSmall ? g_fontSmall : ImGui::GetFont();
        float fs = 13.0f;

        for (int i = 0; i < ctx_count; i++)
        {
            float it_y0 = floorf(w_min.y + ctx_pad_y + (float)i * ctx_item_h);
            float it_y1 = it_y0 + ctx_item_h;
            float it_x0 = w_min.x + 2.0f;
            float it_x1 = w_max.x - 2.0f;

            bool it_hov = (m.x >= it_x0 && m.x <= it_x1 && m.y >= it_y0 && m.y <= it_y1);
            if (it_hov)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), CtrlHovU32());
                if (ImGui::IsMouseClicked(0))
                {
                    if (i == 0)
                    {
                        float cr = Clamp(col->x, 0.0f, 1.0f);
                        float cg = Clamp(col->y, 0.0f, 1.0f);
                        float cb = Clamp(col->z, 0.0f, 1.0f);
                        float ca = Clamp(col->w, 0.0f, 1.0f);
                        int ir = (int)(cr * 255.0f + 0.5f);
                        int ig = (int)(cg * 255.0f + 0.5f);
                        int ib = (int)(cb * 255.0f + 0.5f);
                        int ia = (int)(ca * 255.0f + 0.5f);
                        char hex_buf[16];
                        if (ia < 255)
                        {
                            snprintf(hex_buf, sizeof(hex_buf), "#%02X%02X%02X%02X", ir, ig, ib, ia);
                        }
                        else
                        {
                            snprintf(hex_buf, sizeof(hex_buf), "#%02X%02X%02X", ir, ig, ib);
                        }
                        ImGui::SetClipboardText(hex_buf);
                    }
                    else if (i == 1)
                    {
                        const char* clip = ImGui::GetClipboardText();
                        if (clip)
                        {
                            while (*clip == ' ' || *clip == '\t' || *clip == '\r' || *clip == '\n') clip++;
                            if (*clip == '#') clip++;
                            else if (clip[0] == '0' && (clip[1] == 'x' || clip[1] == 'X')) clip += 2;
                            int hlen = 0;
                            while (HexVal(clip[hlen]) >= 0) hlen++;
                            if (hlen == 6)
                            {
                                int r = (HexVal(clip[0]) << 4) | HexVal(clip[1]);
                                int g = (HexVal(clip[2]) << 4) | HexVal(clip[3]);
                                int b = (HexVal(clip[4]) << 4) | HexVal(clip[5]);
                                col->x = (float)r / 255.0f;
                                col->y = (float)g / 255.0f;
                                col->z = (float)b / 255.0f;
                                col->w = 1.0f;
                            }
                            else if (hlen == 8)
                            {
                                int r = (HexVal(clip[0]) << 4) | HexVal(clip[1]);
                                int g = (HexVal(clip[2]) << 4) | HexVal(clip[3]);
                                int b = (HexVal(clip[4]) << 4) | HexVal(clip[5]);
                                int a = (HexVal(clip[6]) << 4) | HexVal(clip[7]);
                                col->x = (float)r / 255.0f;
                                col->y = (float)g / 255.0f;
                                col->z = (float)b / 255.0f;
                                col->w = (float)a / 255.0f;
                            }
                            else if (hlen == 3)
                            {
                                int r = HexVal(clip[0]) * 17;
                                int g = HexVal(clip[1]) * 17;
                                int b = HexVal(clip[2]) * 17;
                                col->x = (float)r / 255.0f;
                                col->y = (float)g / 255.0f;
                                col->z = (float)b / 255.0f;
                                col->w = 1.0f;
                            }
                        }
                    }
                    ImGui::CloseCurrentPopup();
                }
                else if (i == 2 && outline_opt)
                {
                    *outline_opt = !*outline_opt;
                    if (outline_col == nullptr && !*outline_opt)
                    {
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            if (i < 2)
            {
                ImU32 it_text_col = it_hov ? TextU32() : TextDimU32();
                ImVec2 it_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, ctx_items[i]);
                float it_tx = floorf(w_min.x + (ctx_w - it_sz.x) * 0.5f);
                float it_ty = floorf(it_y0 + (ctx_item_h - it_sz.y) * 0.5f);
                DrawOutlinedText(pdl, f, fs, ImVec2(it_tx, it_ty), it_text_col, border_dark, ctx_items[i]);
            }
            else if (i == 2 && outline_opt)
            {
                float cb_sz = 10.0f;
                float cb_x0 = it_x0 + 6.0f;
                float cb_y0 = floorf(it_y0 + (ctx_item_h - cb_sz) * 0.5f);
                float cb_x1 = cb_x0 + cb_sz;
                float cb_y1 = cb_y0 + cb_sz;
                pdl->AddRectFilled(ImVec2(cb_x0, cb_y0), ImVec2(cb_x1, cb_y1), border_dark);
                if (*outline_opt)
                {
                    pdl->AddRectFilled(ImVec2(cb_x0 + 1.0f, cb_y0 + 1.0f), ImVec2(cb_x1 - 1.0f, cb_y1 - 1.0f), AccentDimU32());
                    pdl->AddRectFilled(ImVec2(cb_x0 + 2.0f, cb_y0 + 2.0f), ImVec2(cb_x1 - 2.0f, cb_y1 - 2.0f), border_light);
                }
                else
                {
                    pdl->AddRectFilled(ImVec2(cb_x0 + 1.0f, cb_y0 + 1.0f), ImVec2(cb_x1 - 1.0f, cb_y1 - 1.0f), border_light);
                    pdl->AddRectFilled(ImVec2(cb_x0 + 2.0f, cb_y0 + 2.0f), ImVec2(cb_x1 - 2.0f, cb_y1 - 2.0f), CtrlBgU32());
                }
                ImVec2 t_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, "Outline");
                float tx = cb_x1 + 6.0f;
                float ty = floorf(it_y0 + (ctx_item_h - t_sz.y) * 0.5f);
                ImU32 it_text_col = *outline_opt ? TextU32() : TextDimU32();
                DrawOutlinedText(pdl, f, fs, ImVec2(tx, ty), it_text_col, border_dark, "Outline");
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    return is_hov || own_open_cp || out_res;
}

inline const char* GetKeyDisplayName(int key)
{
    if (key == ImGuiKey_MouseLeft) return "M1";
    if (key == ImGuiKey_MouseRight) return "M2";
    if (key == ImGuiKey_MouseMiddle) return "M3";
    if (key == ImGuiKey_MouseX1) return "X1";
    if (key == ImGuiKey_MouseX2) return "X2";
    if (key == ImGuiKey_LeftShift) return "LShift";
    if (key == ImGuiKey_RightShift) return "RShift";
    if (key == ImGuiKey_LeftCtrl) return "LCtrl";
    if (key == ImGuiKey_RightCtrl) return "RCtrl";
    if (key == ImGuiKey_LeftAlt) return "LAlt";
    if (key == ImGuiKey_RightAlt) return "RAlt";
    const char* name = ImGui::GetKeyName((ImGuiKey)key);
    return name ? name : "None";
}

inline bool DrawKeybind(const char* id, ImDrawList* dl, ImVec2 right_edge, int* key, int* mode, ImFont* font = nullptr, float font_size = 13.0f)
{
    if (!g_menuOpen)
    {
        g_activeBindId = nullptr;
    }

    bool is_binding = (g_activeBindId != nullptr && (g_activeBindId == id || strcmp(g_activeBindId, id) == 0));

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;

    if (is_binding && ImGui::GetFrameCount() > g_bindStartFrame)
    {
        bool kb_any_popup = !g_inQuestionPopup && ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
        if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Delete))
        {
            *key = 0;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else if (!kb_any_popup && ImGui::IsMouseClicked(0))
        {
            *key = ImGuiKey_MouseLeft;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else if (!kb_any_popup && ImGui::IsMouseClicked(1))
        {
            *key = ImGuiKey_MouseRight;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else if (!kb_any_popup && ImGui::IsMouseClicked(2))
        {
            *key = ImGuiKey_MouseMiddle;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else if (!kb_any_popup && ImGui::IsMouseClicked(3))
        {
            *key = ImGuiKey_MouseX1;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else if (!kb_any_popup && ImGui::IsMouseClicked(4))
        {
            *key = ImGuiKey_MouseX2;
            g_activeBindId = nullptr;
            is_binding = false;
        }
        else
        {
            for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; k++)
            {
                if (k == ImGuiKey_ReservedForModCtrl || k == ImGuiKey_ReservedForModShift ||
                    k == ImGuiKey_ReservedForModAlt || k == ImGuiKey_ReservedForModSuper ||
                    k == ImGuiKey_MouseWheelX || k == ImGuiKey_MouseWheelY)
                {
                    continue;
                }

                if (ImGui::IsKeyPressed((ImGuiKey)k))
                {
                    *key = k;
                    g_activeBindId = nullptr;
                    is_binding = false;
                    break;
                }
            }
        }
    }

    char display_buf[64];
    if (is_binding)
    {
        snprintf(display_buf, sizeof(display_buf), "[ ]");
    }
    else if (*key == 0)
    {
        snprintf(display_buf, sizeof(display_buf), "[ . . . ]");
    }
    else
    {
        snprintf(display_buf, sizeof(display_buf), "[ %s ]", GetKeyDisplayName(*key));
    }

    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;
    ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, display_buf);

    float right_x = floorf(right_edge.x);
    float text_x = floorf(right_x - text_sz.x);
    float y0 = floorf(right_edge.y);
    float text_y = floorf(y0 + (14.0f - text_sz.y) * 0.5f);

    float hit_x0 = text_x - 3.0f;
    float hit_x1 = right_x + 3.0f;
    float hit_y0 = y0 - 2.0f;
    float hit_y1 = y0 + 16.0f;

    bool is_hov = (m.x >= hit_x0 && m.x <= hit_x1 && m.y >= hit_y0 && m.y <= hit_y1);

    char popup_id[64];
    snprintf(popup_id, sizeof(popup_id), "##kb_pop_%s", id);

    bool kb_own_popup = ImGui::IsPopupOpen(popup_id);
    if (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup) && !kb_own_popup && !g_inQuestionPopup)
        is_hov = false;

    if (is_hov && !is_binding)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0))
        {
            g_activeBindId = id;
            g_bindStartFrame = ImGui::GetFrameCount();
            is_binding = true;
            snprintf(display_buf, sizeof(display_buf), "[ ]");
            text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, display_buf);
            text_x = floorf(right_x - text_sz.x);
        }
        else if (ImGui::IsMouseClicked(1))
        {
            ImGui::OpenPopup(popup_id);
        }
    }

    const ImU32 border_dark = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    ImU32 text_col;
    if (is_binding)
    {
        text_col = AccentU32();
    }
    else if (*key != 0)
    {
        text_col = TextU32();
    }
    else
    {
        text_col = TextDimU32();
    }

    DrawOutlinedText(dl, f, fs, ImVec2(text_x, text_y), text_col, border_dark, display_buf);

    const float popup_w = 75.0f;
    const float item_h = 18.0f;
    const float pad_y = 2.0f;
    const float popup_h = pad_y * 2.0f + item_h * 3.0f;

    ImVec2 popup_pos(floorf(right_x - popup_w), floorf(y0 + 18.0f));
    ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(popup_w, popup_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(30.0f / 255.0f, 36.0f / 255.0f, 48.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoMove))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + popup_w, w_min.y + popup_h);

        DrawPixelBox(pdl, w_min, w_max, CtrlBgU32(), border_light, border_dark);
        ImGui::Dummy(ImVec2(popup_w, popup_h));

        const char* modes[] = { "Always", "Toggle", "Hold" };
        for (int i = 0; i < 3; i++)
        {
            float it_y0 = floorf(w_min.y + pad_y + (float)i * item_h);
            float it_y1 = it_y0 + item_h;
            float it_x0 = w_min.x + 2.0f;
            float it_x1 = w_max.x - 2.0f;

            bool it_hov = (m.x >= it_x0 && m.x <= it_x1 && m.y >= it_y0 && m.y <= it_y1);
            if (it_hov)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), CtrlHovU32());
                if (ImGui::IsMouseClicked(0))
                {
                    *mode = i;
                    ImGui::CloseCurrentPopup();
                }
            }

            bool is_selected = (*mode == i);
            ImU32 it_text_col = is_selected ? AccentU32() : TextDimU32();
            ImVec2 t_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, modes[i]);
            float tx = it_x0 + 8.0f;
            float ty = floorf(it_y0 + (item_h - t_sz.y) * 0.5f);
            DrawOutlinedText(pdl, f, fs, ImVec2(tx, ty), it_text_col, border_dark, modes[i]);
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    bool popup_open = ImGui::IsPopupOpen(popup_id);
    return is_hov || popup_open || is_binding;
}

inline bool DrawSlider(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* val, int min_val, int max_val, const char* label = nullptr, const char* format = "%d", ImFont* font = nullptr, float font_size = 13.0f, const ImVec4* fillCol = nullptr)
{
    static const char* s_active_slider_id = nullptr;
    static float s_repeat_timer = 0.0f;

    if (!g_menuOpen && !g_settingsOpen && !g_themeOpen)
    {
        s_active_slider_id = nullptr;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float x1 = floorf(pos.x + width);

    bool has_label = (label && label[0] != '\0');
    float bar_y0 = has_label ? floorf(pos.y + kLabelBandH) : floorf(pos.y);
    float bar_y1 = bar_y0 + 14.0f;

    float btn_minus_x0 = x0;
    float btn_minus_x1 = x0 + 14.0f;
    float btn_plus_x0 = x1 - 14.0f;
    float btn_plus_x1 = x1;
    float bar_x0 = btn_minus_x1 + 4.0f;
    float bar_x1 = btn_plus_x0 - 4.0f;

    bool is_minus_hov = (m.x >= btn_minus_x0 && m.x <= btn_minus_x1 && m.y >= bar_y0 && m.y <= bar_y1);
    bool is_plus_hov  = (m.x >= btn_plus_x0 && m.x <= btn_plus_x1 && m.y >= bar_y0 && m.y <= bar_y1);
    bool is_bar_hov   = (m.x >= bar_x0 && m.x <= bar_x1 && m.y >= bar_y0 && m.y <= bar_y1);

    bool slider_modal = IsModalActive();
    if (slider_modal)
    {
        s_active_slider_id = nullptr;
        is_minus_hov = false;
        is_plus_hov = false;
        is_bar_hov = false;
    }

    if (is_minus_hov || is_plus_hov || is_bar_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (is_minus_hov && ImGui::IsMouseDown(0))
    {
        if (ImGui::IsMouseClicked(0))
        {
            if (*val > min_val)
            {
                *val -= 1;
            }
            s_repeat_timer = 0.0f;
        }
        else if (io.MouseDownDuration[0] > 0.35f)
        {
            s_repeat_timer += io.DeltaTime;
            if (s_repeat_timer >= 0.05f)
            {
                if (*val > min_val)
                {
                    *val -= 1;
                }
                s_repeat_timer = 0.0f;
            }
        }
    }
    else if (is_plus_hov && ImGui::IsMouseDown(0))
    {
        if (ImGui::IsMouseClicked(0))
        {
            if (*val < max_val)
            {
                *val += 1;
            }
            s_repeat_timer = 0.0f;
        }
        else if (io.MouseDownDuration[0] > 0.35f)
        {
            s_repeat_timer += io.DeltaTime;
            if (s_repeat_timer >= 0.05f)
            {
                if (*val < max_val)
                {
                    *val += 1;
                }
                s_repeat_timer = 0.0f;
            }
        }
    }
    else if (!ImGui::IsMouseDown(0))
    {
        s_repeat_timer = 0.0f;
    }

    if (is_bar_hov && ImGui::IsMouseClicked(0))
    {
        s_active_slider_id = id;
    }

    bool bar_active = (s_active_slider_id != nullptr && (s_active_slider_id == id || strcmp(s_active_slider_id, id) == 0));
    float inner_w = (bar_x1 - 2.0f) - (bar_x0 + 2.0f);
    if (bar_active)
    {
        if (ImGui::IsMouseDown(0))
        {
            float frac = (m.x - (bar_x0 + 2.0f)) / inner_w;
            frac = Clamp(frac, 0.0f, 1.0f);
            *val = min_val + (int)floorf(frac * (float)(max_val - min_val) + 0.5f);
            *val = (*val < min_val) ? min_val : (*val > max_val) ? max_val : *val;
        }
        else
        {
            s_active_slider_id = nullptr;
            bar_active = false;
        }
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    char val_buf[32];
    snprintf(val_buf, sizeof(val_buf), format ? format : "%d", *val);
    ImVec2 val_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, val_buf);

    if (has_label)
    {
        float text_y = floorf(pos.y - kLabelTextRise);
        ImU32 col_lbl = TextU32();
        DrawOutlinedText(dl, f, fs, ImVec2(x0, text_y), col_lbl, border_dark, label);
    }

    dl->AddRectFilled(ImVec2(btn_minus_x0, bar_y0), ImVec2(btn_minus_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(btn_minus_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(btn_minus_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(btn_minus_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(btn_minus_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());
    ImVec2 minus_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, "-");
    float mx = floorf(btn_minus_x0 + (14.0f - minus_sz.x) * 0.5f);
    float my = floorf(bar_y0 + (14.0f - minus_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(mx, my), TextDimU32(), border_dark, "-");

    dl->AddRectFilled(ImVec2(btn_plus_x0, bar_y0), ImVec2(btn_plus_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(btn_plus_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(btn_plus_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(btn_plus_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(btn_plus_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());
    ImVec2 plus_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, "+");
    float px = floorf(btn_plus_x0 + (14.0f - plus_sz.x) * 0.5f);
    float py = floorf(bar_y0 + (14.0f - plus_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(px, py), TextDimU32(), border_dark, "+");

    dl->AddRectFilled(ImVec2(bar_x0, bar_y0), ImVec2(bar_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(bar_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(bar_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(bar_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(bar_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());

    float frac = (max_val > min_val) ? ((float)(*val - min_val) / (float)(max_val - min_val)) : 0.0f;
    frac = Clamp(frac, 0.0f, 1.0f);

    static ImGuiStorage s_slider_storage;
    ImGuiID sid = ImGui::GetID(id);
    float current_anim = s_slider_storage.GetFloat(sid, -1.0f);
    if (current_anim < 0.0f)
    {
        current_anim = frac;
    }
    else
    {
        float dt = io.DeltaTime;
        if (dt > 0.05f) dt = 0.05f;
        float speed = 14.0f;
        current_anim += (frac - current_anim) * (1.0f - expf(-speed * dt));
        if (fabsf(frac - current_anim) < 0.0005f)
        {
            current_anim = frac;
        }
    }
    current_anim = Clamp(current_anim, 0.0f, 1.0f);
    s_slider_storage.SetFloat(sid, current_anim);

    float fill_w = floorf(inner_w * current_anim);

    if (fill_w > 0.0f)
    {
        float fill_x1 = bar_x0 + 2.0f + fill_w;
        if (fill_x1 > bar_x1 - 1.0f)
        {
            fill_x1 = bar_x1 - 1.0f;
        }
        ImU32 fillLow = fillCol ? BrightenU32(*fillCol, 0.7f) : AccentDimU32();
        ImU32 fillDark = fillCol ? BrightenU32(*fillCol, 0.55f) : BrightenU32(g_accent1, 0.55f);
        dl->AddRectFilled(ImVec2(bar_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(fill_x1, bar_y1 - 1.0f), fillLow);
        dl->AddRectFilled(ImVec2(bar_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(fill_x1, bar_y1 - 2.0f), fillDark);

        if (fill_x1 < bar_x1 - 2.0f)
        {
            dl->AddRectFilled(ImVec2(fill_x1, bar_y0 + 1.0f), ImVec2(fill_x1 + 1.0f, bar_y1 - 1.0f), border_dark);
        }
    }

    float vx = floorf(bar_x0 + (bar_x1 - bar_x0 - val_sz.x) * 0.5f);
    float vy = floorf(bar_y0 + (14.0f - val_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(vx, vy), TextU32(), border_dark, val_buf);

    float total_h = has_label ? kLabeledCtrlH : 14.0f;
    bool is_total_hov = (m.x >= x0 && m.x <= x1 && m.y >= pos.y - kLabelTextRise && m.y <= pos.y + total_h);
    if (slider_modal) return false;
    return is_total_hov || bar_active || is_minus_hov || is_plus_hov;
}

inline bool DrawSliderFloat(const char* id, ImDrawList* dl, ImVec2 pos, float width, float* val, float min_val, float max_val, const char* label = nullptr, const char* format = "%.1f", ImFont* font = nullptr, float font_size = 13.0f, const ImVec4* fillCol = nullptr)
{
    int range = 1000;
    int ival = (int)(((Clamp(*val, min_val, max_val) - min_val) / ((max_val > min_val) ? (max_val - min_val) : 1.0f)) * (float)range + 0.5f);
    char fmt[32];
    snprintf(fmt, sizeof(fmt), "%s", format ? format : "%.1f");
    int imin = 0, imax = range;
    char numbuf[32];
    snprintf(numbuf, sizeof(numbuf), fmt, (double)*val);
    bool hov = DrawSlider(id, dl, pos, width, &ival, imin, imax, label, numbuf, font, font_size, fillCol);
    float frac = Clamp((float)ival / (float)range, 0.0f, 1.0f);
    *val = min_val + frac * (max_val - min_val);
    return hov;
}

inline bool DrawRangeSlider(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* val_min, int* val_max, int min_val, int max_val, const char* label = nullptr, const char* format = "%d - %d", ImFont* font = nullptr, float font_size = 13.0f, const ImVec4* fillCol = nullptr)
{
    static const char* s_active_rslider_id = nullptr;
    static int s_active_rslider_handle = 0;
    static float s_repeat_timer = 0.0f;

    if (!g_menuOpen && !g_settingsOpen && !g_themeOpen)
    {
        s_active_rslider_id = nullptr;
        s_active_rslider_handle = 0;
    }

    if (*val_min < min_val) *val_min = min_val;
    if (*val_min > max_val) *val_min = max_val;
    if (*val_max < *val_min) *val_max = *val_min;
    if (*val_max > max_val) *val_max = max_val;

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float x1 = floorf(pos.x + width);

    bool has_label = (label && label[0] != '\0');
    float bar_y0 = has_label ? floorf(pos.y + kLabelBandH) : floorf(pos.y);
    float bar_y1 = bar_y0 + 14.0f;

    float btn_minus_x0 = x0;
    float btn_minus_x1 = x0 + 14.0f;
    float btn_plus_x0 = x1 - 14.0f;
    float btn_plus_x1 = x1;
    float bar_x0 = btn_minus_x1 + 4.0f;
    float bar_x1 = btn_plus_x0 - 4.0f;

    bool is_minus_hov = (m.x >= btn_minus_x0 && m.x <= btn_minus_x1 && m.y >= bar_y0 && m.y <= bar_y1);
    bool is_plus_hov  = (m.x >= btn_plus_x0 && m.x <= btn_plus_x1 && m.y >= bar_y0 && m.y <= bar_y1);
    bool is_bar_hov   = (m.x >= bar_x0 && m.x <= bar_x1 && m.y >= bar_y0 && m.y <= bar_y1);

    bool slider_modal = IsModalActive();
    if (slider_modal)
    {
        s_active_rslider_id = nullptr;
        s_active_rslider_handle = 0;
        is_minus_hov = false;
        is_plus_hov = false;
        is_bar_hov = false;
    }

    if (is_minus_hov || is_plus_hov || is_bar_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (is_minus_hov && ImGui::IsMouseDown(0))
    {
        if (ImGui::IsMouseClicked(0))
        {
            if (*val_min > min_val) *val_min -= 1;
            s_repeat_timer = 0.0f;
        }
        else if (io.MouseDownDuration[0] > 0.35f)
        {
            s_repeat_timer += io.DeltaTime;
            if (s_repeat_timer >= 0.05f)
            {
                if (*val_min > min_val) *val_min -= 1;
                s_repeat_timer = 0.0f;
            }
        }
    }
    else if (is_minus_hov && ImGui::IsMouseClicked(1))
    {
        if (*val_min < *val_max) *val_min += 1;
    }
    else if (is_plus_hov && ImGui::IsMouseDown(0))
    {
        if (ImGui::IsMouseClicked(0))
        {
            if (*val_max < max_val) *val_max += 1;
            s_repeat_timer = 0.0f;
        }
        else if (io.MouseDownDuration[0] > 0.35f)
        {
            s_repeat_timer += io.DeltaTime;
            if (s_repeat_timer >= 0.05f)
            {
                if (*val_max < max_val) *val_max += 1;
                s_repeat_timer = 0.0f;
            }
        }
    }
    else if (is_plus_hov && ImGui::IsMouseClicked(1))
    {
        if (*val_max > *val_min) *val_max -= 1;
    }
    else if (!ImGui::IsMouseDown(0))
    {
        s_repeat_timer = 0.0f;
    }

    float inner_w = (bar_x1 - 2.0f) - (bar_x0 + 2.0f);
    float range_span = (max_val > min_val) ? (float)(max_val - min_val) : 1.0f;
    float frac_min = Clamp((float)(*val_min - min_val) / range_span, 0.0f, 1.0f);
    float frac_max = Clamp((float)(*val_max - min_val) / range_span, 0.0f, 1.0f);

    if (is_bar_hov && ImGui::IsMouseClicked(0))
    {
        s_active_rslider_id = id;
        float click_frac = Clamp((m.x - (bar_x0 + 2.0f)) / inner_w, 0.0f, 1.0f);
        float d_min = fabsf(click_frac - frac_min);
        float d_max = fabsf(click_frac - frac_max);
        if (d_min < d_max)
        {
            s_active_rslider_handle = 1;
            *val_min = min_val + (int)floorf(click_frac * range_span + 0.5f);
            if (*val_min > *val_max) *val_min = *val_max;
            if (*val_min < min_val) *val_min = min_val;
        }
        else if (d_max < d_min)
        {
            s_active_rslider_handle = 2;
            *val_max = min_val + (int)floorf(click_frac * range_span + 0.5f);
            if (*val_max < *val_min) *val_max = *val_min;
            if (*val_max > max_val) *val_max = max_val;
        }
        else
        {
            if (click_frac >= frac_max)
            {
                s_active_rslider_handle = 2;
                *val_max = min_val + (int)floorf(click_frac * range_span + 0.5f);
            }
            else
            {
                s_active_rslider_handle = 1;
                *val_min = min_val + (int)floorf(click_frac * range_span + 0.5f);
            }
        }
    }

    bool bar_active = (s_active_rslider_id != nullptr && (s_active_rslider_id == id || strcmp(s_active_rslider_id, id) == 0));
    if (bar_active)
    {
        if (ImGui::IsMouseDown(0))
        {
            float drag_frac = Clamp((m.x - (bar_x0 + 2.0f)) / inner_w, 0.0f, 1.0f);
            int new_val = min_val + (int)floorf(drag_frac * range_span + 0.5f);
            if (s_active_rslider_handle == 1)
            {
                if (new_val > *val_max) new_val = *val_max;
                if (new_val < min_val) new_val = min_val;
                *val_min = new_val;
            }
            else if (s_active_rslider_handle == 2)
            {
                if (new_val < *val_min) new_val = *val_min;
                if (new_val > max_val) new_val = max_val;
                *val_max = new_val;
            }
        }
        else
        {
            s_active_rslider_id = nullptr;
            s_active_rslider_handle = 0;
            bar_active = false;
        }
    }

    if (is_bar_hov && io.MouseWheel != 0.0f)
    {
        float mid_x = bar_x0 + 2.0f + inner_w * (frac_min + frac_max) * 0.5f;
        int wheel_delta = (io.MouseWheel > 0.0f) ? 1 : -1;
        if (m.x < mid_x)
        {
            *val_min = (int)Clamp((float)(*val_min + wheel_delta), (float)min_val, (float)*val_max);
        }
        else
        {
            *val_max = (int)Clamp((float)(*val_max + wheel_delta), (float)*val_min, (float)max_val);
        }
    }

    frac_min = Clamp((float)(*val_min - min_val) / range_span, 0.0f, 1.0f);
    frac_max = Clamp((float)(*val_max - min_val) / range_span, 0.0f, 1.0f);

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    char val_buf[32];
    snprintf(val_buf, sizeof(val_buf), format ? format : "%d - %d", *val_min, *val_max);
    ImVec2 val_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, val_buf);

    if (has_label)
    {
        float text_y = floorf(pos.y - kLabelTextRise);
        ImU32 col_lbl = TextU32();
        DrawOutlinedText(dl, f, fs, ImVec2(x0, text_y), col_lbl, border_dark, label);
    }

    dl->AddRectFilled(ImVec2(btn_minus_x0, bar_y0), ImVec2(btn_minus_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(btn_minus_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(btn_minus_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(btn_minus_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(btn_minus_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());
    ImVec2 minus_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, "-");
    float mx = floorf(btn_minus_x0 + (14.0f - minus_sz.x) * 0.5f);
    float my = floorf(bar_y0 + (14.0f - minus_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(mx, my), TextDimU32(), border_dark, "-");

    dl->AddRectFilled(ImVec2(btn_plus_x0, bar_y0), ImVec2(btn_plus_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(btn_plus_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(btn_plus_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(btn_plus_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(btn_plus_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());
    ImVec2 plus_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, "+");
    float px = floorf(btn_plus_x0 + (14.0f - plus_sz.x) * 0.5f);
    float py = floorf(bar_y0 + (14.0f - plus_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(px, py), TextDimU32(), border_dark, "+");

    dl->AddRectFilled(ImVec2(bar_x0, bar_y0), ImVec2(bar_x1, bar_y1), border_dark);
    dl->AddRectFilled(ImVec2(bar_x0 + 1.0f, bar_y0 + 1.0f), ImVec2(bar_x1 - 1.0f, bar_y1 - 1.0f), border_light);
    dl->AddRectFilled(ImVec2(bar_x0 + 2.0f, bar_y0 + 2.0f), ImVec2(bar_x1 - 2.0f, bar_y1 - 2.0f), CtrlBgU32());

    static ImGuiStorage s_rslider_storage;
    ImGuiID sid_min = ImGui::GetID(id);
    ImGuiID sid_max = sid_min + 1;

    float cur_anim_min = s_rslider_storage.GetFloat(sid_min, -1.0f);
    float cur_anim_max = s_rslider_storage.GetFloat(sid_max, -1.0f);
    if (cur_anim_min < 0.0f) cur_anim_min = frac_min;
    if (cur_anim_max < 0.0f) cur_anim_max = frac_max;

    float dt = io.DeltaTime;
    if (dt > 0.05f) dt = 0.05f;
    float speed = 14.0f;
    cur_anim_min += (frac_min - cur_anim_min) * (1.0f - expf(-speed * dt));
    cur_anim_max += (frac_max - cur_anim_max) * (1.0f - expf(-speed * dt));
    if (fabsf(frac_min - cur_anim_min) < 0.0005f) cur_anim_min = frac_min;
    if (fabsf(frac_max - cur_anim_max) < 0.0005f) cur_anim_max = frac_max;

    cur_anim_min = Clamp(cur_anim_min, 0.0f, 1.0f);
    cur_anim_max = Clamp(cur_anim_max, 0.0f, 1.0f);
    if (cur_anim_min > cur_anim_max) cur_anim_min = cur_anim_max;

    s_rslider_storage.SetFloat(sid_min, cur_anim_min);
    s_rslider_storage.SetFloat(sid_max, cur_anim_max);

    float fill_x0 = floorf(bar_x0 + 2.0f + inner_w * cur_anim_min);
    float fill_x1 = floorf(bar_x0 + 2.0f + inner_w * cur_anim_max);
    if (fill_x0 < bar_x0 + 2.0f) fill_x0 = bar_x0 + 2.0f;
    if (fill_x1 > bar_x1 - 1.0f) fill_x1 = bar_x1 - 1.0f;
    if (fill_x0 > fill_x1) fill_x0 = fill_x1;

    ImU32 fillLow = fillCol ? BrightenU32(*fillCol, 0.7f) : AccentDimU32();
    ImU32 fillDark = fillCol ? BrightenU32(*fillCol, 0.55f) : BrightenU32(g_accent1, 0.55f);

    if (fill_x1 > fill_x0)
    {
        dl->AddRectFilled(ImVec2(fill_x0, bar_y0 + 1.0f), ImVec2(fill_x1, bar_y1 - 1.0f), fillLow);
        dl->AddRectFilled(ImVec2(fill_x0, bar_y0 + 2.0f), ImVec2(fill_x1, bar_y1 - 2.0f), fillDark);
    }

    if (fill_x0 > bar_x0 + 2.0f)
    {
        dl->AddRectFilled(ImVec2(fill_x0 - 1.0f, bar_y0 + 1.0f), ImVec2(fill_x0, bar_y1 - 1.0f), border_dark);
    }
    if (fill_x1 < bar_x1 - 2.0f)
    {
        dl->AddRectFilled(ImVec2(fill_x1, bar_y0 + 1.0f), ImVec2(fill_x1 + 1.0f, bar_y1 - 1.0f), border_dark);
    }

    float vx = floorf(bar_x0 + (bar_x1 - bar_x0 - val_sz.x) * 0.5f);
    float vy = floorf(bar_y0 + (14.0f - val_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, fs, ImVec2(vx, vy), TextU32(), border_dark, val_buf);

    float total_h = has_label ? kLabeledCtrlH : 14.0f;
    bool is_total_hov = (m.x >= x0 && m.x <= x1 && m.y >= pos.y - kLabelTextRise && m.y <= pos.y + total_h);
    if (slider_modal) return false;
    return is_total_hov || bar_active || is_minus_hov || is_plus_hov;
}

inline bool DrawMinMaxSlider(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* val_min, int* val_max, int min_val, int max_val, const char* label = nullptr, const char* format = "%d - %d", ImFont* font = nullptr, float font_size = 13.0f, const ImVec4* fillCol = nullptr)
{
    return DrawRangeSlider(id, dl, pos, width, val_min, val_max, min_val, max_val, label, format, font, font_size, fillCol);
}

inline bool DrawButton(ImDrawList* dl, ImVec2 pos, float width, float height, const char* text, ImFont* font = nullptr, float font_size = 13.0f, bool* out_hovered = nullptr)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float y0 = floorf(pos.y);
    float x1 = floorf(x0 + width);
    float y1 = floorf(y0 + height);

    bool is_hov = (m.x >= x0 && m.x <= x1 && m.y >= y0 && m.y <= y1);
    bool is_down = is_hov && ImGui::IsMouseDown(0);
    bool is_clicked = is_hov && ImGui::IsMouseClicked(0);

    if (IsModalActive())
    {
        is_hov = false;
        is_down = false;
        is_clicked = false;
    }

    if (is_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    ImU32 bg_col = CtrlBgU32();
    ImU32 light_col = border_light;
    DrawPixelBox(dl, ImVec2(x0, y0), ImVec2(x1, y1), bg_col, light_col, border_dark);

    ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, text);
    float tx = floorf(x0 + (width - text_sz.x) * 0.5f);
    float ty = floorf(y0 + (height - text_sz.y) * 0.5f + (is_down ? 1.0f : 0.0f));
    DrawOutlinedText(dl, f, fs, ImVec2(tx, ty), TextDimU32(), border_dark, text);

    if (out_hovered) *out_hovered = is_hov;
    return is_clicked;
}

inline bool DrawTextInput(const char* id, ImDrawList* dl, ImVec2 pos, float width, char* buf, int buf_size, const char* label = nullptr, const char* hint = nullptr, ImFont* font = nullptr, float font_size = 13.0f)
{
    static const char* s_active_input_id = nullptr;
    static int s_cursor_pos = 0;
    static int s_sel_start = -1;
    static int s_sel_end = -1;

    if (!g_menuOpen)
    {
        s_active_input_id = nullptr;
        s_sel_start = -1;
        s_sel_end = -1;
    }

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float x1 = floorf(pos.x + width);

    bool has_label = (label && label[0] != '\0');
    float box_y0 = has_label ? floorf(pos.y + kLabelBandH) : floorf(pos.y);
    float box_h = 20.0f;
    float box_y1 = box_y0 + box_h;

    bool is_active = (s_active_input_id != nullptr && (s_active_input_id == id || strcmp(s_active_input_id, id) == 0));
    bool raw_box_hov = (m.x >= x0 && m.x <= x1 && m.y >= box_y0 && m.y <= box_y1);
    bool text_modal = IsModalActive();
    bool is_box_hov = raw_box_hov && !text_modal;

    if (is_box_hov && !is_active)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
    }
    if (is_box_hov && is_active)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
    }

    if (ImGui::IsMouseClicked(0))
    {
        if (raw_box_hov && !text_modal)
        {
            s_active_input_id = id;
            s_cursor_pos = (int)strlen(buf);
            s_sel_start = -1;
            s_sel_end = -1;
        }
        else if (is_active && !raw_box_hov)
        {
            s_active_input_id = nullptr;
            is_active = false;
            s_sel_start = -1;
            s_sel_end = -1;
        }
    }

    bool has_sel = (is_active && s_sel_start >= 0 && s_sel_end >= 0 && s_sel_start != s_sel_end);
    int sel_min = 0;
    int sel_max = 0;
    if (has_sel)
    {
        sel_min = s_sel_start < s_sel_end ? s_sel_start : s_sel_end;
        sel_max = s_sel_start > s_sel_end ? s_sel_start : s_sel_end;
    }

    if (is_active && text_modal)
    {
        io.InputQueueCharacters.resize(0);
    }

    if (is_active && !text_modal)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            s_active_input_id = nullptr;
            is_active = false;
            s_sel_start = -1;
            s_sel_end = -1;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            s_active_input_id = nullptr;
            is_active = false;
            s_sel_start = -1;
            s_sel_end = -1;
        }
        else
        {
            int len = (int)strlen(buf);
            bool ctrl = io.KeyCtrl;
            bool shift = io.KeyShift;

            if (ctrl && ImGui::IsKeyPressed(ImGuiKey_A))
            {
                s_sel_start = 0;
                s_sel_end = len;
                s_cursor_pos = len;
            }
            else if (ctrl && ImGui::IsKeyPressed(ImGuiKey_C))
            {
                if (has_sel)
                {
                    char clip[256];
                    int clip_len = sel_max - sel_min;
                    if (clip_len > (int)sizeof(clip) - 1)
                    {
                        clip_len = (int)sizeof(clip) - 1;
                    }
                    memcpy(clip, buf + sel_min, (size_t)clip_len);
                    clip[clip_len] = '\0';
                    ImGui::SetClipboardText(clip);
                }
            }
            else if (ctrl && ImGui::IsKeyPressed(ImGuiKey_X))
            {
                if (has_sel)
                {
                    char clip[256];
                    int clip_len = sel_max - sel_min;
                    if (clip_len > (int)sizeof(clip) - 1)
                    {
                        clip_len = (int)sizeof(clip) - 1;
                    }
                    memcpy(clip, buf + sel_min, (size_t)clip_len);
                    clip[clip_len] = '\0';
                    ImGui::SetClipboardText(clip);

                    for (int j = sel_min; j <= len - (sel_max - sel_min); j++)
                    {
                        buf[j] = buf[j + (sel_max - sel_min)];
                    }
                    s_cursor_pos = sel_min;
                    s_sel_start = -1;
                    s_sel_end = -1;
                    has_sel = false;
                    len = (int)strlen(buf);
                }
            }
            else if (ctrl && ImGui::IsKeyPressed(ImGuiKey_V))
            {
                const char* clip = ImGui::GetClipboardText();
                if (clip && clip[0] != '\0')
                {
                    if (has_sel)
                    {
                        for (int j = sel_min; j <= len - (sel_max - sel_min); j++)
                        {
                            buf[j] = buf[j + (sel_max - sel_min)];
                        }
                        s_cursor_pos = sel_min;
                        s_sel_start = -1;
                        s_sel_end = -1;
                        has_sel = false;
                        len = (int)strlen(buf);
                    }

                    int clip_len = (int)strlen(clip);
                    int avail = buf_size - 1 - len;
                    if (clip_len > avail)
                    {
                        clip_len = avail;
                    }
                    if (clip_len > 0)
                    {
                        for (int j = len; j >= s_cursor_pos; j--)
                        {
                            buf[j + clip_len] = buf[j];
                        }
                        for (int j = 0; j < clip_len; j++)
                        {
                            unsigned char ch = (unsigned char)clip[j];
                            if (ch < 32 || ch >= 127)
                            {
                                ch = '?';
                            }
                            buf[s_cursor_pos + j] = (char)ch;
                        }
                        s_cursor_pos += clip_len;
                        len = (int)strlen(buf);
                    }
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Backspace))
            {
                if (has_sel)
                {
                    for (int j = sel_min; j <= len - (sel_max - sel_min); j++)
                    {
                        buf[j] = buf[j + (sel_max - sel_min)];
                    }
                    s_cursor_pos = sel_min;
                    s_sel_start = -1;
                    s_sel_end = -1;
                    has_sel = false;
                }
                else if (s_cursor_pos > 0 && len > 0)
                {
                    for (int j = s_cursor_pos - 1; j < len; j++)
                    {
                        buf[j] = buf[j + 1];
                    }
                    s_cursor_pos--;
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Delete))
            {
                if (has_sel)
                {
                    for (int j = sel_min; j <= len - (sel_max - sel_min); j++)
                    {
                        buf[j] = buf[j + (sel_max - sel_min)];
                    }
                    s_cursor_pos = sel_min;
                    s_sel_start = -1;
                    s_sel_end = -1;
                    has_sel = false;
                }
                else if (s_cursor_pos < len)
                {
                    for (int j = s_cursor_pos; j < len; j++)
                    {
                        buf[j] = buf[j + 1];
                    }
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
            {
                if (has_sel && !shift)
                {
                    s_cursor_pos = sel_min;
                    s_sel_start = -1;
                    s_sel_end = -1;
                }
                else
                {
                    if (shift && s_sel_start < 0)
                    {
                        s_sel_start = s_cursor_pos;
                        s_sel_end = s_cursor_pos;
                    }
                    if (s_cursor_pos > 0)
                    {
                        s_cursor_pos--;
                    }
                    if (shift)
                    {
                        s_sel_end = s_cursor_pos;
                    }
                    else
                    {
                        s_sel_start = -1;
                        s_sel_end = -1;
                    }
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
            {
                if (has_sel && !shift)
                {
                    s_cursor_pos = sel_max;
                    s_sel_start = -1;
                    s_sel_end = -1;
                }
                else
                {
                    if (shift && s_sel_start < 0)
                    {
                        s_sel_start = s_cursor_pos;
                        s_sel_end = s_cursor_pos;
                    }
                    if (s_cursor_pos < len)
                    {
                        s_cursor_pos++;
                    }
                    if (shift)
                    {
                        s_sel_end = s_cursor_pos;
                    }
                    else
                    {
                        s_sel_start = -1;
                        s_sel_end = -1;
                    }
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_Home))
            {
                if (shift && s_sel_start < 0)
                {
                    s_sel_start = s_cursor_pos;
                    s_sel_end = s_cursor_pos;
                }
                s_cursor_pos = 0;
                if (shift)
                {
                    s_sel_end = s_cursor_pos;
                }
                else
                {
                    s_sel_start = -1;
                    s_sel_end = -1;
                }
            }
            else if (ImGui::IsKeyPressed(ImGuiKey_End))
            {
                if (shift && s_sel_start < 0)
                {
                    s_sel_start = s_cursor_pos;
                    s_sel_end = s_cursor_pos;
                }
                s_cursor_pos = len;
                if (shift)
                {
                    s_sel_end = s_cursor_pos;
                }
                else
                {
                    s_sel_start = -1;
                    s_sel_end = -1;
                }
            }

            if (io.InputQueueCharacters.Size > 0 && !ctrl)
            {
                if (has_sel)
                {
                    for (int j = sel_min; j <= len - (sel_max - sel_min); j++)
                    {
                        buf[j] = buf[j + (sel_max - sel_min)];
                    }
                    s_cursor_pos = sel_min;
                    s_sel_start = -1;
                    s_sel_end = -1;
                    has_sel = false;
                    len = (int)strlen(buf);
                }

                for (int n = 0; n < io.InputQueueCharacters.Size; n++)
                {
                    unsigned int c = (unsigned int)io.InputQueueCharacters[n];
                    if (c >= 32 && c < 127 && len < buf_size - 1)
                    {
                        for (int j = len; j >= s_cursor_pos; j--)
                        {
                            buf[j + 1] = buf[j];
                        }
                        buf[s_cursor_pos] = (char)c;
                        s_cursor_pos++;
                        len++;
                    }
                }
                io.InputQueueCharacters.resize(0);
            }
            else if (ctrl)
            {
                io.InputQueueCharacters.resize(0);
            }

            len = (int)strlen(buf);
            if (s_cursor_pos > len)
            {
                s_cursor_pos = len;
            }
        }
    }

    has_sel = (is_active && s_sel_start >= 0 && s_sel_end >= 0 && s_sel_start != s_sel_end);
    if (has_sel)
    {
        sel_min = s_sel_start < s_sel_end ? s_sel_start : s_sel_end;
        sel_max = s_sel_start > s_sel_end ? s_sel_start : s_sel_end;
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    if (has_label)
    {
        float text_y = floorf(pos.y - kLabelTextRise);
        ImU32 col_lbl = (is_active || is_box_hov) ? TextU32() : TextU32();
        DrawOutlinedText(dl, f, fs, ImVec2(x0, text_y), col_lbl, border_dark, label);
    }

    dl->AddRectFilled(ImVec2(x0, box_y0), ImVec2(x1, box_y1), border_dark);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, box_y0 + 1.0f), ImVec2(x1 - 1.0f, box_y1 - 1.0f), is_active ? BorderHovU32() : border_light);
    dl->AddRectFilled(ImVec2(x0 + 2.0f, box_y0 + 2.0f), ImVec2(x1 - 2.0f, box_y1 - 2.0f), CtrlBgU32());

    float text_pad = 6.0f;
    float text_area_x0 = x0 + text_pad;
    float text_area_x1 = x1 - text_pad;
    float max_text_w = text_area_x1 - text_area_x0;

    int len = (int)strlen(buf);
    bool is_empty = (len == 0);

    if (is_empty && !is_active && hint && hint[0] != '\0')
    {
        ImVec2 hint_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, hint);
        float hy = floorf(box_y0 + (box_h - hint_sz.y) * 0.5f);
        DrawOutlinedText(dl, f, fs, ImVec2(text_area_x0, hy), TextDimU32(), border_dark, hint);
    }
    else
    {
        ImVec2 full_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, buf);

        float scroll_off = 0.0f;
        if (is_active)
        {
            char tmp[128];
            int cp = s_cursor_pos < (int)sizeof(tmp) - 1 ? s_cursor_pos : (int)sizeof(tmp) - 1;
            memcpy(tmp, buf, (size_t)cp);
            tmp[cp] = '\0';
            ImVec2 cursor_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, tmp);
            if (cursor_sz.x > max_text_w)
            {
                scroll_off = cursor_sz.x - max_text_w + 2.0f;
            }
        }

        ImVec4 clip_rect(text_area_x0, box_y0 + 2.0f, text_area_x1, box_y1 - 2.0f);
        dl->PushClipRect(ImVec2(clip_rect.x, clip_rect.y), ImVec2(clip_rect.z, clip_rect.w), true);

        float draw_x = text_area_x0 - scroll_off;
        float draw_y = floorf(box_y0 + (box_h - full_sz.y) * 0.5f);

        if (has_sel)
        {
            char tmp_min[128];
            int cm = sel_min < (int)sizeof(tmp_min) - 1 ? sel_min : (int)sizeof(tmp_min) - 1;
            memcpy(tmp_min, buf, (size_t)cm);
            tmp_min[cm] = '\0';
            ImVec2 sm_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, tmp_min);

            char tmp_max[128];
            int cx = sel_max < (int)sizeof(tmp_max) - 1 ? sel_max : (int)sizeof(tmp_max) - 1;
            memcpy(tmp_max, buf, (size_t)cx);
            tmp_max[cx] = '\0';
            ImVec2 sx_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, tmp_max);

            float s_x0 = floorf(draw_x + sm_sz.x);
            float s_x1 = floorf(draw_x + sx_sz.x);
            dl->AddRectFilled(ImVec2(s_x0, box_y0 + 3.0f), ImVec2(s_x1, box_y1 - 3.0f), AccentAlphaU32(180));
        }

        ImU32 text_col = is_active ? TextU32() : TextU32();
        DrawOutlinedText(dl, f, fs, ImVec2(draw_x, draw_y), text_col, border_dark, buf);

        if (is_active)
        {
            char tmp[128];
            int cp = s_cursor_pos < (int)sizeof(tmp) - 1 ? s_cursor_pos : (int)sizeof(tmp) - 1;
            memcpy(tmp, buf, (size_t)cp);
            tmp[cp] = '\0';
            ImVec2 cursor_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, tmp);
            float cursor_x = floorf(draw_x + cursor_sz.x);

            bool blink = (fmodf((float)ImGui::GetTime() * 2.0f, 2.0f) < 1.2f);
            if (blink)
            {
                dl->AddRectFilled(ImVec2(cursor_x, box_y0 + 4.0f), ImVec2(cursor_x + 1.0f, box_y1 - 4.0f), TextU32());
            }
        }

        dl->PopClipRect();
    }

    float total_h = has_label ? kLabeledBoxH : 20.0f;
    bool is_total_hov = (m.x >= x0 && m.x <= x1 && m.y >= pos.y - kLabelTextRise && m.y <= pos.y + total_h);
    if (text_modal) return false;
    return is_total_hov || is_active;
}

inline bool DrawDropdown(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* selected_mask, const char* const items[], int items_count, const char* label = nullptr, ImFont* font = nullptr, float font_size = 13.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float x1 = floorf(pos.x + width);

    bool has_label = (label && label[0] != '\0');
    float box_y0 = has_label ? floorf(pos.y + kLabelBandH) : floorf(pos.y);
    float box_h = 20.0f;
    float box_y1 = box_y0 + box_h;

    bool raw_dd_hov = (m.x >= x0 && m.x <= x1 && m.y >= box_y0 && m.y <= box_y1);

    char popup_id[64];
    snprintf(popup_id, sizeof(popup_id), "##pop_%s", id);

    bool popup_open = ImGui::IsPopupOpen(popup_id);
    bool dd_modal = ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup) && !popup_open && !g_inQuestionPopup;
    bool is_box_hov = raw_dd_hov && !dd_modal;
    if (is_box_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (is_box_hov && ImGui::IsMouseClicked(0) && !popup_open)
    {
        ImGui::OpenPopup(popup_id);
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);

    if (has_label)
    {
        float text_y = floorf(pos.y - kLabelTextRise);
        ImU32 col_lbl = popup_open ? TextU32() : TextDimU32();
        DrawOutlinedText(dl, f, fs, ImVec2(x0, text_y), col_lbl, border_dark, label);
    }

    DrawPixelBox(dl, ImVec2(x0, box_y0), ImVec2(x1, box_y1),
        popup_open ? CtrlHovU32() : CtrlBgU32(), BorderU32(), border_dark);

    char display_buf[128] = "";
    int count = 0;
    int first_idx = -1;
    int second_idx = -1;
    int third_idx = -1;
    for (int i = 0; i < items_count; i++)
    {
        if (*selected_mask & (1 << i))
        {
            count++;
            if (count == 1)
            {
                first_idx = i;
            }
            else if (count == 2)
            {
                second_idx = i;
            }
            else if (count == 3)
            {
                third_idx = i;
            }
        }
    }
    if (count == 0)
    {
        snprintf(display_buf, sizeof(display_buf), "-");
    }
    else if (count == 1)
    {
        snprintf(display_buf, sizeof(display_buf), "%s", items[first_idx]);
    }
    else if (count == 2)
    {
        snprintf(display_buf, sizeof(display_buf), "%s, %s", items[first_idx], items[second_idx]);
    }
    else if (count == 3)
    {
        snprintf(display_buf, sizeof(display_buf), "%s, %s, %s", items[first_idx], items[second_idx], items[third_idx]);
    }
    else
    {
        snprintf(display_buf, sizeof(display_buf), "%s, %s, %s, ...", items[first_idx], items[second_idx], items[third_idx]);
    }

    ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, display_buf);
    float tx = x0 + 8.0f;
    float ty = floorf(box_y0 + (box_h - text_sz.y) * 0.5f);
    dl->PushClipRect(ImVec2(tx, box_y0 + 2.0f), ImVec2(x1 - 18.0f, box_y1 - 2.0f), true);
    DrawOutlinedText(dl, f, fs, ImVec2(tx, ty), popup_open ? TextU32() : TextDimU32(), border_dark, display_buf);
    dl->PopClipRect();

    float ax = floorf(x1 - 10.0f);
    float ay = floorf(box_y0 + (box_h - 3.0f) * 0.5f);
    ImU32 arrow_col = popup_open ? TextU32() : TextDimU32();
    dl->AddRectFilled(ImVec2(ax - 2.0f, ay), ImVec2(ax + 3.0f, ay + 1.0f), arrow_col);
    dl->AddRectFilled(ImVec2(ax - 1.0f, ay + 1.0f), ImVec2(ax + 2.0f, ay + 2.0f), arrow_col);
    dl->AddRectFilled(ImVec2(ax, ay + 2.0f), ImVec2(ax + 1.0f, ay + 3.0f), arrow_col);

    float item_h = 18.0f;
    float pad_y = 2.0f;
    float popup_h = pad_y * 2.0f + (float)items_count * item_h;
    float popup_w = width;

    ImVec2 popup_pos(x0, floorf(box_y1 + 2.0f));
    ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(popup_w, popup_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(30.0f / 255.0f, 36.0f / 255.0f, 48.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoMove))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + popup_w, w_min.y + popup_h);

        DrawPixelBox(pdl, w_min, w_max, CtrlBgU32(), BorderU32(), border_dark);
        ImGui::Dummy(ImVec2(popup_w, popup_h));

        for (int i = 0; i < items_count; i++)
        {
            float it_y0 = floorf(w_min.y + pad_y + (float)i * item_h);
            float it_y1 = it_y0 + item_h;
            float it_x0 = w_min.x + 2.0f;
            float it_x1 = w_max.x - 2.0f;

            bool is_selected = ((*selected_mask & (1 << i)) != 0);
            bool it_hov = (m.x >= it_x0 && m.x <= it_x1 && m.y >= it_y0 && m.y <= it_y1);
            if (it_hov)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), is_selected ? CtrlHovU32() : CtrlBgU32());
                if (ImGui::IsMouseClicked(0))
                {
                    *selected_mask ^= (1 << i);
                }
            }
            else if (is_selected)
            {
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), CtrlHovU32());
            }

            ImU32 it_text_col = (is_selected || it_hov) ? TextU32() : TextDimU32();
            ImVec2 it_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, items[i]);
            float it_tx = it_x0 + 8.0f;
            float it_ty = floorf(it_y0 + (item_h - it_sz.y) * 0.5f);
            DrawOutlinedText(pdl, f, fs, ImVec2(it_tx, it_ty), it_text_col, border_dark, items[i]);
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    popup_open = ImGui::IsPopupOpen(popup_id);
    float total_h = has_label ? kLabeledBoxH : 20.0f;
    bool is_total_hov = (m.x >= x0 && m.x <= x1 && m.y >= pos.y - kLabelTextRise && m.y <= pos.y + total_h);
    return popup_open || (!dd_modal && is_total_hov);
}

inline bool DrawDropdownSingle(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* selected_idx, const char* const items[], int items_count, const char* label = nullptr, ImFont* font = nullptr, float font_size = 13.0f)
{
    int mask = (*selected_idx >= 0 && *selected_idx < items_count) ? (1 << *selected_idx) : 0;
    int before = mask;
    bool hov = DrawDropdown(id, dl, pos, width, &mask, items, items_count, label, font, font_size);
    if (mask != before)
    {
        int added = mask & ~before;
        int sel = -1;
        if (added != 0) { for (int i = 0; i < items_count; i++) if (added & (1 << i)) { sel = i; break; } }
        else { for (int i = 0; i < items_count; i++) if (mask & (1 << i)) { sel = i; break; } }
        if (sel < 0) sel = 0;
        *selected_idx = sel;
    }
    if (*selected_idx < 0 || *selected_idx >= items_count) *selected_idx = 0;
    return hov;
}

enum QuestionItemType
{
    QITEM_CHECKBOX = 0,
    QITEM_SLIDER,
    QITEM_SLIDERF,
    QITEM_DROPDOWN,
    QITEM_DROPDOWNSINGLE,
    QITEM_COLORPICKER,
    QITEM_COLORPICKER_PAIR,
    QITEM_KEYBIND,
    QITEM_SUBMENU
};

struct QuestionItem
{
    QuestionItemType type;
    const char* id;
    const char* label;
    bool* cond;

    bool* bool_val;

    int* int_val;
    int min_val;
    int max_val;
    const char* format;

    float* float_val;
    float float_min;
    float float_max;

    int* dd_mask;
    const char* const* dd_items;
    int dd_count;

    int* dd_sel;

    ImVec4* col_val;
    bool* outline_opt;
    ImVec4* outline_col;

    const char* id2;
    ImVec4* col_val2;
    bool* cond2;

    int* key_val;
    int* mode_val;

    const QuestionItem* sub_items;
    int sub_count;
    bool* open_val;
};

inline QuestionItem MakeQCheckbox(const char* label, bool* val, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_CHECKBOX;
    it.label = label;
    it.bool_val = val;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQCheckboxColor(const char* id, const char* label, bool* val, ImVec4* col, bool* outline_opt = nullptr, ImVec4* outline_col = nullptr, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_CHECKBOX;
    it.id = id;
    it.label = label;
    it.bool_val = val;
    it.col_val = col;
    it.outline_opt = outline_opt;
    it.outline_col = outline_col;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQCheckboxColor(const char* label, bool* val, ImVec4* col, bool* outline_opt = nullptr, ImVec4* outline_col = nullptr, bool* cond = nullptr)
{
    return MakeQCheckboxColor(nullptr, label, val, col, outline_opt, outline_col, cond);
}

inline QuestionItem MakeQCheckboxColorPair(const char* id, const char* label, bool* val, ImVec4* col, const char* id2, ImVec4* col2, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_CHECKBOX;
    it.id = id;
    it.label = label;
    it.bool_val = val;
    it.col_val = col;
    it.id2 = id2;
    it.col_val2 = col2;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQCheckboxColorPair(const char* label, bool* val, ImVec4* col, ImVec4* col2, bool* cond = nullptr)
{
    return MakeQCheckboxColorPair(nullptr, label, val, col, nullptr, col2, cond);
}

inline QuestionItem MakeQCheckboxKeybind(const char* id, const char* label, bool* val, int* key, int* mode, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_CHECKBOX;
    it.id = id;
    it.label = label;
    it.bool_val = val;
    it.key_val = key;
    it.mode_val = mode;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQSubmenu(const char* id, const char* label, const QuestionItem* sub_items, int sub_count, bool* open_val, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_SUBMENU;
    it.id = id;
    it.label = label;
    it.sub_items = sub_items;
    it.sub_count = sub_count;
    it.open_val = open_val;
    it.cond = cond;
    return it;
}

inline float QSubmenuContentHeight(const QuestionItem* items, int count)
{
    float h = 0.0f;
    if (!items || count <= 0)
        return h;
    for (int i = 0; i < count; ++i)
    {
        if (items[i].cond && !*items[i].cond)
            continue;
        if (items[i].type == QITEM_CHECKBOX || items[i].type == QITEM_COLORPICKER ||
            items[i].type == QITEM_COLORPICKER_PAIR || items[i].type == QITEM_KEYBIND)
            h += 20.0f;
        else if (items[i].type == QITEM_SLIDER || items[i].type == QITEM_SLIDERF)
            h += (items[i].label && items[i].label[0] != '\0') ? (kLabeledCtrlH + 4.0f) : 17.0f;
        else if (items[i].type == QITEM_DROPDOWN || items[i].type == QITEM_DROPDOWNSINGLE)
            h += (items[i].label && items[i].label[0] != '\0') ? (kLabeledBoxH + 4.0f) : 24.0f;
        else
            h += 20.0f;
    }
    return h;
}

inline QuestionItem MakeQSlider(const char* id, const char* label, int* val, int min_v, int max_v, const char* fmt = "%d", bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_SLIDER;
    it.id = id;
    it.label = label;
    it.int_val = val;
    it.min_val = min_v;
    it.max_val = max_v;
    it.format = fmt;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQSliderFloat(const char* id, const char* label, float* val, float min_v, float max_v, const char* fmt = "%.1f", bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_SLIDERF;
    it.id = id;
    it.label = label;
    it.float_val = val;
    it.float_min = min_v;
    it.float_max = max_v;
    it.format = fmt;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQDropdown(const char* id, const char* label, int* mask, const char* const items[], int count, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_DROPDOWN;
    it.id = id;
    it.label = label;
    it.dd_mask = mask;
    it.dd_items = items;
    it.dd_count = count;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQDropdownSingle(const char* id, const char* label, int* sel, const char* const items[], int count, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_DROPDOWNSINGLE;
    it.id = id;
    it.label = label;
    it.dd_sel = sel;
    it.dd_items = items;
    it.dd_count = count;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQColorPicker(const char* id, const char* label, ImVec4* col, bool* outline_opt, ImVec4* outline_col, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_COLORPICKER;
    it.id = id;
    it.label = label;
    it.col_val = col;
    it.outline_opt = outline_opt;
    it.outline_col = outline_col;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQColorPicker(const char* id, const char* label, ImVec4* col, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_COLORPICKER;
    it.id = id;
    it.label = label;
    it.col_val = col;
    it.outline_opt = nullptr;
    it.outline_col = nullptr;
    it.cond = cond;
    return it;
}

inline QuestionItem MakeQColorPickerPair(const char* id, const char* label, ImVec4* col, bool* cond, const char* id2, ImVec4* col2, bool* cond2 = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_COLORPICKER_PAIR;
    it.id = id;
    it.label = label;
    it.col_val = col;
    it.cond = cond;
    it.id2 = id2;
    it.col_val2 = col2;
    it.cond2 = cond2;
    return it;
}

inline QuestionItem MakeQColorPickerPair(const char* label, ImVec4* col, const char* label2, ImVec4* col2, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_COLORPICKER_PAIR;
    it.id = nullptr;
    it.label = label;
    it.col_val = col;
    it.cond = cond;
    it.id2 = nullptr;
    it.col_val2 = col2;
    it.cond2 = cond;
    return it;
}

inline QuestionItem MakeQKeybind(const char* id, const char* label, int* key, int* mode, bool* cond = nullptr)
{
    QuestionItem it{};
    it.type = QITEM_KEYBIND;
    it.id = id;
    it.label = label;
    it.key_val = key;
    it.mode_val = mode;
    it.cond = cond;
    return it;
}

struct QuestionOption
{
    bool* val = nullptr;
    const char* label = nullptr;
    ImVec4* col = nullptr;
    bool* outline_opt = nullptr;
    ImVec4* outline_col = nullptr;
    const char* id = nullptr;
    ImVec4* col2 = nullptr;
    const char* id2 = nullptr;
    bool* cond = nullptr;

    QuestionOption() = default;
    QuestionOption(bool* v, const char* lbl, ImVec4* c = nullptr, bool* out_opt = nullptr, ImVec4* out_col = nullptr, const char* i = nullptr, bool* cn = nullptr)
        : val(v), label(lbl), col(c), outline_opt(out_opt), outline_col(out_col), id(i), col2(nullptr), id2(nullptr), cond(cn) {}
    QuestionOption(bool* v, const char* lbl, ImVec4* c1, ImVec4* c2, const char* i1 = nullptr, const char* i2 = nullptr, bool* cn = nullptr)
        : val(v), label(lbl), col(c1), outline_opt(nullptr), outline_col(nullptr), id(i1), col2(c2), id2(i2), cond(cn) {}
};

inline bool DrawQuestionButton(const char* id, ImDrawList* dl, ImVec2 right_edge, const QuestionItem* items, int item_count, ImFont* font = nullptr, float font_size = 13.0f, int depth = 0)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    char popup_id[64];
    snprintf(popup_id, sizeof(popup_id), "##qb_pop_%s", id);

    const char* btn_text = "[ ? ]";
    ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, btn_text);
    float right_x = floorf(right_edge.x);
    float text_x = floorf(right_x - text_sz.x);
    float y0 = floorf(right_edge.y);
    float text_y = floorf(y0 + (14.0f - text_sz.y) * 0.5f);

    float hit_x0 = text_x - 3.0f;
    float hit_x1 = right_x + 3.0f;
    float hit_y0 = y0 - 2.0f;
    float hit_y1 = y0 + 16.0f;

    bool is_hov = (m.x >= hit_x0 && m.x <= hit_x1 && m.y >= hit_y0 && m.y <= hit_y1);
    bool own_popup = ImGui::IsPopupOpen(popup_id);
    bool sub_any = false;
    if ((ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup) && !own_popup && depth == 0) || g_activeBindId != nullptr)
        is_hov = false;

    if (is_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0))
        {
            if (!own_popup)
            {
                ImGui::OpenPopup(popup_id);
            }
        }
    }

    const ImU32 border_dark = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    ImU32 text_col = own_popup ? AccentU32() : (is_hov ? TextU32() : TextDimU32());
    DrawOutlinedText(dl, f, fs, ImVec2(text_x, text_y), text_col, border_dark, btn_text);

    float pad_x = 12.0f;
    float pad_y = 6.0f;
    bool has_slider_or_dd = false;
    bool has_picker_or_kb = false;
    bool has_picker_pair = false;
    float max_label_w = 0.0f;
    float pop_h = pad_y * 2.0f;

    for (int i = 0; i < item_count; i++)
    {
        if (items[i].type == QITEM_SLIDER || items[i].type == QITEM_SLIDERF || items[i].type == QITEM_DROPDOWN || items[i].type == QITEM_DROPDOWNSINGLE)
        {
            has_slider_or_dd = true;
        }
        else if (items[i].type == QITEM_COLORPICKER || items[i].type == QITEM_COLORPICKER_PAIR || items[i].type == QITEM_KEYBIND)
        {
            has_picker_or_kb = true;
            if (items[i].type == QITEM_COLORPICKER_PAIR || (items[i].outline_opt && items[i].outline_col))
                has_picker_pair = true;
        }
        else if (items[i].type == QITEM_CHECKBOX && (items[i].col_val || items[i].key_val))
        {
            has_picker_or_kb = true;
            if (items[i].col_val2 || (items[i].outline_opt && items[i].outline_col))
                has_picker_pair = true;
        }
        else if (items[i].type == QITEM_SUBMENU)
        {
            has_picker_or_kb = true;
            if (items[i].col_val)
                has_picker_pair = true;
        }
        ImVec2 opt_ts = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, items[i].label ? items[i].label : "");
        if (opt_ts.x > max_label_w) max_label_w = opt_ts.x;
        if (items[i].type == QITEM_SUBMENU && items[i].sub_items && items[i].sub_count > 0)
        {
            for (int k = 0; k < items[i].sub_count; ++k)
            {
                const char* sl = items[i].sub_items[k].label;
                if (!sl || !sl[0])
                    continue;
                ImVec2 sts = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, sl);
                float need = sts.x + 10.0f;
                if (need > max_label_w) max_label_w = need;
            }
        }

        bool pair_visible = items[i].type != QITEM_COLORPICKER_PAIR ||
            ((!items[i].cond || *items[i].cond) || (!items[i].cond2 || *items[i].cond2));
        if ((items[i].cond && !*items[i].cond && items[i].type != QITEM_COLORPICKER_PAIR) || !pair_visible)
        {
            continue;
        }

        if (items[i].type == QITEM_CHECKBOX)
        {
            pop_h += 20.0f;
        }
        else if (items[i].type == QITEM_SUBMENU)
        {
            pop_h += 20.0f;
            if ((!items[i].cond || *items[i].cond) && items[i].open_val && *items[i].open_val)
                pop_h += QSubmenuContentHeight(items[i].sub_items, items[i].sub_count);
        }
        else if (items[i].type == QITEM_SLIDER || items[i].type == QITEM_SLIDERF)
        {
            pop_h += (items[i].label && items[i].label[0] != '\0') ? (kLabeledCtrlH + 4.0f) : 17.0f;
        }
        else if (items[i].type == QITEM_DROPDOWN || items[i].type == QITEM_DROPDOWNSINGLE)
        {
            pop_h += (items[i].label && items[i].label[0] != '\0') ? (kLabeledBoxH + 4.0f) : 24.0f;
        }
        else if (items[i].type == QITEM_COLORPICKER || items[i].type == QITEM_COLORPICKER_PAIR)
        {
            pop_h += 20.0f;
        }
        else if (items[i].type == QITEM_KEYBIND)
        {
            pop_h += 20.0f;
        }
    }

    float pop_w = 100.0f;
    if (has_slider_or_dd)
    {
        pop_w = (std::max)(230.0f, pad_x * 2.0f + max_label_w + 30.0f);
    }
    else if (has_picker_or_kb)
    {
        float right_reserve = has_picker_pair ? 48.0f : 28.0f;
        float needed_w = pad_x * 2.0f + 14.0f + kCheckboxLabelGap + max_label_w + right_reserve;
        pop_w = (std::max)(170.0f, needed_w);
    }
    else
    {
        pop_w = floorf(pad_x + 14.0f + kCheckboxLabelGap + max_label_w + 14.0f);
        if (pop_w < 100.0f) pop_w = 100.0f;
    }

    ImVec2 pop_pos(floorf(right_x - pop_w), floorf(y0 + 18.0f));

    ImGui::SetNextWindowPos(pop_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(pop_w, pop_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(24.0f / 255.0f, 24.0f / 255.0f, 24.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + pop_w, w_min.y + pop_h);

        DrawPixelBox(pdl, w_min, w_max, GroupU32(), border_light, border_dark);
        ImGui::Dummy(ImVec2(pop_w, pop_h));

        float cur_y = w_min.y + pad_y;
        float item_w = pop_w - pad_x * 2.0f;

        g_inQuestionPopup = true;

        bool is_child_popup_active = false;
        for (int i = 0; i < item_count; i++)
        {
            char auto_cpid[64];
            snprintf(auto_cpid, sizeof(auto_cpid), "##q_cp_%s_%d", id ? id : "qb", i);
            char auto_cpid2[64];
            snprintf(auto_cpid2, sizeof(auto_cpid2), "##q_cp2_%s_%d", id ? id : "qb", i);
            const char* cp1_id = items[i].id ? items[i].id : auto_cpid;
            const char* cp2_id = items[i].id2 ? items[i].id2 : auto_cpid2;

            char ctx_id1[64];
            snprintf(ctx_id1, sizeof(ctx_id1), "%s##cp_ctx", cp1_id);
            char ctx_id2[64];
            snprintf(ctx_id2, sizeof(ctx_id2), "%s##cp_ctx", cp2_id);
            char out_id1[64];
            snprintf(out_id1, sizeof(out_id1), "%s_out", cp1_id);

            if (ImGui::IsPopupOpen(cp1_id) || ImGui::IsPopupOpen(ctx_id1) || ImGui::IsPopupOpen(out_id1) ||
                ImGui::IsPopupOpen(cp2_id) || ImGui::IsPopupOpen(ctx_id2))
            {
                is_child_popup_active = true;
                break;
            }
            if (items[i].type == QITEM_SUBMENU && items[i].id)
            {
                char subpid[64];
                snprintf(subpid, sizeof(subpid), "##qb_pop_%s", items[i].id);
                if (ImGui::IsPopupOpen(subpid))
                {
                    is_child_popup_active = true;
                    break;
                }
            }
        }

        for (int i = 0; i < item_count; i++)
        {
            bool pair_show = items[i].type != QITEM_COLORPICKER_PAIR ||
                ((!items[i].cond || *items[i].cond) || (!items[i].cond2 || *items[i].cond2));
            if ((items[i].cond && !*items[i].cond && items[i].type != QITEM_COLORPICKER_PAIR) || !pair_show)
            {
                continue;
            }

            if (items[i].type == QITEM_CHECKBOX)
            {
                float cb_x = w_min.x + pad_x;
                float cb_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                DrawCheckbox(pdl, ImVec2(cb_x, cb_y), items[i].bool_val, items[i].label, f, fs, !is_child_popup_active);

                char auto_cpid[64];
                snprintf(auto_cpid, sizeof(auto_cpid), "##q_cp_%s_%d", id ? id : "qb", i);
                char auto_cpid2[64];
                snprintf(auto_cpid2, sizeof(auto_cpid2), "##q_cp2_%s_%d", id ? id : "qb", i);
                const char* cp1_id = items[i].id ? items[i].id : auto_cpid;
                const char* cp2_id = items[i].id2 ? items[i].id2 : auto_cpid2;

                if (items[i].col_val && items[i].col_val2)
                {
                    float sx = w_max.x - pad_x - 14.0f;
                    DrawColorPicker(cp1_id, pdl, ImVec2(sx - 20.0f, cb_y), ImVec2(14.0f, 14.0f), items[i].col_val, nullptr, nullptr);
                    DrawColorPicker(cp2_id, pdl, ImVec2(sx, cb_y), ImVec2(14.0f, 14.0f), items[i].col_val2, nullptr, nullptr);
                }
                else if (items[i].col_val)
                {
                    ImVec2 cp_pos(w_max.x - pad_x - 14.0f, cb_y);
                    DrawColorPicker(cp1_id, pdl, cp_pos, ImVec2(14.0f, 14.0f), items[i].col_val, items[i].outline_opt, items[i].outline_col);
                }
                if (items[i].key_val)
                {
                    char auto_kbid[64];
                    snprintf(auto_kbid, sizeof(auto_kbid), "##q_kb_%s_%d", id ? id : "qb", i);
                    DrawKeybind(items[i].id ? items[i].id : auto_kbid, pdl, ImVec2(w_max.x - pad_x, cb_y), items[i].key_val, items[i].mode_val, f, fs);
                }
                cur_y += 20.0f;
            }
            else if (items[i].type == QITEM_SUBMENU)
            {
                float row_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                if (items[i].bool_val)
                {
                    DrawCheckbox(pdl, ImVec2(w_min.x + pad_x, row_y), items[i].bool_val, items[i].label, f, fs, !is_child_popup_active);
                }
                else if (items[i].label && items[i].label[0] != '\0')
                {
                    DrawOutlinedText(pdl, f, fs, ImVec2(w_min.x + pad_x, row_y), TextU32(), border_dark, items[i].label);
                }
                bool is_open = items[i].open_val && *items[i].open_val;
                const char* tg_text = is_open ? "[ - ]" : "[ + ]";
                ImVec2 tg_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, tg_text);
                float sw_right = floorf(w_max.x - pad_x);
                float tg_right = sw_right;
                if (items[i].col_val)
                    tg_right = floorf(sw_right - 14.0f - 6.0f);
                float tg_x = floorf(tg_right - tg_sz.x);
                float tg_y = floorf(row_y + (14.0f - tg_sz.y) * 0.5f);
                bool tg_hov = (m.x >= tg_x - 3.0f && m.x <= tg_right + 3.0f && tg_y - 2.0f <= m.y && m.y <= tg_y + 16.0f);
                if (is_child_popup_active || g_activeBindId != nullptr)
                    tg_hov = false;
                if (tg_hov)
                {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if (ImGui::IsMouseClicked(0) && items[i].open_val)
                        *items[i].open_val = !*items[i].open_val;
                }
                DrawOutlinedText(pdl, f, fs, ImVec2(tg_x, tg_y), tg_hov ? TextU32() : TextDimU32(), border_dark, tg_text);
                if (items[i].col_val)
                {
                    char auto_subcp[64];
                    snprintf(auto_subcp, sizeof(auto_subcp), "##q_subcp_%s", items[i].id ? items[i].id : "qb");
                    DrawColorPicker(auto_subcp, pdl, ImVec2(sw_right - 14.0f, row_y), ImVec2(14.0f, 14.0f), items[i].col_val, items[i].outline_opt, items[i].outline_col);
                }
                cur_y += 20.0f;
                if (is_open && items[i].sub_items && items[i].sub_count > 0)
                {
                    int tl_rows = 0;
                    for (int c = 0; c < items[i].sub_count; ++c)
                    {
                        const QuestionItem& s = items[i].sub_items[c];
                        if (s.cond && !*s.cond)
                            continue;
                        if (s.type == QITEM_CHECKBOX || s.type == QITEM_COLORPICKER)
                            ++tl_rows;
                    }
                    if (tl_rows > 0)
                    {
                        float tl_x = floorf(w_min.x + pad_x + 10.0f - 6.0f);
                        float tl_top = floorf(cur_y + 3.0f);
                        float tl_bot = floorf(cur_y + (float)(tl_rows - 1) * 20.0f + 17.0f);
                        pdl->AddLine(ImVec2(tl_x - 2.0f, tl_top), ImVec2(tl_x - 2.0f, tl_bot), border_dark, 1.0f);
                        pdl->AddLine(ImVec2(tl_x, tl_top), ImVec2(tl_x, tl_bot), IM_COL32(0x70, 0x60, 0x80, 255), 2.0f);
                        pdl->AddLine(ImVec2(tl_x + 2.0f, tl_top), ImVec2(tl_x + 2.0f, tl_bot), border_dark, 1.0f);
                    }
                    for (int k = 0; k < items[i].sub_count; ++k)
                    {
                        const QuestionItem& sub = items[i].sub_items[k];
                        if (sub.cond && !*sub.cond)
                            continue;
                        if (sub.type == QITEM_CHECKBOX)
                        {
                            float scb_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                            DrawCheckbox(pdl, ImVec2(w_min.x + pad_x + 10.0f, scb_y), sub.bool_val, sub.label, f, fs, !is_child_popup_active);
                            if (sub.col_val)
                            {
                                char auto_scp[64];
                                snprintf(auto_scp, sizeof(auto_scp), "##q_scp_%s_%d", items[i].id ? items[i].id : "qb", k);
                                const char* scp_id = sub.id ? sub.id : auto_scp;
                                DrawColorPicker(scp_id, pdl, ImVec2(w_max.x - pad_x - 14.0f, scb_y), ImVec2(14.0f, 14.0f), sub.col_val, sub.outline_opt, sub.outline_col);
                            }
                            cur_y += 20.0f;
                        }
                        else if (sub.type == QITEM_COLORPICKER)
                        {
                            float scp_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                            if (sub.label && sub.label[0] != '\0')
                            {
                                DrawOutlinedText(pdl, f, fs, ImVec2(w_min.x + pad_x + 10.0f, scp_y), TextU32(), border_dark, sub.label);
                            }
                            char auto_scp[64];
                            snprintf(auto_scp, sizeof(auto_scp), "##q_scpp_%s_%d", items[i].id ? items[i].id : "qb", k);
                            const char* scp_id = sub.id ? sub.id : auto_scp;
                            DrawColorPicker(scp_id, pdl, ImVec2(w_max.x - pad_x - 14.0f, scp_y), ImVec2(14.0f, 14.0f), sub.col_val, sub.outline_opt, sub.outline_col);
                            cur_y += 20.0f;
                        }
                    }
                }
            }
            else if (items[i].type == QITEM_SLIDER)
            {
                float sl_x = w_min.x + pad_x;
                float sl_h = (items[i].label && items[i].label[0] != '\0') ? (kLabeledCtrlH + 4.0f) : 17.0f;
                DrawSlider(items[i].id ? items[i].id : "##q_sl", pdl, ImVec2(sl_x, cur_y), item_w, items[i].int_val, items[i].min_val, items[i].max_val, items[i].label, items[i].format ? items[i].format : "%d", f, fs);
                cur_y += sl_h;
            }
            else if (items[i].type == QITEM_SLIDERF)
            {
                float sl_x = w_min.x + pad_x;
                float sl_h = (items[i].label && items[i].label[0] != '\0') ? (kLabeledCtrlH + 4.0f) : 17.0f;
                DrawSliderFloat(items[i].id ? items[i].id : "##q_slf", pdl, ImVec2(sl_x, cur_y), item_w, items[i].float_val, items[i].float_min, items[i].float_max, items[i].label, items[i].format ? items[i].format : "%.1f", f, fs);
                cur_y += sl_h;
            }
            else if (items[i].type == QITEM_DROPDOWN)
            {
                float dd_x = w_min.x + pad_x;
                float dd_h = (items[i].label && items[i].label[0] != '\0') ? (kLabeledBoxH + 4.0f) : 24.0f;
                DrawDropdown(items[i].id ? items[i].id : "##q_dd", pdl, ImVec2(dd_x, cur_y), item_w, items[i].dd_mask, items[i].dd_items, items[i].dd_count, items[i].label, f, fs);
                cur_y += dd_h;
            }
            else if (items[i].type == QITEM_DROPDOWNSINGLE)
            {
                float dd_x = w_min.x + pad_x;
                float dd_h = (items[i].label && items[i].label[0] != '\0') ? (kLabeledBoxH + 4.0f) : 24.0f;
                DrawDropdownSingle(items[i].id ? items[i].id : "##q_dds", pdl, ImVec2(dd_x, cur_y), item_w, items[i].dd_sel, items[i].dd_items, items[i].dd_count, items[i].label, f, fs);
                cur_y += dd_h;
            }
            else if (items[i].type == QITEM_COLORPICKER)
            {
                float row_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                if (items[i].label && items[i].label[0] != '\0')
                {
                    DrawOutlinedText(pdl, f, fs, ImVec2(w_min.x + pad_x, row_y), TextU32(), border_dark, items[i].label);
                }
                char auto_cpid[64];
                snprintf(auto_cpid, sizeof(auto_cpid), "##q_cp_%s_%d", id ? id : "qb", i);
                const char* cp_id = items[i].id ? items[i].id : auto_cpid;
                ImVec2 cp_pos(w_max.x - pad_x - 14.0f, row_y);
                DrawColorPicker(cp_id, pdl, cp_pos, ImVec2(14.0f, 14.0f), items[i].col_val, items[i].outline_opt, items[i].outline_col);
                cur_y += 20.0f;
            }
            else if (items[i].type == QITEM_COLORPICKER_PAIR)
            {
                bool show1 = !items[i].cond || *items[i].cond;
                bool show2 = !items[i].cond2 || *items[i].cond2;
                float row_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                if (items[i].label && items[i].label[0] != '\0')
                {
                    DrawOutlinedText(pdl, f, fs, ImVec2(w_min.x + pad_x, row_y), TextU32(), border_dark, items[i].label);
                }
                char auto_cpid1[64];
                snprintf(auto_cpid1, sizeof(auto_cpid1), "##q_cp1_%s_%d", id ? id : "qb", i);
                char auto_cpid2[64];
                snprintf(auto_cpid2, sizeof(auto_cpid2), "##q_cp2_%s_%d", id ? id : "qb", i);
                const char* cp1_id = items[i].id ? items[i].id : auto_cpid1;
                const char* cp2_id = items[i].id2 ? items[i].id2 : auto_cpid2;

                float sx = w_max.x - pad_x - 14.0f;
                if (show1 && show2)
                {
                    DrawColorPicker(cp1_id, pdl, ImVec2(sx - 20.0f, row_y), ImVec2(14.0f, 14.0f), items[i].col_val, nullptr, nullptr);
                    DrawColorPicker(cp2_id, pdl, ImVec2(sx, row_y), ImVec2(14.0f, 14.0f), items[i].col_val2, nullptr, nullptr);
                }
                else if (show1)
                {
                    DrawColorPicker(cp1_id, pdl, ImVec2(sx, row_y), ImVec2(14.0f, 14.0f), items[i].col_val, nullptr, nullptr);
                }
                else if (show2)
                {
                    DrawColorPicker(cp2_id, pdl, ImVec2(sx, row_y), ImVec2(14.0f, 14.0f), items[i].col_val2, nullptr, nullptr);
                }
                cur_y += 20.0f;
            }
            else if (items[i].type == QITEM_KEYBIND)
            {
                float row_y = floorf(cur_y + (20.0f - 14.0f) * 0.5f);
                if (items[i].label && items[i].label[0] != '\0')
                {
                    DrawOutlinedText(pdl, f, fs, ImVec2(w_min.x + pad_x, row_y), TextU32(), border_dark, items[i].label);
                }
                char auto_kbid[64];
                snprintf(auto_kbid, sizeof(auto_kbid), "##q_kb_%s_%d", id ? id : "qb", i);
                const char* kb_id = items[i].id ? items[i].id : auto_kbid;
                DrawKeybind(kb_id, pdl, ImVec2(w_max.x - pad_x, row_y), items[i].key_val, items[i].mode_val, f, fs);
                cur_y += 20.0f;
            }
        }

        g_inQuestionPopup = false;

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    return is_hov || own_popup || sub_any;
}

inline bool DrawQuestionButton(const char* id, ImDrawList* dl, ImVec2 right_edge, QuestionOption* options, int option_count, ImFont* font = nullptr, float font_size = 13.0f)
{
    QuestionItem stack_items[16];
    int n = (option_count < 16) ? option_count : 16;
    for (int i = 0; i < n; i++)
    {
        char auto_cpid[64];
        snprintf(auto_cpid, sizeof(auto_cpid), "%s_qopt_%d", id ? id : "qb", i);
        if (options[i].col && options[i].col2)
        {
            char auto_cpid2[64];
            snprintf(auto_cpid2, sizeof(auto_cpid2), "%s_qopt2_%d", id ? id : "qb", i);
            stack_items[i] = MakeQCheckboxColorPair(options[i].id ? options[i].id : auto_cpid, options[i].label, options[i].val, options[i].col, options[i].id2 ? options[i].id2 : auto_cpid2, options[i].col2, options[i].cond);
        }
        else if (options[i].col)
        {
            stack_items[i] = MakeQCheckboxColor(options[i].id ? options[i].id : auto_cpid, options[i].label, options[i].val, options[i].col, options[i].outline_opt, options[i].outline_col, options[i].cond);
        }
        else
        {
            stack_items[i] = MakeQCheckbox(options[i].label, options[i].val, options[i].cond);
        }
    }
    return DrawQuestionButton(id, dl, right_edge, stack_items, n, font, font_size);
}

inline bool DrawQuestionButton(const char* id, ImDrawList* dl, ImVec2 right_edge, bool* sub_val, const char* sub_label = "Sub-option", ImVec4* col = nullptr, ImFont* font = nullptr, float font_size = 13.0f)
{
    QuestionOption opt(sub_val, sub_label, col);
    return DrawQuestionButton(id, dl, right_edge, &opt, 1, font, font_size);
}

inline void UpdateThemeFromMask()
{
    static int s_prevTheme = -1;
    if (s_prevTheme < 0) s_prevTheme = g_themeMask;
    if (g_themeMask != s_prevTheme)
    {
        int added = g_themeMask & ~s_prevTheme;
        int sel = -1;
        if (added != 0) { for (int i = 0; i < 6; i++) if (added & (1 << i)) { sel = i; break; } }
        else { for (int i = 0; i < 6; i++) if (g_themeMask & (1 << i)) { sel = i; break; } }
        if (sel < 0) sel = 0;
        g_themeMask = (1 << sel);
        s_prevTheme = g_themeMask;
        ApplyThemePalette(sel);
    }
    if (g_themeMask == 0) { g_themeMask = (1 << 0); s_prevTheme = g_themeMask; }
}

inline constexpr float kNavTile = 32.0f;
inline constexpr float kNavPad = 6.0f;
inline constexpr float kNavGap = 6.0f;
inline constexpr float kNavTop = 12.0f;

inline void CalcNavBarRect(int width, int height, ImVec2& outMin, ImVec2& outMax)
{
    (void)height;
    float bw = kNavPad * 2.0f + kNavTile * 3.0f + kNavGap * 2.0f;
    float bh = kNavPad * 2.0f + kNavTile;
    float bx0 = floorf(((float)width - bw) * 0.5f);
    outMin = ImVec2(bx0, kNavTop);
    outMax = ImVec2(bx0 + bw, kNavTop + bh);
}

inline bool NavHitTest(int x, int y, int width, int height)
{
    ImVec2 mn, mx;
    CalcNavBarRect(width, height, mn, mx);
    return (x >= (int)mn.x && x <= (int)mx.x && y >= (int)mn.y && y <= (int)mx.y);
}

inline bool PanelHitTest(int x, int y)
{
    if (g_settingsOpen && g_setPos.x >= 0.0f && x >= (int)g_setPos.x && x <= (int)(g_setPos.x + g_setSize.x) && y >= (int)(g_setPos.y - 16.0f) && y <= (int)(g_setPos.y + g_setSize.y + 16.0f))
    {
        return true;
    }
    if (g_themeOpen && g_themePos.x >= 0.0f && x >= (int)g_themePos.x && x <= (int)(g_themePos.x + g_themeSize.x) && y >= (int)(g_themePos.y - 16.0f) && y <= (int)(g_themePos.y + g_themeSize.y + 16.0f))
    {
        return true;
    }
    if (g_keybinds && g_menuOpen && g_bindsPos.x >= 0.0f && x >= (int)g_bindsPos.x && x <= (int)(g_bindsPos.x + g_bindsSize.x) && y >= (int)g_bindsPos.y && y <= (int)(g_bindsPos.y + g_bindsSize.y))
    {
        return true;
    }
    if (g_movement && g_menuOpen && g_movementPos.x >= 0.0f && x >= (int)g_movementPos.x && x <= (int)(g_movementPos.x + g_movementSize.x) && y >= (int)g_movementPos.y && y <= (int)(g_movementPos.y + g_movementSize.y))
    {
        return true;
    }
    if (g_internalDebug && g_menuOpen && g_debugPos.x >= 0.0f && x >= (int)g_debugPos.x && x <= (int)(g_debugPos.x + g_debugSize.x) && y >= (int)g_debugPos.y && y <= (int)(g_debugPos.y + g_debugSize.y))
    {
        return true;
    }
    if (g_explorerOpen && g_explorerPos.x >= 0.0f && x >= (int)g_explorerPos.x && x <= (int)(g_explorerPos.x + g_explorerSize.x) && y >= (int)g_explorerPos.y && y <= (int)(g_explorerPos.y + g_explorerSize.y))
    {
        return true;
    }
    if (g_playerPreviewOpen && g_playerPreviewPos.x >= 0.0f && x >= (int)g_playerPreviewPos.x && x <= (int)(g_playerPreviewPos.x + g_playerPreviewSize.x) && y >= (int)g_playerPreviewPos.y && y <= (int)(g_playerPreviewPos.y + g_playerPreviewSize.y))
    {
        return true;
    }
    return false;
}

inline ID3D11ShaderResourceView* g_iconHomeTex = nullptr;
inline ID3D11ShaderResourceView* g_iconPalTex = nullptr;
inline ID3D11ShaderResourceView* g_iconFileTex = nullptr;
inline ID3D11ShaderResourceView* g_iconUserTex = nullptr;

ID3D11ShaderResourceView* LoadIconTexture(const unsigned char* png, int len);

inline void RenderWatermark(ImDrawList* dl, int width, float fps)
{
    if (!g_watermark)
    {
        return;
    }
    char buf[128] = "rpbrx";
    int len = (int)strlen(buf);
    static float s_fpsSmooth = -1.0f;
    static float s_fpsLastTime = 0.0f;

    float now_t = (float)ImGui::GetTime();
    float sdt = (s_fpsLastTime > 0.0f) ? (now_t - s_fpsLastTime) : 0.016f;
    if (sdt < 0.0001f) sdt = 0.0001f;
    if (sdt > 0.5f) sdt = 0.5f;
    s_fpsLastTime = now_t;
    float alpha = 1.0f - expf(-sdt / 0.6f);

    if (s_fpsSmooth < 0.0f) s_fpsSmooth = fps;
    else s_fpsSmooth += (fps - s_fpsSmooth) * alpha;

    if (g_watermarkFps && len < (int)sizeof(buf) - 1)
    {
        int disp_fps = (int)(s_fpsSmooth + 0.5f);
        if (disp_fps < 10) disp_fps = (int)((s_fpsSmooth + 0.5f) / 10.0f) * 10;
        else disp_fps = (disp_fps / 10) * 10;
        if (disp_fps < 0) disp_fps = 0;
        if (disp_fps > 9990) disp_fps = 9990;
        len += snprintf(buf + len, sizeof(buf) - len, " | %d fps", disp_fps);
    }
    if (g_watermarkPlayers && len < (int)sizeof(buf) - 1)
    {
        int n = (rpbrx::g_proc && rpbrx::g_proc->connected()) ? (int)rpbrx::g_ctx.player_list().size() : 0;
        if (n > 999) n = 999;
        len += snprintf(buf + len, sizeof(buf) - len, " | %d players", n);
    }
    if (g_watermarkDate && len < (int)sizeof(buf) - 1)
    {
        time_t now = time(nullptr);
        struct tm tm_buf;
        localtime_s(&tm_buf, &now);
        len += snprintf(buf + len, sizeof(buf) - len, " | %02d.%02d.%04d", tm_buf.tm_mday, tm_buf.tm_mon + 1, tm_buf.tm_year + 1900);
    }
    ImFont* f = g_fontSmall ? g_fontSmall : ImGui::GetFont();
    char tmpl[128] = "rpbrx";
    int tlen = (int)strlen(tmpl);
    if (g_watermarkFps)
    {
        tlen += snprintf(tmpl + tlen, sizeof(tmpl) - tlen, " | 9999 fps");
    }
    if (g_watermarkPlayers)
    {
        tlen += snprintf(tmpl + tlen, sizeof(tmpl) - tlen, " | 999 players");
    }
    if (g_watermarkDate)
    {
        tlen += snprintf(tmpl + tlen, sizeof(tmpl) - tlen, " | 00.00.0000");
    }
    ImVec2 ref_sz = f->CalcTextSizeA(13.0f, FLT_MAX, 0.0f, tmpl);
    float bw = floorf(ref_sz.x + 16.0f);
    float bh = 20.0f;
    float bx1 = (float)width - 12.0f;
    float bx0 = floorf(bx1 - bw);
    float by0 = 12.0f;
    float by1 = by0 + bh;
    DrawPixelBox(dl, ImVec2(bx0, by0), ImVec2(bx1, by1), CtrlBgU32(), BorderU32(), IM_COL32(0, 0, 0, 255));
    float tx = floorf(bx0 + 8.0f);
    float ty = floorf(by0 + (bh - ref_sz.y) * 0.5f);
    DrawOutlinedText(dl, f, 13.0f, ImVec2(tx, ty), TextU32(), IM_COL32(0, 0, 0, 255), buf);
}

inline int ImGuiKeyToVirtualKey(int key)
{
    if (key >= ImGuiKey_0 && key <= ImGuiKey_9) return '0' + (key - ImGuiKey_0);
    if (key >= ImGuiKey_A && key <= ImGuiKey_Z) return 'A' + (key - ImGuiKey_A);
    if (key >= ImGuiKey_F1 && key <= ImGuiKey_F12) return VK_F1 + (key - ImGuiKey_F1);
    if (key >= ImGuiKey_Keypad0 && key <= ImGuiKey_Keypad9) return VK_NUMPAD0 + (key - ImGuiKey_Keypad0);
    if (key == ImGuiKey_MouseLeft) return VK_LBUTTON;
    if (key == ImGuiKey_MouseRight) return VK_RBUTTON;
    if (key == ImGuiKey_MouseMiddle) return VK_MBUTTON;
    if (key == ImGuiKey_MouseX1) return VK_XBUTTON1;
    if (key == ImGuiKey_MouseX2) return VK_XBUTTON2;
    if (key == ImGuiKey_Tab) return VK_TAB;
    if (key == ImGuiKey_LeftArrow) return VK_LEFT;
    if (key == ImGuiKey_RightArrow) return VK_RIGHT;
    if (key == ImGuiKey_UpArrow) return VK_UP;
    if (key == ImGuiKey_DownArrow) return VK_DOWN;
    if (key == ImGuiKey_PageUp) return VK_PRIOR;
    if (key == ImGuiKey_PageDown) return VK_NEXT;
    if (key == ImGuiKey_Home) return VK_HOME;
    if (key == ImGuiKey_End) return VK_END;
    if (key == ImGuiKey_Insert) return VK_INSERT;
    if (key == ImGuiKey_Delete) return VK_DELETE;
    if (key == ImGuiKey_Backspace) return VK_BACK;
    if (key == ImGuiKey_Space) return VK_SPACE;
    if (key == ImGuiKey_Enter || key == ImGuiKey_KeypadEnter) return VK_RETURN;
    if (key == ImGuiKey_Escape) return VK_ESCAPE;
    if (key == ImGuiKey_LeftShift || key == ImGuiKey_RightShift) return VK_SHIFT;
    if (key == ImGuiKey_LeftCtrl || key == ImGuiKey_RightCtrl) return VK_CONTROL;
    if (key == ImGuiKey_LeftAlt || key == ImGuiKey_RightAlt) return VK_MENU;
    if (key == ImGuiKey_CapsLock) return VK_CAPITAL;
    return 0;
}

inline bool IsKeybindDown(int key)
{
    if (key <= 0) return false;
    int vk = ImGuiKeyToVirtualKey(key);
    if (vk != 0)
    {
        return (GetAsyncKeyState(vk) & 0x8000) != 0;
    }
    if (key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END)
    {
        return ImGui::IsKeyDown((ImGuiKey)key);
    }
    return false;
}

struct MenuKeyToggle
{
    bool toggled = false;
    bool prevDown = false;
};

inline std::unordered_map<std::string, MenuKeyToggle> s_menuKeyToggles;
inline std::mutex s_menuKeyMtx;

inline bool IsMenuKeyActive(const char* id, int key, int mode)
{
    if (mode == 0) return true;
    if (key <= 0) return false;
    bool down = IsKeybindDown(key);
    if (mode == 2) return down;
    std::lock_guard<std::mutex> lk(s_menuKeyMtx);
    std::string k = id ? id : "";
    MenuKeyToggle& st = s_menuKeyToggles[k];
    if (down && !st.prevDown) st.toggled = !st.toggled;
    st.prevDown = down;
    return st.toggled;
}

inline void RenderKeybinds(ImDrawList* dl, int width, int height)
{
    (void)dl; (void)width; (void)height;
}

inline void RenderMovement(ImDrawList* dl, int width, int height)
{
    (void)dl; (void)width; (void)height;
}

inline bool CopyTextToClipboard(const std::string& text)
{
    if (text.empty())
        return false;
    HWND hwnd = Overlay::GetOverlayWindow();
    if (!hwnd)
        hwnd = GetForegroundWindow();
    if (!hwnd || !OpenClipboard(hwnd))
        return false;
    bool ok = false;
    EmptyClipboard();
    size_t bytes = text.size() + 1;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, bytes);
    if (mem)
    {
        void* dst = GlobalLock(mem);
        if (dst)
        {
            memcpy(dst, text.c_str(), bytes);
            GlobalUnlock(mem);
            ok = SetClipboardData(CF_TEXT, mem) != nullptr;
        }
        if (!ok)
            GlobalFree(mem);
    }
    CloseClipboard();
    return ok;
}

inline void RenderInternalDebug(ImDrawList* dl, int width, int height, float fps)
{
    (void)dl; (void)width; (void)height; (void)fps;
}

inline bool RenderNavBar(ImDrawList* dl, int width, int height)
{
    (void)height;
    g_navBarHov = false;
    if (g_navHidden)
    {
        return false;
    }
    const float tile = kNavTile;
    const float pad = kNavPad;
    const float gap = kNavGap;
    ImVec2 barMin, barMax;
    CalcNavBarRect(width, height, barMin, barMax);
    float bx0 = barMin.x;
    float by0 = barMin.y;
    float bx1 = barMax.x;
    float by1 = barMax.y;

    float hx0 = bx0 + pad;
    float hy0 = by0 + pad;
    float px0 = hx0 + tile + gap;
    float py0 = hy0;
    float sx0 = px0 + tile + gap;
    float sy0 = hy0;

    bool home_active = g_menuOpen;
    bool pal_active = g_themeOpen;
    bool set_active = g_settingsOpen;

    DrawPixelBox(dl, ImVec2(bx0, by0), ImVec2(bx1, by1), FrameU32(), BorderU32(), IM_COL32(0, 0, 0, 255));
    dl->AddRectFilled(ImVec2(bx0 + 2.0f, by0 + 2.0f), ImVec2(bx1 - 2.0f, by0 + 3.0f), AccentU32());

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;

    bool home_hov = (m.x >= hx0 && m.x <= hx0 + tile && m.y >= hy0 && m.y <= hy0 + tile);
    bool pal_hov  = (m.x >= px0 && m.x <= px0 + tile && m.y >= py0 && m.y <= py0 + tile);
    bool set_hov  = (m.x >= sx0 && m.x <= sx0 + tile && m.y >= sy0 && m.y <= sy0 + tile);

    if (IsModalActive())
    {
        home_hov = false;
        pal_hov = false;
        set_hov = false;
    }

    if (home_hov || pal_hov || set_hov)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
    g_navBarHov = home_hov || pal_hov || set_hov;

    bool home_click = home_hov && ImGui::IsMouseClicked(0);
    bool pal_click  = pal_hov && ImGui::IsMouseClicked(0);
    bool set_click  = set_hov && ImGui::IsMouseClicked(0);

    if (home_click)
    {
        if (g_menuOpen)
        {
            g_menuOpen = false;
        }
        else
        {
            g_activeTab = 0;
            g_menuOpen = true;
        }
    }
    if (pal_click)
    {
        g_themeOpen = !g_themeOpen;
    }
    if (set_click)
    {
        g_settingsOpen = !g_settingsOpen;
    }

    auto draw_nav_tile = [&](ImVec2 pos, bool active, bool hov) {
        ImU32 bg = (active || hov) ? CtrlHovU32() : CtrlBgU32();
        ImU32 border = (active || hov) ? BorderHovU32() : BorderU32();
        DrawPixelBox(dl, pos, ImVec2(pos.x + tile, pos.y + tile), bg, border, IM_COL32(0, 0, 0, 255));
    };

    draw_nav_tile(ImVec2(hx0, hy0), home_active, home_hov);
    draw_nav_tile(ImVec2(px0, py0), pal_active, pal_hov);
    draw_nav_tile(ImVec2(sx0, sy0), set_active, set_hov);

    ImU32 home_hole = (home_active || home_hov) ? CtrlHovU32() : CtrlBgU32();
    ImU32 set_hole  = (set_active || set_hov)   ? CtrlHovU32() : CtrlBgU32();

    ImU32 col_active_icon = AccentU32();
    ImU32 col_hov_icon = IM_COL32(255, 255, 255, 255);
    ImU32 col_idle_icon = TextDimU32();

    ImU32 home_glyph = home_active ? (home_hov ? col_hov_icon : col_active_icon) : (home_hov ? col_hov_icon : col_idle_icon);
    ImU32 pal_col    = pal_active  ? (pal_hov  ? col_hov_icon : col_active_icon) : (pal_hov  ? col_hov_icon : col_idle_icon);
    ImU32 set_glyph  = set_active  ? (set_hov  ? col_hov_icon : col_active_icon) : (set_hov  ? col_hov_icon : col_idle_icon);

    float hcx = hx0 + tile * 0.5f;
    float hcy = hy0 + tile * 0.5f;
    if (!g_iconHomeTex)
    {
        g_iconHomeTex = LoadIconTexture(kIconHomePng, (int)sizeof(kIconHomePng));
    }
    if (g_iconHomeTex)
    {
        float hisz = 16.0f;
        dl->AddImage((ImTextureID)g_iconHomeTex, ImVec2(hcx - hisz * 0.5f, hcy - hisz * 0.5f), ImVec2(hcx + hisz * 0.5f, hcy + hisz * 0.5f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), home_glyph);
    }
    else
    {
        dl->AddTriangleFilled(ImVec2(hcx - 6.0f, hcy + 1.0f), ImVec2(hcx + 6.0f, hcy + 1.0f), ImVec2(hcx, hcy - 5.0f), home_glyph);
        dl->AddRectFilled(ImVec2(hcx - 4.0f, hcy + 1.0f), ImVec2(hcx + 4.0f, hcy + 6.0f), home_glyph);
        dl->AddRectFilled(ImVec2(hcx - 1.5f, hcy + 3.5f), ImVec2(hcx + 1.5f, hcy + 6.0f), home_hole);
    }

    float scx = sx0 + tile * 0.5f;
    float scy = sy0 + tile * 0.5f;
    for (int i = 0; i < 8; i++)
    {
        float ang = (float)i * 3.14159265f / 4.0f;
        float dx = cosf(ang);
        float dy = sinf(ang);
        dl->AddLine(ImVec2(scx + dx * 5.0f, scy + dy * 5.0f), ImVec2(scx + dx * 8.0f, scy + dy * 8.0f), set_glyph, 2.5f);
    }
    dl->AddCircle(ImVec2(scx, scy), 5.0f, set_glyph, 24, 2.5f);
    dl->AddCircleFilled(ImVec2(scx, scy), 2.0f, set_hole);

    float pcx = px0 + tile * 0.5f;
    float pcy = py0 + tile * 0.5f;
    if (!g_iconPalTex)
    {
        g_iconPalTex = LoadIconTexture(kIconPalPng, (int)sizeof(kIconPalPng));
    }
    if (g_iconPalTex)
    {
        float pisz = 16.0f;
        dl->AddImage((ImTextureID)g_iconPalTex, ImVec2(pcx - pisz * 0.5f, pcy - pisz * 0.5f), ImVec2(pcx + pisz * 0.5f, pcy + pisz * 0.5f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), pal_col);
    }
    else
    {
        dl->AddCircle(ImVec2(pcx, pcy), 6.0f, pal_col, 24, 2.0f);
        dl->AddCircleFilled(ImVec2(pcx - 3.0f, pcy - 2.0f), 1.3f, pal_col);
        dl->AddCircleFilled(ImVec2(pcx + 2.5f, pcy - 3.0f), 1.3f, pal_col);
        dl->AddCircleFilled(ImVec2(pcx + 3.0f, pcy + 2.0f), 1.3f, pal_col);
        dl->AddCircleFilled(ImVec2(pcx + 3.5f, pcy + 3.5f), 2.0f, (pal_active || pal_hov) ? CtrlHovU32() : CtrlBgU32());
    }

    return g_navBarHov;
}

inline void Render()
{
    if (!g_menuOpen)
    {
        g_menuAnim = 0.0f;
        g_activeBindId = nullptr;
        g_modalAtFrameStart = false;
        return;
    }
    g_modalAtFrameStart = (ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup) || g_activeBindId != nullptr);
    ImGuiIO& io = ImGui::GetIO();
    if (g_menuAnim < 1.0f)
    {
        float dt = io.DeltaTime;
        if (dt > 0.05f) dt = 0.05f;
        g_menuAnim += dt * 6.0f;
        if (g_menuAnim > 1.0f) g_menuAnim = 1.0f;
    }
    float t = g_menuAnim;
    float ease = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
    float y_off = (1.0f - ease) * -14.0f;

    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
    center.y -= 100.0f;
    if (g_pos.x < 0.0f)
    {
        g_pos = ImVec2(center.x - g_menuSize.x * 0.5f, center.y - g_menuSize.y * 0.5f);
    }
    ImGui::SetNextWindowSize(g_menuSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(g_pos.x, g_pos.y + y_off), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(ease);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ease);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, g_colFrame);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("##uibase", nullptr, flags))
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        pos.x = floorf(pos.x);
        pos.y = floorf(pos.y);
        ImVec2 max = ImVec2(floorf(pos.x + size.x), floorf(pos.y + size.y));

        const ImU32 col_outline    = IM_COL32(0, 0, 0, 255);
        const ImU32 col_bg_frame   = FrameU32();
        const ImU32 col_bg_group   = GroupU32();
        const ImU32 col_highlight  = BorderU32();
        ImU32 col_accent_top = ImGui::ColorConvertFloat4ToU32(g_accent1);

        DrawPixelBox(dl, pos, max, col_bg_frame, col_highlight, col_outline);

        dl->AddRectFilled(ImVec2(pos.x + 2.0f, pos.y + 2.0f), ImVec2(max.x - 2.0f, pos.y + 3.0f), col_accent_top);

        const float header_space = 46.0f;
        const float margin = 10.0f;

        const ImU32 col_text = TextU32();
        DrawWaveText(dl, ImVec2(pos.x + 10.0f, pos.y + 12.0f), col_text, col_outline, "home");

        ImVec2 inner_pos = ImVec2(pos.x + margin, pos.y + margin + header_space);
        ImVec2 inner_max = ImVec2(max.x - margin, max.y - margin);

        ImFont* tab_font = g_fontSmall ? g_fontSmall : ImGui::GetFont();
        const int kTabCount = 2;
        const char* sections[] = { "Combat", "Render" };
        const float tab_h = 22.0f;
        const float tab_top_y = inner_pos.y - tab_h;
        const float pad_x = 8.0f;
        const float tab_gap = 4.0f;

        float tab_bounds_min_x[kTabCount];
        float tab_bounds_max_x[kTabCount];
        float cur_tab_x = inner_pos.x + 12.0f;

        ImVec2 m = io.MousePos;
        bool tab_hovered = false;

        for (int i = 0; i < kTabCount; i++)
        {
            ImVec2 text_sz = tab_font->CalcTextSizeA(13.0f, FLT_MAX, 0.0f, sections[i]);
            float w = floorf(text_sz.x + pad_x * 2.0f);
            tab_bounds_min_x[i] = cur_tab_x;
            tab_bounds_max_x[i] = cur_tab_x + w;
            bool is_hov = (m.x >= cur_tab_x && m.x <= cur_tab_x + w && m.y >= tab_top_y && m.y <= inner_pos.y);
            if (is_hov)
            {
                tab_hovered = true;
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                if (ImGui::IsMouseClicked(0))
                {
                    g_activeTab = i;
                }
            }
            cur_tab_x += w + tab_gap;
        }

        if (g_activeTab < 0 || g_activeTab >= kTabCount)
        {
            g_activeTab = 0;
        }

        float active_min_x = tab_bounds_min_x[g_activeTab];
        float active_max_x = tab_bounds_max_x[g_activeTab];

        DrawTabbedContainer(dl, inner_pos, inner_max, active_min_x, active_max_x, tab_top_y, col_bg_frame, col_highlight, col_outline, col_accent_top);

        for (int i = 0; i < kTabCount; i++)
        {
            float t_min_x = tab_bounds_min_x[i];
            float t_max_x = tab_bounds_max_x[i];
            ImVec2 text_sz = tab_font->CalcTextSizeA(13.0f, FLT_MAX, 0.0f, sections[i]);
            float tx = floorf(t_min_x + (t_max_x - t_min_x - text_sz.x) * 0.5f);
            float ty = floorf(tab_top_y + (tab_h - text_sz.y) * 0.5f + 1.0f);
            bool is_hov = (m.x >= t_min_x && m.x <= t_max_x && m.y >= tab_top_y && m.y <= inner_pos.y);
            bool is_active = (g_activeTab == i);
            ImU32 col_tab = is_active ? TextU32() : TextDimU32();
            DrawOutlinedText(dl, tab_font, 13.0f, ImVec2(tx, ty), col_tab, col_outline, sections[i]);
        }

        const float group_pad = 12.0f;
        const float group_gap = 10.0f;
        float avail_w = (inner_max.x - inner_pos.x) - (group_pad * 2.0f) - group_gap;
        float col_w = floorf(avail_w * 0.5f);

        ImVec2 left_min = ImVec2(inner_pos.x + group_pad, inner_pos.y + group_pad + 6.0f);
        ImVec2 left_max = ImVec2(left_min.x + col_w, inner_max.y - group_pad);

        ImVec2 right_min = ImVec2(left_max.x + group_gap, inner_pos.y + group_pad + 6.0f);
        ImVec2 right_max = ImVec2(inner_max.x - group_pad, inner_max.y - group_pad);

        if (g_activeTab == 0)
        {
            DrawGroupBox(dl, left_min, left_max, "aim assist", tab_font, 13.0f, col_bg_group, col_highlight, col_outline);
            DrawGroupBox(dl, right_min, right_max, "camera & misc", tab_font, 13.0f, col_bg_group, col_highlight, col_outline);
        }
        else if (g_activeTab == 1)
        {
            DrawGroupBox(dl, left_min, left_max, "player esp", tab_font, 13.0f, col_bg_group, col_highlight, col_outline);
            DrawGroupBox(dl, right_min, right_max, "effects & world", tab_font, 13.0f, col_bg_group, col_highlight, col_outline);
        }

        bool cb_hov = false;
        bool cp_hov = false;
        bool kb_hov = false;
        bool sl_hov = false;
        bool btn_hov = false;
        bool ti_hov = false;
        bool dd_hov = false;
        ImVec2 picker_sz(14.0f, 14.0f);

        const float swatch_pair_w = picker_sz.x * 2.0f + 6.0f;
        const float qb_right_off = 14.0f + swatch_pair_w + 6.0f;
        const float qb_single_swatch_off = 14.0f + picker_sz.x + 6.0f;
        float col_avail_w = (left_max.x - 14.0f) - (left_min.x + 14.0f);
        float col_avail_w_r = (right_max.x - 14.0f) - (right_min.x + 14.0f);

        // ===== TAB 0: COMBAT =====
        if (g_activeTab == 0)
        {
            float y = left_min.y + 20.0f;

            // Aim Assist checkbox + keybind
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::aim_enabled, "Aim Assist", tab_font, 13.0f);
            kb_hov |= DrawKeybind("##aim_key", dl, ImVec2(left_max.x - 14.0f, y), &S::aim_key, &S::aim_key_mode, tab_font, 13.0f);
            y += kCheckRowPitch;

            // Target Part dropdown
            {
                const char* part_items[] = { "RootPart", "Head" };
                dd_hov |= DrawDropdownSingle("##dd_aimpart", dl, ImVec2(left_min.x + 14.0f, y), col_avail_w, &S::aim_part, part_items, 2, "Target Part", tab_font, 13.0f);
                y += 37.0f;
            }

            // FOV slider (0-1000)
            sl_hov |= DrawSliderFloat("##sl_aimfov", dl, ImVec2(left_min.x + 14.0f, y), col_avail_w, &S::aim_fov, 0.0f, 1000.0f, "FOV", "%.0f", tab_font, 13.0f);
            y += 33.0f;

            // Speed slider (0-30, step 0.1)
            sl_hov |= DrawSliderFloat("##sl_aimspeed", dl, ImVec2(left_min.x + 14.0f, y), col_avail_w, &S::aim_speed, 0.0f, 30.0f, "Speed", "%.1f", tab_font, 13.0f);
            y += 33.0f;

            // Require Right Click
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::aim_require_rclick, "Require Right Click", tab_font, 13.0f);
            y += kCheckRowPitch;

            // Show Target Info
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::aim_show_target, "Show Target Info", tab_font, 13.0f);
            y += kCheckRowPitch;

            // Team Check
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::aim_team_check, "Team Check", tab_font, 13.0f);
            y += kCheckRowPitch;

            // Range Circle + sub-options
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::aim_range_circle, "Range Circle", tab_font, 13.0f);
            {
                QuestionItem circle_items[] = {
                    MakeQColorPicker("##q_circlecol", "COLOR", reinterpret_cast<ImVec4*>(S::aim_circle_color)),
                    MakeQSliderFloat("##q_circletrans", "Transparency", &S::aim_circle_transparency, 0.0f, 1.0f, "%.2f"),
                    MakeQSliderFloat("##q_circlethick", "Thickness", &S::aim_circle_thickness, 1.0f, 10.0f, "%.1f"),
                    MakeQCheckbox("FILLED", &S::aim_circle_filled)
                };
                kb_hov |= DrawQuestionButton("##qb_circle", dl, ImVec2(left_max.x - 14.0f, y), circle_items, 4, tab_font, 13.0f);
            }
            y += kCheckRowPitch;

            // -- RIGHT GROUP: Camera & Misc --
            float ry = right_min.y + 20.0f;

            // FOV (Camera)
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::fov_enabled, "FOV", tab_font, 13.0f);
            kb_hov |= DrawKeybind("##fov_key", dl, ImVec2(right_max.x - 14.0f, ry), &S::fov_key, &S::fov_key_mode, tab_font, 13.0f);
            ry += kCheckRowPitch;
            if (S::fov_enabled)
            {
                sl_hov |= DrawSliderFloat("##sl_camerafov", dl, ImVec2(right_min.x + 14.0f, ry), col_avail_w_r, &S::fov_value, 30.0f, 120.0f, "FOV", "%.0f", tab_font, 13.0f);
                ry += 33.0f;
            }

            // Global Visual Team Check — syncs all per-feature team checks
            {
                bool prev = S::vis_team_check;
                cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::vis_team_check, "Global Team Check", tab_font, 13.0f);
                if (S::vis_team_check != prev) {
                    S::aim_team_check = S::vis_team_check;
                    S::esp_team_check = S::vis_team_check;
                    S::tracer_team_check = S::vis_team_check;
                    S::name_team_check = S::vis_team_check;
                    S::chams_team_check = S::vis_team_check;
                }
            }
            ry += kCheckRowPitch;
        }
        // ===== TAB 1: VISUALS =====
        else if (g_activeTab == 1)
        {
            // -- LEFT GROUP: Player ESP --
            float y = left_min.y + 20.0f;

            // ESP
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::esp_enabled, "ESP", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_esp", dl, ImVec2(left_max.x - 14.0f - picker_sz.x, y), picker_sz,
                reinterpret_cast<ImVec4*>(S::esp_color), nullptr, nullptr);
            y += kCheckRowPitch;

            // Mode dropdown
            {
                const char* mode_items[] = { "2D", "3D", "Skeleton" };
                dd_hov |= DrawDropdownSingle("##dd_espmode", dl, ImVec2(left_min.x + 14.0f, y), col_avail_w, &S::esp_mode, mode_items, 3, "Mode", tab_font, 13.0f);
                y += 37.0f;
            }

            // 2D mode sub-options
            if (S::esp_mode == 0)
            {
                cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::esp_bounding_box, "Bounding Box", tab_font, 13.0f);
                y += kCheckRowPitch;
                {
                    const char* style_items[] = { "Full", "Corner" };
                    dd_hov |= DrawDropdownSingle("##dd_boxstyle", dl, ImVec2(left_min.x + 14.0f, y), col_avail_w, &S::esp_box_style, style_items, 2, "Box Style", tab_font, 13.0f);
                    y += 37.0f;
                }
                cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::esp_filled, "Filled", tab_font, 13.0f);
                cp_hov |= DrawColorPicker("##col_espfill", dl, ImVec2(left_max.x - 14.0f - picker_sz.x, y), picker_sz,
                    reinterpret_cast<ImVec4*>(S::esp_fill_color), nullptr, nullptr);
                y += kCheckRowPitch;
            }

            // Health Bar
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::health_enabled, "Health Bar", tab_font, 13.0f);
            y += kCheckRowPitch;

            // Skeleton
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::skeleton_enabled, "Skeleton", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_skel", dl, ImVec2(left_max.x - 14.0f - picker_sz.x, y), picker_sz,
                reinterpret_cast<ImVec4*>(S::skeleton_color), nullptr, nullptr);
            y += kCheckRowPitch;

            // Head Dot
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::headdot_enabled, "Head Dot", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_headdot", dl, ImVec2(left_max.x - 14.0f - picker_sz.x, y), picker_sz,
                reinterpret_cast<ImVec4*>(S::headdot_color), nullptr, nullptr);
            {
                QuestionItem hd_items[] = {
                    MakeQCheckbox("FILL", &S::headdot_fill),
                    MakeQCheckbox("OUTLINE", &S::headdot_outline),
                    MakeQSliderFloat("##q_headdotsz", "Thickness", &S::headdot_thickness, 1.0f, 10.0f, "%.1f")
                };
                kb_hov |= DrawQuestionButton("##qb_headdot", dl, ImVec2(left_max.x - qb_single_swatch_off, y), hd_items, 3, tab_font, 13.0f);
            }
            y += kCheckRowPitch;

            // Team Check
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::esp_team_check, "Team Check", tab_font, 13.0f);
            y += kCheckRowPitch;

            // Distance Check
            cb_hov |= DrawCheckbox(dl, ImVec2(left_min.x + 14.0f, y), &S::esp_distance_check, "Distance Check", tab_font, 13.0f);
            {
                QuestionItem dist_items[] = {
                    MakeQSliderFloat("##q_espdistmin", "Min Range", &S::esp_distance_min, 0.0f, 256.0f, "%.0f"),
                    MakeQSliderFloat("##q_espdistmax", "Max Range", &S::esp_distance_max, 0.0f, 256.0f, "%.0f")
                };
                kb_hov |= DrawQuestionButton("##qb_espdist", dl, ImVec2(left_max.x - 14.0f, y), dist_items, 2, tab_font, 13.0f);
            }
            y += kCheckRowPitch;

            // -- RIGHT GROUP: Effects & World --
            float ry = right_min.y + 20.0f;

            // Nametags
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::name_enabled, "Nametags", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_name", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::name_color), nullptr, nullptr);
            {
                QuestionItem name_items[] = {
                    MakeQSliderFloat("##q_namescale", "Scale", &S::name_scale, 0.1f, 1.5f, "%.1f"),
                    MakeQSliderFloat("##q_nametrans", "Transparency", &S::name_bg_transparency, 0.0f, 1.0f, "%.2f"),
                    MakeQCheckbox("SHOW HEALTH", &S::name_show_health),
                    MakeQCheckbox("SHOW DISTANCE", &S::name_show_distance),
                    MakeQCheckbox("DISPLAY NAME", &S::name_use_displayname),
                    MakeQCheckbox("BACKGROUND", &S::name_show_background),
                    MakeQCheckbox("SHOW TOOL", &S::name_show_tool),
                    MakeQCheckbox("TEAM CHECK", &S::name_team_check),
                    MakeQCheckbox("DISTANCE CHECK", &S::name_distance_check),
                    MakeQSliderFloat("##q_namedistmin", "Min Range", &S::name_distance_min, 0.0f, 256.0f, "%.0f", &S::name_distance_check),
                    MakeQSliderFloat("##q_namedistmax", "Max Range", &S::name_distance_max, 0.0f, 256.0f, "%.0f", &S::name_distance_check)
                };
                kb_hov |= DrawQuestionButton("##qb_name", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), name_items, 11, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

            // Chams
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::chams_enabled, "Chams", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_chams", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::chams_color), nullptr, nullptr);
            {
                QuestionItem chams_items[] = {
                    MakeQColorPicker("##q_chamsout", "OUTLINE", reinterpret_cast<ImVec4*>(S::chams_outline_color)),
                    MakeQSliderFloat("##q_chamstrans", "Transparency", &S::chams_transparency, 0.0f, 1.0f, "%.2f"),
                    MakeQSliderFloat("##q_chamsouttr", "Outline Trans.", &S::chams_outline_transparency, 0.0f, 1.0f, "%.2f"),
                    MakeQCheckbox("THROUGH WALLS", &S::chams_render_walls),
                    MakeQCheckbox("TEAM CHECK", &S::chams_team_check)
                };
                kb_hov |= DrawQuestionButton("##qb_chams", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), chams_items, 5, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

            // Tracers
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::tracer_enabled, "Tracers", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_tracer", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::tracer_color), nullptr, nullptr);
            {
                static const char* const kTracerStartItems[] = { "Middle", "Bottom", "Mouse" };
                static const char* const kTracerEndItems[] = { "Head", "Torso" };
                QuestionItem tracer_items[] = {
                    MakeQDropdownSingle("##q_tracerstart", "Start", &S::tracer_start, kTracerStartItems, 3),
                    MakeQDropdownSingle("##q_tracerend", "End", &S::tracer_end, kTracerEndItems, 2),
                    MakeQSliderFloat("##q_tracertrans", "Transparency", &S::tracer_transparency, 0.0f, 1.0f, "%.2f"),
                    MakeQCheckbox("COLOR BY DIST", &S::tracer_color_by_distance),
                    MakeQCheckbox("SHOW BEHIND", &S::tracer_behind),
                    MakeQCheckbox("TEAM CHECK", &S::tracer_team_check),
                    MakeQCheckbox("DISTANCE CHECK", &S::tracer_distance_check),
                    MakeQSliderFloat("##q_tracerdistmin", "Min Range", &S::tracer_distance_min, 0.0f, 256.0f, "%.0f", &S::tracer_distance_check),
                    MakeQSliderFloat("##q_tracerdistmax", "Max Range", &S::tracer_distance_max, 0.0f, 256.0f, "%.0f", &S::tracer_distance_check)
                };
                kb_hov |= DrawQuestionButton("##qb_tracer", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), tracer_items, 9, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

            // Equipped Tool
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::tool_enabled, "Equipped Tool", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_tool", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::tool_color), nullptr, nullptr);
            ry += kCheckRowPitch;

            // WhiteHits
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::whitehits_enabled, "WhiteHits", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_whitehits", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::whitehits_color), nullptr, nullptr);
            {
                QuestionItem wh_items[] = {
                    MakeQSliderFloat("##q_whdur", "Duration", &S::whitehits_duration, 0.05f, 1.0f, "%.2f")
                };
                kb_hov |= DrawQuestionButton("##qb_whitehits", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), wh_items, 1, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

            // Footsteps
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::footsteps_enabled, "Footsteps", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_footsteps", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::footsteps_color), nullptr, nullptr);
            {
                QuestionItem fs_items[] = {
                    MakeQSliderFloat("##q_fsrad", "Radius", &S::footsteps_radius, 0.1f, 3.0f, "%.1f"),
                    MakeQSliderFloat("##q_fslife", "Lifetime", &S::footsteps_lifetime, 0.5f, 10.0f, "%.1f"),
                    MakeQSliderFloat("##q_fsmax", "Max Count", &S::footsteps_max, 1.0f, 256.0f, "%.0f")
                };
                kb_hov |= DrawQuestionButton("##qb_footsteps", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), fs_items, 3, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

            // Kit ESP
            cb_hov |= DrawCheckbox(dl, ImVec2(right_min.x + 14.0f, ry), &S::kitesp_enabled, "Kit ESP", tab_font, 13.0f);
            cp_hov |= DrawColorPicker("##col_kitesp", dl, ImVec2(right_max.x - 14.0f - picker_sz.x, ry), picker_sz,
                reinterpret_cast<ImVec4*>(S::kitesp_color), nullptr, nullptr);
            {
                QuestionItem ke_items[] = {
                    MakeQSliderFloat("##q_kitscale", "Scale", &S::kitesp_scale, 0.1f, 1.5f, "%.1f"),
                    MakeQCheckbox("SHOW DISTANCE", &S::kitesp_show_distance),
                    MakeQCheckbox("BACKGROUND", &S::kitesp_background),
                    MakeQCheckbox("ALCHEMIST", &S::kitesp_alchemist),
                    MakeQCheckbox("BEEKEEPER", &S::kitesp_bee),
                    MakeQCheckbox("ELDER TREE", &S::kitesp_bigman),
                    MakeQCheckbox("GHOST CATCHER", &S::kitesp_ghost),
                    MakeQCheckbox("METAL DETECTOR", &S::kitesp_metal),
                    MakeQCheckbox("SHEEP HERDER", &S::kitesp_sheep),
                    MakeQCheckbox("SORCERER", &S::kitesp_sorcerer),
                    MakeQCheckbox("STAR COLLECTOR", &S::kitesp_star)
                };
                kb_hov |= DrawQuestionButton("##qb_kitesp", dl, ImVec2(right_max.x - qb_single_swatch_off, ry), ke_items, 11, tab_font, 13.0f);
            }
            ry += kCheckRowPitch;

        }
        UpdateThemeFromMask();

        bool popup_open = ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
        bool hovering = (m.x >= pos.x && m.x <= max.x && m.y >= pos.y && m.y <= max.y);
        bool overPanel = g_settingsOpen && g_setPos.x >= 0.0f && (m.x >= g_setPos.x && m.x <= g_setPos.x + g_setSize.x && m.y >= g_setPos.y && m.y <= g_setPos.y + g_setSize.y);
        bool overTheme = g_themeOpen && g_themePos.x >= 0.0f && (m.x >= g_themePos.x && m.x <= g_themePos.x + g_themeSize.x && m.y >= g_themePos.y && m.y <= g_themePos.y + g_themeSize.y);
        if (!g_dragging && ImGui::IsMouseClicked(0) && hovering && !popup_open && !overPanel && !overTheme && g_activeBindId == nullptr && !tab_hovered && !g_navBarHov && !cb_hov && !cp_hov && !kb_hov && !sl_hov && !btn_hov && !ti_hov && !dd_hov)
        {
            g_dragging = true;
            g_menuAnim = 1.0f;
            g_dragOff = ImVec2(m.x - g_pos.x, m.y - g_pos.y);
        }
        if (g_dragging)
        {
            if (ImGui::IsMouseDown(0))
            {
                g_pos.x = m.x - g_dragOff.x;
                g_pos.y = m.y - g_dragOff.y;
            }
            else
            {
                g_dragging = false;
            }
        }
        if (ease < 0.999f)
        {
            for (int i = 0; i < dl->VtxBuffer.Size; i++)
            {
                ImDrawVert& v = dl->VtxBuffer.Data[i];
                ImU32 a = (v.col >> 24) & 0xFF;
                a = (ImU32)((float)a * ease);
                v.col = (v.col & 0x00FFFFFF) | (a << 24);
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(6);
}

inline std::string Base64Encode(const unsigned char* data, size_t len)
{
    static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(((len + 2) / 3) * 4);
    for (size_t i = 0; i < len; i += 3)
    {
        unsigned int val = (data[i] << 16);
        if (i + 1 < len) val |= (data[i + 1] << 8);
        if (i + 2 < len) val |= data[i + 2];
        out.push_back(tbl[(val >> 18) & 0x3F]);
        out.push_back(tbl[(val >> 12) & 0x3F]);
        out.push_back((i + 1 < len) ? tbl[(val >> 6) & 0x3F] : '=');
        out.push_back((i + 2 < len) ? tbl[val & 0x3F] : '=');
    }
    return out;
}

inline std::string Base64Decode(const std::string& in)
{
    std::string out;
    std::vector<int> T(256, -1);
    static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for (int i = 0; i < 64; i++) T[(unsigned char)tbl[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : in)
    {
        if (T[c] == -1) continue;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

inline void GetDesktopConfigDir(char* out_path, size_t max_len)
{
    char desktop[MAX_PATH] = "";
    if (SHGetFolderPathA(nullptr, CSIDL_DESKTOPDIRECTORY, nullptr, 0, desktop) != S_OK)
    {
        char user_prof[MAX_PATH] = "";
        GetEnvironmentVariableA("USERPROFILE", user_prof, sizeof(user_prof));
        if (user_prof[0] != '\0')
        {
            snprintf(desktop, sizeof(desktop), "%s\\Desktop", user_prof);
        }
        else
        {
            snprintf(desktop, sizeof(desktop), ".");
        }
    }
    snprintf(out_path, max_len, "%s\\saved-configs", desktop);
    CreateDirectoryA(out_path, nullptr);
}

inline std::vector<std::string> g_configFiles;
inline int g_configSelectedIdx = 0;

inline void RefreshConfigList()
{
    g_configFiles.clear();
    char dir[MAX_PATH];
    GetDesktopConfigDir(dir, sizeof(dir));
    char search_pattern[MAX_PATH];
    snprintf(search_pattern, sizeof(search_pattern), "%s\\*.config", dir);

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search_pattern, &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::string fname = fd.cFileName;
                size_t dot = fname.rfind(".config");
                if (dot != std::string::npos)
                {
                    fname = fname.substr(0, dot);
                }
                if (!fname.empty())
                {
                    g_configFiles.push_back(fname);
                }
            }
        } while (FindNextFileA(hFind, &fd));
        FindClose(hFind);
    }
    if (g_configFiles.empty())
    {
        g_configFiles.push_back("default");
    }
    if (g_configSelectedIdx < 0) g_configSelectedIdx = 0;
    if (g_configSelectedIdx >= (int)g_configFiles.size())
    {
        g_configSelectedIdx = 0;
    }
}

inline bool DeleteConfigFile(const char* name)
{
    if (!name || name[0] == '\0')
    {
        return false;
    }
    char dir[MAX_PATH];
    GetDesktopConfigDir(dir, sizeof(dir));
    char file_name[128];
    if (strstr(name, ".config"))
    {
        snprintf(file_name, sizeof(file_name), "%s", name);
    }
    else
    {
        snprintf(file_name, sizeof(file_name), "%s.config", name);
    }
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s\\%s", dir, file_name);
    BOOL ok = DeleteFileA(full_path);
    RefreshConfigList();
    return ok != FALSE;
}

inline std::string BuildConfigPayload(const char* name)
{
    std::string payload;
    payload.reserve(8192);

    char buf[512];
    snprintf(buf, sizeof(buf), "[CONFIG_META]\nversion=2.0.0\ntype=rpbrx\nname=%s\n\n[SETTINGS]\n", (name && name[0] != '\0') ? name : "default");
    payload += buf;

    // -- Aim Assist --
    snprintf(buf, sizeof(buf), "aim_enabled=%d\naim_key=%d\naim_key_mode=%d\naim_part=%d\naim_fov=%.2f\naim_speed=%.2f\naim_range_circle=%d\naim_require_rclick=%d\naim_show_target=%d\naim_team_check=%d\n",
        S::aim_enabled ? 1 : 0, S::aim_key, S::aim_key_mode, S::aim_part, (double)S::aim_fov, (double)S::aim_speed,
        S::aim_range_circle ? 1 : 0, S::aim_require_rclick ? 1 : 0, S::aim_show_target ? 1 : 0, S::aim_team_check ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "aim_circle_color=%.4f,%.4f,%.4f,%.4f\naim_circle_transparency=%.2f\naim_circle_thickness=%.2f\naim_circle_filled=%d\n",
        S::aim_circle_color[0], S::aim_circle_color[1], S::aim_circle_color[2], S::aim_circle_color[3],
        (double)S::aim_circle_transparency, (double)S::aim_circle_thickness, S::aim_circle_filled ? 1 : 0);
    payload += buf;

    // -- ESP --
    snprintf(buf, sizeof(buf), "esp_enabled=%d\nesp_mode=%d\nesp_bounding_box=%d\nesp_filled=%d\nesp_box_style=%d\nesp_team_check=%d\nesp_distance_check=%d\nesp_distance_min=%.2f\nesp_distance_max=%.2f\n",
        S::esp_enabled ? 1 : 0, S::esp_mode, S::esp_bounding_box ? 1 : 0, S::esp_filled ? 1 : 0,
        S::esp_box_style, S::esp_team_check ? 1 : 0, S::esp_distance_check ? 1 : 0,
        (double)S::esp_distance_min, (double)S::esp_distance_max);
    payload += buf;

    snprintf(buf, sizeof(buf), "esp_color=%.4f,%.4f,%.4f,%.4f\nesp_fill_color=%.4f,%.4f,%.4f,%.4f\n",
        S::esp_color[0], S::esp_color[1], S::esp_color[2], S::esp_color[3],
        S::esp_fill_color[0], S::esp_fill_color[1], S::esp_fill_color[2], S::esp_fill_color[3]);
    payload += buf;

    // -- Health, Skeleton --
    snprintf(buf, sizeof(buf), "health_enabled=%d\nskeleton_enabled=%d\n",
        S::health_enabled ? 1 : 0, S::skeleton_enabled ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "skeleton_color=%.4f,%.4f,%.4f,%.4f\n",
        S::skeleton_color[0], S::skeleton_color[1], S::skeleton_color[2], S::skeleton_color[3]);
    payload += buf;

    // -- Chams --
    snprintf(buf, sizeof(buf), "chams_enabled=%d\nchams_transparency=%.2f\nchams_outline_transparency=%.2f\nchams_render_walls=%d\nchams_team_check=%d\n",
        S::chams_enabled ? 1 : 0, (double)S::chams_transparency, (double)S::chams_outline_transparency,
        S::chams_render_walls ? 1 : 0, S::chams_team_check ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "chams_color=%.4f,%.4f,%.4f,%.4f\nchams_outline_color=%.4f,%.4f,%.4f,%.4f\n",
        S::chams_color[0], S::chams_color[1], S::chams_color[2], S::chams_color[3],
        S::chams_outline_color[0], S::chams_outline_color[1], S::chams_outline_color[2], S::chams_outline_color[3]);
    payload += buf;

    // -- Nametags --
    snprintf(buf, sizeof(buf), "name_enabled=%d\nname_scale=%.2f\nname_bg_transparency=%.2f\nname_show_health=%d\nname_show_distance=%d\nname_use_displayname=%d\nname_show_background=%d\nname_show_tool=%d\nname_team_check=%d\nname_distance_check=%d\nname_distance_min=%.2f\nname_distance_max=%.2f\n",
        S::name_enabled ? 1 : 0, (double)S::name_scale, (double)S::name_bg_transparency,
        S::name_show_health ? 1 : 0, S::name_show_distance ? 1 : 0, S::name_use_displayname ? 1 : 0,
        S::name_show_background ? 1 : 0, S::name_show_tool ? 1 : 0, S::name_team_check ? 1 : 0,
        S::name_distance_check ? 1 : 0, (double)S::name_distance_min, (double)S::name_distance_max);
    payload += buf;

    snprintf(buf, sizeof(buf), "name_color=%.4f,%.4f,%.4f,%.4f\n",
        S::name_color[0], S::name_color[1], S::name_color[2], S::name_color[3]);
    payload += buf;

    // -- Kit ESP --
    snprintf(buf, sizeof(buf), "kitesp_enabled=%d\nkitesp_scale=%.2f\nkitesp_background=%d\nkitesp_show_distance=%d\nkitesp_alchemist=%d\nkitesp_bee=%d\nkitesp_bigman=%d\nkitesp_ghost=%d\nkitesp_metal=%d\nkitesp_sheep=%d\nkitesp_sorcerer=%d\nkitesp_star=%d\n",
        S::kitesp_enabled ? 1 : 0, (double)S::kitesp_scale, S::kitesp_background ? 1 : 0,
        S::kitesp_show_distance ? 1 : 0,
        S::kitesp_alchemist ? 1 : 0, S::kitesp_bee ? 1 : 0, S::kitesp_bigman ? 1 : 0,
        S::kitesp_ghost ? 1 : 0, S::kitesp_metal ? 1 : 0, S::kitesp_sheep ? 1 : 0,
        S::kitesp_sorcerer ? 1 : 0, S::kitesp_star ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "kitesp_color=%.4f,%.4f,%.4f,%.4f\nkitesp_bg_color=%.4f,%.4f,%.4f,%.4f\n",
        S::kitesp_color[0], S::kitesp_color[1], S::kitesp_color[2], S::kitesp_color[3],
        S::kitesp_bg_color[0], S::kitesp_bg_color[1], S::kitesp_bg_color[2], S::kitesp_bg_color[3]);
    payload += buf;

    // -- WhiteHits --
    snprintf(buf, sizeof(buf), "whitehits_enabled=%d\nwhitehits_duration=%.2f\n",
        S::whitehits_enabled ? 1 : 0, (double)S::whitehits_duration);
    payload += buf;

    snprintf(buf, sizeof(buf), "whitehits_color=%.4f,%.4f,%.4f,%.4f\n",
        S::whitehits_color[0], S::whitehits_color[1], S::whitehits_color[2], S::whitehits_color[3]);
    payload += buf;

    // -- FOV --
    snprintf(buf, sizeof(buf), "fov_enabled=%d\nfov_value=%.2f\nfov_key=%d\nfov_key_mode=%d\n",
        S::fov_enabled ? 1 : 0, (double)S::fov_value, S::fov_key, S::fov_key_mode);
    payload += buf;

    // -- Tracers --
    snprintf(buf, sizeof(buf), "tracer_enabled=%d\ntracer_start=%d\ntracer_end=%d\ntracer_transparency=%.2f\ntracer_color_by_distance=%d\ntracer_behind=%d\ntracer_team_check=%d\ntracer_distance_check=%d\ntracer_distance_min=%.2f\ntracer_distance_max=%.2f\n",
        S::tracer_enabled ? 1 : 0, S::tracer_start, S::tracer_end,
        (double)S::tracer_transparency, S::tracer_color_by_distance ? 1 : 0, S::tracer_behind ? 1 : 0,
        S::tracer_team_check ? 1 : 0, S::tracer_distance_check ? 1 : 0,
        (double)S::tracer_distance_min, (double)S::tracer_distance_max);
    payload += buf;

    snprintf(buf, sizeof(buf), "tracer_color=%.4f,%.4f,%.4f,%.4f\n",
        S::tracer_color[0], S::tracer_color[1], S::tracer_color[2], S::tracer_color[3]);
    payload += buf;

    // -- Equipped Tool --
    snprintf(buf, sizeof(buf), "tool_enabled=%d\n", S::tool_enabled ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "tool_color=%.4f,%.4f,%.4f,%.4f\n",
        S::tool_color[0], S::tool_color[1], S::tool_color[2], S::tool_color[3]);
    payload += buf;

    // -- Head Dot --
    snprintf(buf, sizeof(buf), "headdot_enabled=%d\nheaddot_fill=%d\nheaddot_outline=%d\nheaddot_thickness=%.2f\n",
        S::headdot_enabled ? 1 : 0, S::headdot_fill ? 1 : 0, S::headdot_outline ? 1 : 0, (double)S::headdot_thickness);
    payload += buf;

    snprintf(buf, sizeof(buf), "headdot_color=%.4f,%.4f,%.4f,%.4f\nheaddot_outline_color=%.4f,%.4f,%.4f,%.4f\n",
        S::headdot_color[0], S::headdot_color[1], S::headdot_color[2], S::headdot_color[3],
        S::headdot_outline_color[0], S::headdot_outline_color[1], S::headdot_outline_color[2], S::headdot_outline_color[3]);
    payload += buf;

    // -- Footsteps --
    snprintf(buf, sizeof(buf), "footsteps_enabled=%d\nfootsteps_radius=%.2f\nfootsteps_lifetime=%.2f\nfootsteps_max=%.0f\n",
        S::footsteps_enabled ? 1 : 0, (double)S::footsteps_radius, (double)S::footsteps_lifetime, (double)S::footsteps_max);
    payload += buf;

    snprintf(buf, sizeof(buf), "footsteps_color=%.4f,%.4f,%.4f,%.4f\n",
        S::footsteps_color[0], S::footsteps_color[1], S::footsteps_color[2], S::footsteps_color[3]);
    payload += buf;

    // -- Global --
    snprintf(buf, sizeof(buf), "vis_team_check=%d\n", S::vis_team_check ? 1 : 0);
    payload += buf;

    // -- UI --
    snprintf(buf, sizeof(buf), "user_name=%s\n", g_textinput1);
    payload += buf;

    snprintf(buf, sizeof(buf), "vsync=%d\nstream_proof=%d\nfps_cap=%d\nwatermark=%d\nwatermark_fps=%d\nwatermark_players=%d\nwatermark_date=%d\nkeybinds=%d\n",
        g_vsync ? 1 : 0, g_streamProof ? 1 : 0, g_fpsCap, g_watermark ? 1 : 0,
        g_watermarkFps ? 1 : 0, g_watermarkPlayers ? 1 : 0, g_watermarkDate ? 1 : 0, g_keybinds ? 1 : 0);
    payload += buf;

    snprintf(buf, sizeof(buf), "binds_pos=%.1f,%.1f\n", g_bindsPos.x, g_bindsPos.y);
    payload += buf;

    snprintf(buf, sizeof(buf), "theme_mask=%d\n", g_themeMask);
    payload += buf;

    snprintf(buf, sizeof(buf), "accent=%.4f,%.4f,%.4f,%.4f\ncol_frame=%.4f,%.4f,%.4f,%.4f\ncol_group=%.4f,%.4f,%.4f,%.4f\ncol_ctrl=%.4f,%.4f,%.4f,%.4f\ncol_ctrl_hov=%.4f,%.4f,%.4f,%.4f\ncol_border=%.4f,%.4f,%.4f,%.4f\ncol_text=%.4f,%.4f,%.4f,%.4f\ncol_text_dim=%.4f,%.4f,%.4f,%.4f\n",
        g_accent1.x, g_accent1.y, g_accent1.z, g_accent1.w,
        g_colFrame.x, g_colFrame.y, g_colFrame.z, g_colFrame.w,
        g_colGroup.x, g_colGroup.y, g_colGroup.z, g_colGroup.w,
        g_colCtrl.x, g_colCtrl.y, g_colCtrl.z, g_colCtrl.w,
        g_colCtrlHov.x, g_colCtrlHov.y, g_colCtrlHov.z, g_colCtrlHov.w,
        g_colBorder.x, g_colBorder.y, g_colBorder.z, g_colBorder.w,
        g_colText.x, g_colText.y, g_colText.z, g_colText.w,
        g_colTextDim.x, g_colTextDim.y, g_colTextDim.z, g_colTextDim.w);
    payload += buf;

    payload += "\n[DATA_CHUNK]\n";
    for (int i = 0; i < 200; i++)
    {
        snprintf(buf, sizeof(buf), "chunk_%03d=a8f7c9e1b2d34567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef\n", i);
        payload += buf;
    }

    return Base64Encode(reinterpret_cast<const unsigned char*>(payload.data()), payload.size());
}

inline bool SaveConfig(const char* name)
{
    char dir[MAX_PATH];
    GetDesktopConfigDir(dir, sizeof(dir));
    char file_name[128];
    if (!name || name[0] == '\0')
    {
        snprintf(file_name, sizeof(file_name), "default.config");
    }
    else
    {
        if (strstr(name, ".config"))
        {
            snprintf(file_name, sizeof(file_name), "%s", name);
        }
        else
        {
            snprintf(file_name, sizeof(file_name), "%s.config", name);
        }
    }
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s\\%s", dir, file_name);

    std::string b64 = BuildConfigPayload(file_name);

    FILE* fp = nullptr;
    if (fopen_s(&fp, full_path, "wb") != 0 || !fp)
    {
        return false;
    }
    fwrite(b64.data(), 1, b64.size(), fp);
    fclose(fp);
    RefreshConfigList();
    return true;
}

inline bool ExportConfig(const char* name)
{
    std::string b64 = BuildConfigPayload(name);
    ImGui::SetClipboardText(b64.c_str());
    return true;
}

inline bool ApplyConfigRaw(const std::string& raw)
{
    if (raw.empty())
    {
        return false;
    }

    auto get_int = [&](const char* key, int& out) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            out = atoi(raw.c_str() + p);
        }
    };

    auto get_bool = [&](const char* key, bool& out) {
        int v = 0;
        get_int(key, v);
        out = (v != 0);
    };

    auto get_col = [&](const char* key, ImVec4& out) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            float r = 0, g = 0, b = 0, a = 1.0f;
            const char* s = raw.c_str() + p;
            char* end = nullptr;
            r = strtof(s, &end);
            if (end && *end == ',') {
                s = end + 1;
                g = strtof(s, &end);
                if (end && *end == ',') {
                    s = end + 1;
                    b = strtof(s, &end);
                    if (end && *end == ',') {
                        s = end + 1;
                        a = strtof(s, &end);
                    }
                }
            }
            out = ImVec4(r, g, b, a);
        }
    };

    auto get_str = [&](const char* key, char* out, size_t max_l) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            size_t end_p = raw.find('\n', p);
            if (end_p == std::string::npos) end_p = raw.size();
            size_t l = end_p - p;
            if (l >= max_l) l = max_l - 1;
            memcpy(out, raw.data() + p, l);
            out[l] = '\0';
        }
    };

    auto get_float = [&](const char* key, float& out) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            out = strtof(raw.c_str() + p, nullptr);
        }
    };

    auto get_col_arr = [&](const char* key, float arr[4]) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            float r = 0, g = 0, b = 0, a = 1.0f;
            const char* s = raw.c_str() + p;
            char* end = nullptr;
            r = strtof(s, &end);
            if (end && *end == ',') {
                s = end + 1;
                g = strtof(s, &end);
                if (end && *end == ',') {
                    s = end + 1;
                    b = strtof(s, &end);
                    if (end && *end == ',') {
                        s = end + 1;
                        a = strtof(s, &end);
                    }
                }
            }
            arr[0] = r; arr[1] = g; arr[2] = b; arr[3] = a;
        }
    };

    auto get_vec2 = [&](const char* key, ImVec2& out) {
        std::string tag = std::string(key) + "=";
        size_t p = raw.find(tag);
        if (p != std::string::npos) {
            p += tag.size();
            const char* s = raw.data() + p;
            char* end = nullptr;
            float vx = strtof(s, &end);
            if (end && *end == ',') {
                float vy = strtof(end + 1, nullptr);
                out = ImVec2(vx, vy);
            }
        }
    };

    // -- Aim Assist --
    get_bool("aim_enabled", S::aim_enabled);
    get_int("aim_key", S::aim_key);
    get_int("aim_key_mode", S::aim_key_mode);
    get_int("aim_part", S::aim_part);
    get_float("aim_fov", S::aim_fov);
    get_float("aim_speed", S::aim_speed);
    get_bool("aim_range_circle", S::aim_range_circle);
    get_col_arr("aim_circle_color", S::aim_circle_color);
    get_float("aim_circle_transparency", S::aim_circle_transparency);
    get_float("aim_circle_thickness", S::aim_circle_thickness);
    get_bool("aim_circle_filled", S::aim_circle_filled);
    get_bool("aim_require_rclick", S::aim_require_rclick);
    get_bool("aim_show_target", S::aim_show_target);
    get_bool("aim_team_check", S::aim_team_check);

    // -- ESP --
    get_bool("esp_enabled", S::esp_enabled);
    get_int("esp_mode", S::esp_mode);
    get_col_arr("esp_color", S::esp_color);
    get_bool("esp_bounding_box", S::esp_bounding_box);
    get_bool("esp_filled", S::esp_filled);
    get_col_arr("esp_fill_color", S::esp_fill_color);
    get_int("esp_box_style", S::esp_box_style);
    get_bool("esp_team_check", S::esp_team_check);
    get_bool("esp_distance_check", S::esp_distance_check);
    get_float("esp_distance_min", S::esp_distance_min);
    get_float("esp_distance_max", S::esp_distance_max);

    // -- Health, Skeleton --
    get_bool("health_enabled", S::health_enabled);
    get_bool("skeleton_enabled", S::skeleton_enabled);
    get_col_arr("skeleton_color", S::skeleton_color);

    // -- Chams --
    get_bool("chams_enabled", S::chams_enabled);
    get_col_arr("chams_color", S::chams_color);
    get_col_arr("chams_outline_color", S::chams_outline_color);
    get_float("chams_transparency", S::chams_transparency);
    get_float("chams_outline_transparency", S::chams_outline_transparency);
    get_bool("chams_render_walls", S::chams_render_walls);
    get_bool("chams_team_check", S::chams_team_check);

    // -- Nametags --
    get_bool("name_enabled", S::name_enabled);
    get_col_arr("name_color", S::name_color);
    get_float("name_scale", S::name_scale);
    get_float("name_bg_transparency", S::name_bg_transparency);
    get_bool("name_show_health", S::name_show_health);
    get_bool("name_show_distance", S::name_show_distance);
    get_bool("name_use_displayname", S::name_use_displayname);
    get_bool("name_show_background", S::name_show_background);
    get_bool("name_show_tool", S::name_show_tool);
    get_bool("name_team_check", S::name_team_check);
    get_bool("name_distance_check", S::name_distance_check);
    get_float("name_distance_min", S::name_distance_min);
    get_float("name_distance_max", S::name_distance_max);

    // -- Kit ESP --
    get_bool("kitesp_enabled", S::kitesp_enabled);
    get_col_arr("kitesp_color", S::kitesp_color);
    get_float("kitesp_scale", S::kitesp_scale);
    get_bool("kitesp_background", S::kitesp_background);
    get_bool("kitesp_show_distance", S::kitesp_show_distance);
    get_col_arr("kitesp_bg_color", S::kitesp_bg_color);
    get_bool("kitesp_alchemist", S::kitesp_alchemist);
    get_bool("kitesp_bee", S::kitesp_bee);
    get_bool("kitesp_bigman", S::kitesp_bigman);
    get_bool("kitesp_ghost", S::kitesp_ghost);
    get_bool("kitesp_metal", S::kitesp_metal);
    get_bool("kitesp_sheep", S::kitesp_sheep);
    get_bool("kitesp_sorcerer", S::kitesp_sorcerer);
    get_bool("kitesp_star", S::kitesp_star);

    // -- WhiteHits --
    get_bool("whitehits_enabled", S::whitehits_enabled);
    get_col_arr("whitehits_color", S::whitehits_color);
    get_float("whitehits_duration", S::whitehits_duration);

    // -- FOV --
    get_bool("fov_enabled", S::fov_enabled);
    get_float("fov_value", S::fov_value);
    get_int("fov_key", S::fov_key);
    get_int("fov_key_mode", S::fov_key_mode);

    // -- Tracers --
    get_bool("tracer_enabled", S::tracer_enabled);
    get_col_arr("tracer_color", S::tracer_color);
    get_int("tracer_start", S::tracer_start);
    get_int("tracer_end", S::tracer_end);
    get_float("tracer_transparency", S::tracer_transparency);
    get_bool("tracer_color_by_distance", S::tracer_color_by_distance);
    get_bool("tracer_behind", S::tracer_behind);
    get_bool("tracer_team_check", S::tracer_team_check);
    get_bool("tracer_distance_check", S::tracer_distance_check);
    get_float("tracer_distance_min", S::tracer_distance_min);
    get_float("tracer_distance_max", S::tracer_distance_max);

    // -- Equipped Tool --
    get_bool("tool_enabled", S::tool_enabled);
    get_col_arr("tool_color", S::tool_color);

    // -- Head Dot --
    get_bool("headdot_enabled", S::headdot_enabled);
    get_col_arr("headdot_color", S::headdot_color);
    get_bool("headdot_fill", S::headdot_fill);
    get_bool("headdot_outline", S::headdot_outline);
    get_col_arr("headdot_outline_color", S::headdot_outline_color);
    get_float("headdot_thickness", S::headdot_thickness);

    // -- Footsteps --
    get_bool("footsteps_enabled", S::footsteps_enabled);
    get_col_arr("footsteps_color", S::footsteps_color);
    get_float("footsteps_radius", S::footsteps_radius);
    get_float("footsteps_lifetime", S::footsteps_lifetime);
    get_float("footsteps_max", S::footsteps_max);

    // -- Global --
    get_bool("vis_team_check", S::vis_team_check);

    // -- UI --
    get_str("user_name", g_textinput1, sizeof(g_textinput1));
    get_bool("vsync", g_vsync);
    get_bool("stream_proof", g_streamProof);
    get_int("fps_cap", g_fpsCap);
    get_bool("watermark", g_watermark);
    get_bool("watermark_fps", g_watermarkFps);
    get_bool("watermark_players", g_watermarkPlayers);
    get_bool("watermark_date", g_watermarkDate);
    get_bool("keybinds", g_keybinds);
    get_vec2("binds_pos", g_bindsPos);
    get_int("theme_mask", g_themeMask);
    get_col("accent", g_accent1);
    get_col("col_frame", g_colFrame);
    get_col("col_group", g_colGroup);
    get_col("col_ctrl", g_colCtrl);
    get_col("col_ctrl_hov", g_colCtrlHov);
    get_col("col_border", g_colBorder);
    get_col("col_text", g_colText);
    get_col("col_text_dim", g_colTextDim);

    return true;
}

inline bool LoadConfig(const char* name)
{
    char dir[MAX_PATH];
    GetDesktopConfigDir(dir, sizeof(dir));
    char file_name[128];
    if (!name || name[0] == '\0')
    {
        snprintf(file_name, sizeof(file_name), "default.config");
    }
    else
    {
        if (strstr(name, ".config"))
        {
            snprintf(file_name, sizeof(file_name), "%s", name);
        }
        else
        {
            snprintf(file_name, sizeof(file_name), "%s.config", name);
        }
    }
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s\\%s", dir, file_name);

    FILE* fp = nullptr;
    if (fopen_s(&fp, full_path, "rb") != 0 || !fp)
    {
        return false;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (sz <= 0)
    {
        fclose(fp);
        return false;
    }
    std::string b64;
    b64.resize((size_t)sz);
    fread(&b64[0], 1, (size_t)sz, fp);
    fclose(fp);

    std::string raw = Base64Decode(b64);
    return ApplyConfigRaw(raw);
}

inline bool ImportConfig()
{
    const char* clip = ImGui::GetClipboardText();
    if (!clip || clip[0] == '\0')
    {
        return false;
    }
    std::string raw = Base64Decode(clip);
    if (raw.empty() || raw.find("[CONFIG_META]") == std::string::npos)
    {
        return false;
    }
    return ApplyConfigRaw(raw);
}

inline bool DrawConfigDropdown(const char* id, ImDrawList* dl, ImVec2 pos, float width, int* selected_idx, const std::vector<std::string>& items, const char* label = nullptr, ImFont* font = nullptr, float font_size = 13.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 m = io.MousePos;
    ImFont* f = font ? font : ImGui::GetFont();
    float fs = (font_size > 0.0f) ? font_size : 13.0f;

    float x0 = floorf(pos.x);
    float x1 = floorf(pos.x + width);

    bool has_label = (label && label[0] != '\0');
    float box_y0 = has_label ? floorf(pos.y + kLabelBandH) : floorf(pos.y);
    float box_h = 20.0f;
    float box_y1 = box_y0 + box_h;

    const char* popup_id = id;
    bool popup_open = ImGui::IsPopupOpen(popup_id);

    bool is_box_hov = (m.x >= x0 && m.x <= x1 && m.y >= box_y0 && m.y <= box_y1);
    bool dd_modal = IsModalActive();

    if (is_box_hov && !dd_modal)
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0))
        {
            RefreshConfigList();
            ImGui::OpenPopup(popup_id);
        }
    }

    const ImU32 border_dark  = IM_COL32(0, 0, 0, 255);
    const ImU32 border_light = BorderU32();

    if (has_label)
    {
        float text_y = floorf(pos.y - kLabelTextRise);
        ImU32 col_lbl = popup_open ? TextU32() : TextDimU32();
        DrawOutlinedText(dl, f, fs, ImVec2(x0, text_y), col_lbl, border_dark, label);
    }

    dl->AddRectFilled(ImVec2(x0, box_y0), ImVec2(x1, box_y1), border_dark);
    dl->AddRectFilled(ImVec2(x0 + 1.0f, box_y0 + 1.0f), ImVec2(x1 - 1.0f, box_y1 - 1.0f), popup_open ? BorderHovU32() : border_light);
    dl->AddRectFilled(ImVec2(x0 + 2.0f, box_y0 + 2.0f), ImVec2(x1 - 2.0f, box_y1 - 2.0f), popup_open ? CtrlHovU32() : CtrlBgU32());

    const char* display_str = "-";
    if (!items.empty() && *selected_idx >= 0 && *selected_idx < (int)items.size())
    {
        display_str = items[*selected_idx].c_str();
    }

    ImVec2 text_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, display_str);
    float tx = x0 + 8.0f;
    float ty = floorf(box_y0 + (box_h - text_sz.y) * 0.5f);
    dl->PushClipRect(ImVec2(tx, box_y0 + 2.0f), ImVec2(x1 - 18.0f, box_y1 - 2.0f), true);
    DrawOutlinedText(dl, f, fs, ImVec2(tx, ty), popup_open ? TextU32() : TextDimU32(), border_dark, display_str);
    dl->PopClipRect();

    float ax = floorf(x1 - 10.0f);
    float ay = floorf(box_y0 + (box_h - 3.0f) * 0.5f);
    ImU32 arrow_col = popup_open ? TextU32() : TextDimU32();
    dl->AddRectFilled(ImVec2(ax - 2.0f, ay), ImVec2(ax + 3.0f, ay + 1.0f), arrow_col);
    dl->AddRectFilled(ImVec2(ax - 1.0f, ay + 1.0f), ImVec2(ax + 2.0f, ay + 2.0f), arrow_col);
    dl->AddRectFilled(ImVec2(ax, ay + 2.0f), ImVec2(ax + 1.0f, ay + 3.0f), arrow_col);

    float item_h = 18.0f;
    float pad_y = 2.0f;
    int show_count = (int)items.size();
    if (show_count < 1) show_count = 1;
    if (show_count > 6) show_count = 6;
    float popup_h = pad_y * 2.0f + (float)show_count * item_h;
    float popup_w = width;

    ImVec2 popup_pos(x0, floorf(box_y1 + 2.0f));
    ImGui::SetNextWindowPos(popup_pos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(popup_w, popup_h), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(30.0f / 255.0f, 36.0f / 255.0f, 48.0f / 255.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoMove))
    {
        ImDrawList* pdl = ImGui::GetWindowDrawList();
        ImVec2 w_min = ImGui::GetWindowPos();
        w_min.x = floorf(w_min.x);
        w_min.y = floorf(w_min.y);
        ImVec2 w_max = ImVec2(w_min.x + popup_w, w_min.y + popup_h);

        DrawPixelBox(pdl, w_min, w_max, CtrlBgU32(), border_light, border_dark);
        ImGui::Dummy(ImVec2(popup_w, popup_h));

        for (int i = 0; i < (int)items.size(); i++)
        {
            float it_y0 = floorf(w_min.y + pad_y + (float)i * item_h);
            float it_y1 = it_y0 + item_h;
            float it_x0 = w_min.x + 2.0f;
            float it_x1 = w_max.x - 2.0f;

            bool is_selected = (*selected_idx == i);
            bool it_hov = (m.x >= it_x0 && m.x <= it_x1 && m.y >= it_y0 && m.y <= it_y1);
            if (it_hov)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), is_selected ? CtrlHovU32() : CtrlBgU32());
                if (ImGui::IsMouseClicked(0))
                {
                    *selected_idx = i;
                    snprintf(g_configName, sizeof(g_configName), "%s", items[i].c_str());
                    ImGui::CloseCurrentPopup();
                }
            }
            else if (is_selected)
            {
                pdl->AddRectFilled(ImVec2(it_x0, it_y0), ImVec2(it_x1, it_y1), CtrlHovU32());
            }

            ImU32 it_text_col = (is_selected || it_hov) ? TextU32() : TextDimU32();
            ImVec2 it_sz = f->CalcTextSizeA(fs, FLT_MAX, 0.0f, items[i].c_str());
            float it_tx = it_x0 + 8.0f;
            float it_ty = floorf(it_y0 + (item_h - it_sz.y) * 0.5f);
            DrawOutlinedText(pdl, f, fs, ImVec2(it_tx, it_ty), it_text_col, border_dark, items[i].c_str());
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    popup_open = ImGui::IsPopupOpen(popup_id);
    float total_h = has_label ? kLabeledBoxH : 20.0f;
    bool is_total_hov = (m.x >= x0 && m.x <= x1 && m.y >= pos.y - kLabelTextRise && m.y <= pos.y + total_h);
    return popup_open || (!dd_modal && is_total_hov);
}

inline void RenderSettingsPanel()
{
    if (!g_settingsOpen)
    {
        g_setAnim = 0.0f;
        g_setDragging = false;
        return;
    }
    ImGuiIO& io = ImGui::GetIO();
    if (g_setAnim < 1.0f)
    {
        float dt = io.DeltaTime;
        if (dt > 0.05f) dt = 0.05f;
        g_setAnim += dt * 6.0f;
        if (g_setAnim > 1.0f) g_setAnim = 1.0f;
    }
    float t = g_setAnim;
    float ease = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
    float y_off = (1.0f - ease) * -14.0f;

    if (g_setPos.x < 0.0f)
    {
        ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        g_setPos = ImVec2(center.x - g_setSize.x * 0.5f + 300.0f, center.y - g_setSize.y * 0.5f - 250.0f);
    }
    ImGui::SetNextWindowSize(g_setSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(g_setPos.x, g_setPos.y + y_off), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(ease);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ease);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, g_colFrame);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("##uisettings", nullptr, flags))
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        pos.x = floorf(pos.x);
        pos.y = floorf(pos.y);
        ImVec2 max = ImVec2(floorf(pos.x + size.x), floorf(pos.y + size.y));

        const ImU32 col_outline = IM_COL32(0, 0, 0, 255);
        DrawPixelBox(dl, pos, max, FrameU32(), BorderU32(), col_outline);
        dl->AddRectFilled(ImVec2(pos.x + 2.0f, pos.y + 2.0f), ImVec2(max.x - 2.0f, pos.y + 3.0f), AccentU32());

        ImFont* f = g_fontSmall ? g_fontSmall : ImGui::GetFont();
        ImVec2 gmin = ImVec2(pos.x + 14.0f, pos.y + 24.0f);
        ImVec2 gmax = ImVec2(max.x - 14.0f, max.y - 10.0f);
        DrawGroupBox(dl, gmin, gmax, "settings", f, 13.0f, GroupU32(), BorderU32(), col_outline);

        float cw = (gmax.x - 14.0f) - (gmin.x + 14.0f);
        float r1_y = gmin.y + 16.0f;
        float r2_y = r1_y + kCheckRowPitch;
        float r3_y = r2_y + kCheckRowPitch;
        float r4_y = r3_y + kLabeledCtrlH + 6.0f;
        float r5_y = r4_y + kCheckRowPitch;
        float r6_y = r5_y + kCheckRowPitch;

        bool s1 = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r1_y), &g_vsync, "VSync", f, 13.0f);
        bool s2 = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r2_y), &g_streamProof, "Stream proof", f, 13.0f);
        bool s3 = DrawSlider("##sl_fpscap", dl, ImVec2(gmin.x + 14.0f, r3_y), cw, &g_fpsCap, 0, 240, "FPS Cap (0=off)", "%d", f, 13.0f);
        if (g_fpsCap > 0 && g_fpsCap < 30) g_fpsCap = 30;
        bool s4 = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r4_y), &g_watermark, "Watermark", f, 13.0f);
        QuestionOption wm_opts[] = {
            { &g_watermarkFps, "FPS" },
            { &g_watermarkPlayers, "PLAYERS" },
            { &g_watermarkDate, "DATE" }
        };
        bool s_wm_qb = DrawQuestionButton("##qb_wm", dl, ImVec2(gmax.x - 14.0f, r4_y), wm_opts, 3, f, 13.0f);
        bool s5 = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r5_y), &g_keybinds, "Keybinds", f, 13.0f);
        bool s6 = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r6_y), &g_movement, "Movement", f, 13.0f);
        float r6b_y = r6_y + kCheckRowPitch;
        bool s6b = DrawCheckbox(dl, ImVec2(gmin.x + 14.0f, r6b_y), &g_internalDebug, "Logs", f, 13.0f);
        QuestionOption mov_opts[] = {
            { &g_movementVelocityOpt, "VELOCITY" },
            { &g_movementGraphOpt, "GRAPH" },
            { &g_movementKeysOpt, "KEYS" }
        };
        bool s_mov_qb = DrawQuestionButton("##qb_mov", dl, ImVec2(gmax.x - 14.0f, r6_y), mov_opts, 3, f, 13.0f);

        float r7_y = r6b_y + kCheckRowPitch + 2.0f;
        static bool s_cfgInited = false;
        if (!s_cfgInited)
        {
            RefreshConfigList();
            s_cfgInited = true;
        }
        bool s_dd = DrawConfigDropdown("##dd_cfgsel", dl, ImVec2(gmin.x + 14.0f, r7_y), cw, &g_configSelectedIdx, g_configFiles, "Select config", f, 13.0f);

        float r8_y = r7_y + 33.0f;
        bool s7 = DrawTextInput("##ti_cfg", dl, ImVec2(gmin.x + 14.0f, r8_y), cw, g_configName, sizeof(g_configName), "Config name", "default", f, 13.0f);

        float r9_y = r8_y + 37.0f;
        float b3_w = floorf((cw - 8.0f) / 3.0f);
        static float s_saveTime = -10.0f;
        static float s_loadTime = -10.0f;
        static float s_delTime  = -10.0f;
        static float s_expTime  = -10.0f;
        static float s_impTime  = -10.0f;

        bool save_hov = false, load_hov = false, del_hov = false, exp_hov = false, imp_hov = false;

        const char* save_lbl = ((float)ImGui::GetTime() - s_saveTime < 1.0f) ? "Saved!" : "Save";
        const char* load_lbl = ((float)ImGui::GetTime() - s_loadTime < 1.0f) ? "Loaded!" : "Load";
        const char* del_lbl  = ((float)ImGui::GetTime() - s_delTime < 1.0f) ? "Deleted!" : "Delete";
        const char* exp_lbl  = ((float)ImGui::GetTime() - s_expTime < 1.0f) ? "Exported!" : "Export";
        const char* imp_lbl  = ((float)ImGui::GetTime() - s_impTime < 1.0f) ? "Imported!" : "Import";

        bool save_click = DrawButton(dl, ImVec2(gmin.x + 14.0f, r9_y), b3_w, 20.0f, save_lbl, f, 13.0f, &save_hov);
        bool load_click = DrawButton(dl, ImVec2(gmin.x + 14.0f + b3_w + 4.0f, r9_y), b3_w, 20.0f, load_lbl, f, 13.0f, &load_hov);
        bool del_click  = DrawButton(dl, ImVec2(gmin.x + 14.0f + (b3_w + 4.0f) * 2.0f, r9_y), cw - (b3_w + 4.0f) * 2.0f, 20.0f, del_lbl, f, 13.0f, &del_hov);

        float r10_y = r9_y + 24.0f;
        float b2_w = floorf((cw - 6.0f) * 0.5f);
        bool exp_click  = DrawButton(dl, ImVec2(gmin.x + 14.0f, r10_y), b2_w, 20.0f, exp_lbl, f, 13.0f, &exp_hov);
        bool imp_click  = DrawButton(dl, ImVec2(gmin.x + 14.0f + b2_w + 6.0f, r10_y), b2_w, 20.0f, imp_lbl, f, 13.0f, &imp_hov);

        if (save_click)
        {
            if (SaveConfig(g_configName))
            {
                s_saveTime = (float)ImGui::GetTime();
            }
        }
        if (load_click)
        {
            if (LoadConfig(g_configName))
            {
                s_loadTime = (float)ImGui::GetTime();
            }
        }
        if (del_click)
        {
            if (DeleteConfigFile(g_configName))
            {
                s_delTime = (float)ImGui::GetTime();
            }
        }
        if (exp_click)
        {
            if (ExportConfig(g_configName))
            {
                s_expTime = (float)ImGui::GetTime();
            }
        }
        if (imp_click)
        {
            if (ImportConfig())
            {
                s_impTime = (float)ImGui::GetTime();
            }
        }

        ImVec2 m = io.MousePos;
        bool pHover = (m.x >= pos.x && m.x <= max.x && m.y >= pos.y && m.y <= max.y);
        bool anyPop = ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
        bool overTheme = g_themeOpen && g_themePos.x >= 0.0f && (m.x >= g_themePos.x && m.x <= g_themePos.x + g_themeSize.x && m.y >= g_themePos.y && m.y <= g_themePos.y + g_themeSize.y);
        bool overBinds = g_keybinds && (m.x >= g_bindsPos.x && m.x <= g_bindsPos.x + g_bindsSize.x && m.y >= g_bindsPos.y && m.y <= g_bindsPos.y + g_bindsSize.y);
        bool overMove = g_movement && (m.x >= g_movementPos.x && m.x <= g_movementPos.x + g_movementSize.x && m.y >= g_movementPos.y && m.y <= g_movementPos.y + g_movementSize.y);
        bool overDebug = g_internalDebug && (m.x >= g_debugPos.x && m.x <= g_debugPos.x + g_debugSize.x && m.y >= g_debugPos.y && m.y <= g_debugPos.y + g_debugSize.y);
        bool pcb = s1 || s2 || s4 || s5 || s6 || s6b || s_wm_qb || s_mov_qb;
        bool psl = s3;
        bool pdd = s_dd;
        bool pti = s7;
        bool pbtn = save_hov || load_hov || del_hov || exp_hov || imp_hov;
        if (!g_setDragging && ImGui::IsMouseClicked(0) && pHover && !anyPop && !overTheme && !overBinds && !overMove && !overDebug && !g_navBarHov && g_activeBindId == nullptr && !pcb && !psl && !pdd && !pti && !pbtn)
        {
            g_setDragging = true;
            g_setAnim = 1.0f;
            g_setDragOff = ImVec2(m.x - g_setPos.x, m.y - g_setPos.y);
        }
        if (g_setDragging)
        {
            if (ImGui::IsMouseDown(0))
            {
                g_setPos.x = m.x - g_setDragOff.x;
                g_setPos.y = m.y - g_setDragOff.y;
            }
            else
            {
                g_setDragging = false;
            }
        }
        if (ease < 0.999f)
        {
            for (int i = 0; i < dl->VtxBuffer.Size; i++)
            {
                ImDrawVert& v = dl->VtxBuffer.Data[i];
                ImU32 a = (v.col >> 24) & 0xFF;
                a = (ImU32)((float)a * ease);
                v.col = (v.col & 0x00FFFFFF) | (a << 24);
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(6);
}

inline void RenderThemePanel()
{
    if (!g_themeOpen)
    {
        g_themeAnim = 0.0f;
        g_themeDragging = false;
        return;
    }
    ImGuiIO& io = ImGui::GetIO();
    if (g_themeAnim < 1.0f)
    {
        float dt = io.DeltaTime;
        if (dt > 0.05f) dt = 0.05f;
        g_themeAnim += dt * 6.0f;
        if (g_themeAnim > 1.0f) g_themeAnim = 1.0f;
    }
    float t = g_themeAnim;
    float ease = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
    float y_off = (1.0f - ease) * -14.0f;

    if (g_themePos.x < 0.0f)
    {
        ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        g_themePos = ImVec2(center.x - g_themeSize.x * 0.5f - 300.0f, center.y - g_themeSize.y * 0.5f - 250.0f);
    }
    ImGui::SetNextWindowSize(g_themeSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(g_themePos.x, g_themePos.y + y_off), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(ease);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ease);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, g_colFrame);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
    if (ImGui::Begin("##uitheme", nullptr, flags))
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();
        pos.x = floorf(pos.x);
        pos.y = floorf(pos.y);
        ImVec2 max = ImVec2(floorf(pos.x + size.x), floorf(pos.y + size.y));

        const ImU32 col_outline = IM_COL32(0, 0, 0, 255);
        DrawPixelBox(dl, pos, max, FrameU32(), BorderU32(), col_outline);
        dl->AddRectFilled(ImVec2(pos.x + 2.0f, pos.y + 2.0f), ImVec2(max.x - 2.0f, pos.y + 3.0f), AccentU32());

        ImFont* f = g_fontSmall ? g_fontSmall : ImGui::GetFont();
        ImVec2 gmin = ImVec2(pos.x + 14.0f, pos.y + 24.0f);
        ImVec2 gmax = ImVec2(max.x - 14.0f, max.y - 10.0f);
        DrawGroupBox(dl, gmin, gmax, "theme", f, 13.0f, GroupU32(), BorderU32(), col_outline);

        float cw = (gmax.x - 14.0f) - (gmin.x + 14.0f);
        float t1_y = gmin.y + 18.0f;
        float t2_y = t1_y + 37.0f;

        const char* theme_items[] = { "Basic", "Cyan", "Purple", "Mint", "Red", "Amber" };
        bool s4 = DrawDropdown("##dd_theme", dl, ImVec2(gmin.x + 14.0f, t1_y), cw, &g_themeMask, theme_items, 6, "Theme", f, 13.0f);

        ImVec2 pk_sz(14.0f, 14.0f);
        ImU32 lab_col = TextU32();
        ImU32 lab_out = IM_COL32(0, 0, 0, 255);
        struct ThemeRow { const char* id; ImVec4* col; const char* label; };
        ThemeRow theme_rows[] = {
            { "##c_th0", &g_accent1, "Accent" },
            { "##c_th1", &g_colFrame, "Window" },
            { "##c_th2", &g_colGroup, "Groups" },
            { "##c_th3", &g_colCtrl, "Controls" },
            { "##c_th4", &g_colCtrlHov, "Ctrl hover" },
            { "##c_th5", &g_colBorder, "Borders" },
            { "##c_th6", &g_colText, "Text" },
            { "##c_th7", &g_colTextDim, "Text dim" },
        };
        bool cp_any = false;
        float pr_y = t2_y;
        for (int i = 0; i < 8; i++)
        {
            bool ch = DrawColorPicker(theme_rows[i].id, dl, ImVec2(gmin.x + 14.0f, pr_y), pk_sz, theme_rows[i].col);
            ImVec2 lbsz = f->CalcTextSizeA(13.0f, FLT_MAX, 0.0f, theme_rows[i].label);
            float lby = floorf(pr_y + (14.0f - lbsz.y) * 0.5f);
            DrawOutlinedText(dl, f, 13.0f, ImVec2(gmin.x + 14.0f + 22.0f, lby), lab_col, lab_out, theme_rows[i].label);
            cp_any = cp_any || ch || ImGui::IsPopupOpen(theme_rows[i].id);
            pr_y += kCheckRowPitch;
        }

        bool rs_hov = false;
        bool rs_click = DrawButton(dl, ImVec2(gmin.x + 14.0f, pr_y + 4.0f), cw, 20.0f, "Reset", f, 13.0f, &rs_hov);
        if (rs_click)
        {
            ApplyThemePalette(0);
            g_themeMask = (1 << 0);
        }

        ImVec2 m = io.MousePos;
        bool pHover = (m.x >= pos.x && m.x <= max.x && m.y >= pos.y && m.y <= max.y);
        bool anyPop = ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopup);
        bool overSet = g_settingsOpen && g_setPos.x >= 0.0f && (m.x >= g_setPos.x && m.x <= g_setPos.x + g_setSize.x && m.y >= g_setPos.y && m.y <= g_setPos.y + g_setSize.y);
        bool overBinds = g_keybinds && (m.x >= g_bindsPos.x && m.x <= g_bindsPos.x + g_bindsSize.x && m.y >= g_bindsPos.y && m.y <= g_bindsPos.y + g_bindsSize.y);
        bool overMove = g_movement && (m.x >= g_movementPos.x && m.x <= g_movementPos.x + g_movementSize.x && m.y >= g_movementPos.y && m.y <= g_movementPos.y + g_movementSize.y);
        bool overDebug = g_internalDebug && (m.x >= g_debugPos.x && m.x <= g_debugPos.x + g_debugSize.x && m.y >= g_debugPos.y && m.y <= g_debugPos.y + g_debugSize.y);
        if (!g_themeDragging && ImGui::IsMouseClicked(0) && pHover && !anyPop && !overSet && !overBinds && !overMove && !overDebug && !g_navBarHov && g_activeBindId == nullptr && !s4 && !cp_any && !rs_hov)
        {
            g_themeDragging = true;
            g_themeAnim = 1.0f;
            g_themeDragOff = ImVec2(m.x - g_themePos.x, m.y - g_themePos.y);
        }
        if (g_themeDragging)
        {
            if (ImGui::IsMouseDown(0))
            {
                g_themePos.x = m.x - g_themeDragOff.x;
                g_themePos.y = m.y - g_themeDragOff.y;
            }
            else
            {
                g_themeDragging = false;
            }
        }
        if (ease < 0.999f)
        {
            for (int i = 0; i < dl->VtxBuffer.Size; i++)
            {
                ImDrawVert& v = dl->VtxBuffer.Data[i];
                ImU32 a = (v.col >> 24) & 0xFF;
                a = (ImU32)((float)a * ease);
                v.col = (v.col & 0x00FFFFFF) | (a << 24);
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleColor(1);
    ImGui::PopStyleVar(6);
}

inline void RenderExplorerPanel() {}
inline void RenderPlayerPreviewPanel() {}
}
