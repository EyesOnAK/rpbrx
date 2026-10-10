#pragma once
#include "../../../menu/imgui/imgui.h"

namespace rpbrx::visual
{
    void render(ImDrawList* dl, int w, int h);
    void on_game_joined();
}
