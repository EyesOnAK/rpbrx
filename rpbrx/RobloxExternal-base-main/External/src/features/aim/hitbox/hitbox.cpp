#include "hitbox.h"
#include "../../../menu/overlay/settings.h"
#include "../../../menu/core/gui/menu.h"

namespace rpbrx::aim::hitbox
{
    float get_scale()
    {
        if (!settings::hitbox_enabled) return 1.0f;
        if (settings::hitbox_key && !Menu::IsMenuKeyActive("##hitbox_key", settings::hitbox_key, settings::hitbox_key_mode))
            return 1.0f;
        return settings::hitbox_scale;
    }

    int get_target_part()
    {
        return settings::hitbox_part;
    }
}
