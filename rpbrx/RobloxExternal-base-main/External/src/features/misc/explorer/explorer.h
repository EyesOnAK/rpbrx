#pragma once
#include "../../../menu/imgui/imgui.h"

namespace rpbrx::explorer
{

void render(ImDrawList* dl, ImVec2 pos, ImVec2 mx);

void on_close();
}
