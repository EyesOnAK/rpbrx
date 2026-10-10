#include "app.h"
#include "session.h"
#include "frame.h"
#include "../menu/core/overlay/overlay.h"
#include "../utility/global/globals.h"
#include "../utility/cache/cache.h"
#include "../utility/logger/logger.h"
#include <Windows.h>
#include <atomic>
#include <iostream>
#include <thread>

namespace rpbrx::app
{
    std::atomic<bool> g_overlayReady{ false };
    std::atomic<bool> g_firstFrame{ false };

    void badge(const char* label, WORD labelColor, const char* msg) {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO old{};
        GetConsoleScreenBufferInfo(h, &old);
        std::cout << "       ";
        SetConsoleTextAttribute(h, C_WHITE);
        std::cout << "[";
        SetConsoleTextAttribute(h, labelColor);
        std::cout << label;
        SetConsoleTextAttribute(h, C_WHITE);
        std::cout << "] ";
        SetConsoleTextAttribute(h, C_DIM);
        std::cout << msg << "\n";
        SetConsoleTextAttribute(h, old.wAttributes);
    }

    int run()
    {
        std::atomic<bool> stopWatcher{ false };
        std::thread watcher(session::stage_watcher, std::ref(stopWatcher));
        std::atomic<bool> stopTargetWatch{ false };
        std::thread targetWatch(session::target_watcher, std::ref(stopTargetWatch));

        OverlayConfig cfg;
        cfg.windowTitle = L" ";
        cfg.targetWindow = L"";

        Overlay overlay(cfg);
        if (!overlay.Initialize())
        {
            badge("fail", C_RED, "failed to initialize overlay - see rpbrx_log.txt");
            LOG("overlay", "Initialize failed");
            MessageBoxW(nullptr, L"Failed to initialize overlay", L"Error", MB_OK);
            stopWatcher.store(true);
            if (watcher.joinable()) watcher.join();
            stopTargetWatch.store(true);
            if (targetWatch.joinable()) targetWatch.join();
            return 1;
        }
        g_overlayReady.store(true);

        overlay.SetRenderCallback([](ImDrawList* dl, int width, int height, float fps, bool menuOpen) {
            frame::tick(dl, width, height, fps, menuOpen);
        });

        overlay.SetVSync(g_vsync);
        overlay.SetStreamProof(g_streamProof);
        Overlay::SetMenuOpen(true);

        overlay.Run();
        stopWatcher.store(true);
        if (watcher.joinable()) watcher.join();
        stopTargetWatch.store(true);
        if (targetWatch.joinable()) targetWatch.join();
        badge("done", C_GREEN, "session ended cleanly");
        LOG("boot", "session end");
        return 0;
    }
}
