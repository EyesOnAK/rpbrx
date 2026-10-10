#include "misc.h"
#include "../walkspeed/walkspeed.h"
#include "../jumppower/jumppower.h"
#include "../hitsound/hitsound.h"
#include "../noclip/noclip.h"
#include "../hipheight/hipheight.h"
#include "../infjump/infjump.h"

#include "../time/time.h"
#include "../tickrate/tickrate.h"
#include "../speed/speed.h"

namespace rpbrx::misc
{
    void run()
    {
        walkspeed::run();
        jumppower::run();

        hitsound::run();
        noclip::run();
        hipheight::run();
        infjump::run();
        time::run();
        tickrate::run();
        speed::run();
    }
}
