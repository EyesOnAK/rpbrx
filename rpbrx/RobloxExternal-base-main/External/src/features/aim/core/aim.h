#pragma once
#include "../../../utility/cache/math.h"
#include "../../../utility/cache/cache.h"
#include <cstdint>

namespace rpbrx::aim
{
    void run(int w, int h);

    node resolve_part(const node& character, int partIdx);

    void set_debug_target(const math::vec3& world, uintptr_t playerAddr);
    void clear_debug_target();
    bool debug_target(math::vec3& out);
    bool debug_target_player(uintptr_t& out);

    bool debug_recent_player(uintptr_t addr);

    bool debug_in_use();
}
