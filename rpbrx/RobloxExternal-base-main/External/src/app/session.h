#pragma once
#include <atomic>

namespace rpbrx::session
{
    void stage_watcher(std::atomic<bool>& stop);
    void target_watcher(std::atomic<bool>& stop);
}
