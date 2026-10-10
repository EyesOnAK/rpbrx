#pragma once
#include "../../../menu/imgui/imgui.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/cache/math.h"
#include "../../../utility/struct/offsets.h"
#include "../../../utility/struct/world2screen.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/resources/font.h"
#include <string>
#include <vector>
#include <chrono>
#include <unordered_set>

namespace rpbrx::visual::kitesp
{
    struct KitEntity {
        uintptr_t addr = 0;
        std::string label;
        math::vec3 pos{};
        int kitIdx = -1;
    };

    void tick(ImDrawList* dl, int w, int h);
    void clear();
}
