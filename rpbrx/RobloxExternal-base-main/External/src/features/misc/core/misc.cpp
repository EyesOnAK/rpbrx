#include "misc.h"
#include "../fov/fov.h"
#include "../../../menu/overlay/settings.h"

namespace rpbrx::misc
{
    void run()
    {
        fov::tick_fast(fov::worker_want_active());
    }
}
