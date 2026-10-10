#pragma once
#include "../../../menu/imgui/imgui.h"
#include <string>

namespace rpbrx::visual::targethud
{
    struct TargetInfo {
        std::string name;
        float hp = 0;
        float maxHp = 100;
        float distance = 0;
        std::string tool;
        bool valid = false;
    };

    void set_target(const TargetInfo& info);
    void clear_target();
    void render(ImDrawList* dl, int screenW, int screenH);
}
