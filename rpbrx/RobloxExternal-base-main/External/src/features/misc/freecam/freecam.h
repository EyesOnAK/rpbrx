#pragma once
#include <atomic>

namespace rpbrx::misc::freecam
{
    void run();
    void thread_main(std::atomic<bool>& stop);
}
