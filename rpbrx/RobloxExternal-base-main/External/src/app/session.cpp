#include "session.h"
#include "app.h"
#include "../utility/logger/logger.h"
#include "../utility/cache/cache.h"
#include "../utility/struct/offsets.h"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>

namespace rpbrx::session
{
    void stage_watcher(std::atomic<bool>& stop) {
        int shown = 0;
        while (!stop.load()) {
            if (shown < 1 && rpbrx::g_proc && rpbrx::g_proc->connected()) {
                shown = 1;
                char buf[192];
                snprintf(buf, sizeof(buf), "pid=%u base=0x%llX client=%s",
                    (unsigned)rpbrx::g_proc->pid(),
                    (unsigned long long)rpbrx::g_proc->base(),
                    Offsets::ClientVersion.c_str());
                app::badge("attached", app::C_GREEN, buf);
                LOG("attach", buf);
            }
            if (shown < 2 && app::g_overlayReady.load()) {
                shown = 2;
                app::badge("overlay", app::C_GREEN, "DirectX ready");
                LOG("overlay", "DirectX ready");
            }
            if (shown < 3 && app::g_firstFrame.load()) {
                shown = 3;
                app::badge("ready", app::C_CYAN, "press INSERT for menu - have fun");
                LOG("boot", "ready");
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    void target_watcher(std::atomic<bool>& stop) {
        int ticks = 0;
        while (!stop.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (stop.load())
                return;
            if (++ticks < 5)
                continue;
            ticks = 0;
            if (rpbrx::g_proc && rpbrx::g_proc->connected() && !rpbrx::g_proc->alive()) {
                unsigned pid = (unsigned)rpbrx::g_proc->pid();
                rpbrx::g_proc->detach();
                char buf[160];
                snprintf(buf, sizeof(buf), "game closed (pid=%u) - handle parked, waiting for restart", pid);
                app::badge("left", app::C_YELLOW, buf);
                LOG("game", buf);
                Logger::note_session("no game (closed)");
            }
        }
    }
}
