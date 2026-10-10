#include "health.h"
#include "../../../menu/overlay/settings.h"

namespace rpbrx::aim::health
{
    bool check(float hp)
    {
        if (hp <= 0.0f)
            return false;
        return true;
    }
}
