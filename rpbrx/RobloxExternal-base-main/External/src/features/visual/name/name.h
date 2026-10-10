#pragma once
#include "../../../menu/imgui/imgui.h"
#include "../../../utility/struct/structs.h"
#include <string>

namespace rpbrx::visual::name
{
    // Simple name draw (used by subsystems that need basic nametag rendering)
    void draw(ImDrawList* dl, const data::rect& rc, const std::string& str);
}
