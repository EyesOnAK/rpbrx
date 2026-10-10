#pragma once
#include "../../../utility/cache/math.h"

namespace rpbrx::aim::distance
{
    bool check(float dist);
    bool check(const math::vec3& targetPos, const math::vec3& camPos);
}
