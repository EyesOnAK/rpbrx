#pragma once
#include "../../../menu/imgui/imgui.h"
#include "../../../utility/cache/math.h"
#include "../../../utility/struct/world2screen.h"

namespace rpbrx::visual::footsteps
{
    void update(uintptr_t playerAddr, const math::vec3& footPos);
    void render(ImDrawList* dl, const math::view_matrix& view, float screenW, float screenH);
    void clear();
}
