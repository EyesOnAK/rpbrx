#pragma once
#include "../../../menu/imgui/imgui.h"
#include "../../../utility/cache/math.h"
#include "../../../utility/struct/world2screen.h"

namespace rpbrx::visual::chams
{
    void draw(ImDrawList* dl, float x0, float y0, float x1, float y1);
}
