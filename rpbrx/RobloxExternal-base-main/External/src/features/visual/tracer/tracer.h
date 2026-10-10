#pragma once
#include "../../../menu/imgui/imgui.h"

namespace rpbrx::visual::tracer
{
    // Draw a line from (startX,startY) to (endX,endY)
    void draw(ImDrawList* dl, float startX, float startY, float endX, float endY, ImU32 col, float thickness = 1.0f);
}
