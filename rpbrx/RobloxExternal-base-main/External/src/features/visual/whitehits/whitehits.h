#pragma once
#include "../../../menu/imgui/imgui.h"

namespace rpbrx::visual::whitehits
{
    void on_health_change(uintptr_t playerAddr, float oldHp, float newHp);
    void draw(ImDrawList* dl, uintptr_t playerAddr, float x0, float y0, float x1, float y1);
    void clear();
}
