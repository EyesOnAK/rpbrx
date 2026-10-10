#include "tracer.h"

namespace rpbrx::visual::tracer
{
    void draw(ImDrawList* dl, float startX, float startY, float endX, float endY, ImU32 col, float thickness)
    {
        if (!dl) return;
        dl->AddLine(ImVec2(startX, startY), ImVec2(endX, endY), col, thickness);
    }
}
