#include "speed.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/core/gui/menu.h"
#include "../../../utility/cache/cache.h"
#include "../../../utility/struct/offsets.h"

namespace rpbrx::misc::speed
{
    static float s_original = 16.0f;
    static bool  s_saved = false;

    void run()
    {
        if (!g_proc || !g_proc->connected() || !g_ctx.ready) return;

        if (!settings::speed_enabled) {
            if (s_saved) {
                node hum = g_ctx.local_humanoid;
                if (hum) {
                    g_proc->write<float>(hum.addr + Offsets::Humanoid::WalkSpeed, s_original);
                    s_saved = false;
                }
            }
            return;
        }

        if (settings::speed_key && !Menu::IsMenuKeyActive("##speed_key", settings::speed_key, settings::speed_key_mode))
        {
            if (s_saved) {
                node hum = g_ctx.local_humanoid;
                if (hum) {
                    g_proc->write<float>(hum.addr + Offsets::Humanoid::WalkSpeed, s_original);
                    s_saved = false;
                }
            }
            return;
        }

        node hum = g_ctx.local_humanoid;
        if (!hum) return;

        if (!s_saved) {
            s_original = g_proc->read<float>(hum.addr + Offsets::Humanoid::WalkSpeed);
            s_saved = true;
        }

        g_proc->write<float>(hum.addr + Offsets::Humanoid::WalkSpeed, settings::speed_value);
    }
}
