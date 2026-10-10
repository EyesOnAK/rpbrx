#include "tickrate.h"
#include "../../../menu/overlay/settings.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/offsets.h"

namespace rpbrx::misc::tickrate
{
    static float s_original = 60.0f;
    static bool  s_saved = false;

    void run()
    {
        if (!settings::tickrate_enabled) {
            if (s_saved && g_proc && g_proc->connected() && g_ctx.ready && g_ctx.workspace) {
                // Offsets::TaskScheduler would go here when known
                s_saved = false;
            }
            return;
        }

        if (!g_proc || !g_proc->connected() || !g_ctx.ready) return;

        // TaskScheduler-based tickrate modification requires the TaskScheduler pointer offset
        // which is version-specific. The framework is here - add the offset when available.
        // Offsets::TaskScheduler::Pointer, Offsets::TaskScheduler::FPS
    }
}
