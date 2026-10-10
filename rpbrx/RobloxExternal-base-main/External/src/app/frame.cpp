#include "frame.h"
#include "app.h"
#include "../menu/core/overlay/overlay.h"
#include "../menu/core/gui/menu.h"
#include "../menu/overlay/settings.h"
#include "../utility/cache/cache.h"
#include "../utility/logger/logger.h"
#include "../utility/struct/offsets.h"
#include "../utility/global/globals.h"
#include "../features/visual/core/visual.h"
#include "../features/visual/world/ore.h"
#include "../features/visual/world/plants.h"
#include "../features/visual/world/bots.h"
#include "../features/visual/world/animals.h"
#include "../features/visual/world/tools.h"
#include "../features/visual/world/crates.h"
#include "../features/visual/world/keycards.h"
#include "../features/aim/core/aim.h"
#include "../features/misc/core/misc.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>

static void PrintDebugLines(float fps)
{
    using namespace rpbrx;
    bool conn = g_proc && g_proc->connected();
    uint32_t pid = conn ? g_proc->pid() : 0;
    uint64_t base = conn ? g_proc->base() : 0;
    uint64_t dm = g_ctx.datamodel ? g_ctx.datamodel.addr : 0;
    uint64_t ws = g_ctx.workspace ? g_ctx.workspace.addr : 0;
    uint64_t pl = g_ctx.players ? g_ctx.players.addr : 0;
    std::string plName = g_ctx.players ? g_ctx.players.name() : "none";
    std::string lpName = g_ctx.local_player ? g_ctx.local_player.name() : "none";
    size_t plCount = g_ctx.player_list().size();
    math::vec3 pos = g_ctx.local_pos;

    LOGF("debug", "FPS: %5.1f | PID: %u Base: 0x%llX | DM: 0x%llX WS: 0x%llX | Players(%s): %zu (0x%llX) | Local: %s | Pos: (%.1f, %.1f, %.1f) Speed: %.1f | Ready: %d",
        fps, pid, (unsigned long long)base, (unsigned long long)dm, (unsigned long long)ws,
        plName.c_str(), plCount, (unsigned long long)pl,
        lpName.c_str(), pos.x, pos.y, pos.z, g_localSpeed, g_ctx.ready ? 1 : 0);
}

static bool NeedsGameData()
{
    using namespace rpbrx;
    return settings::aim_enabled
        || settings::aim_autoshoot
        || settings::aim_show_fov
        || settings::box_enabled
        || settings::skeleton_enabled
        || settings::health_enabled
        || settings::name_enabled
        || settings::headdot_enabled
        || settings::distance_enabled
        || settings::tool_enabled
        || settings::flags_team
        || settings::flags_state
        || settings::flags_health
        || settings::tracer_enabled
        || settings::oof_enabled
        || settings::walkspeed_enabled
        || settings::jumppower_enabled
        || settings::fov_enabled
        || settings::noclip_enabled
         || settings::freecam_enabled
         || settings::hipheight_enabled
        || settings::infjump_enabled
        || settings::hitsound_enabled
        || settings::time_enabled
        || settings::world_fog_enabled
        || settings::world_exposure_enabled
        || settings::world_ore_enabled
        || settings::world_plant_enabled
        || settings::world_bots_enabled
        || settings::world_animals_enabled
        || settings::world_tools_enabled
        || settings::world_crates_enabled
        || settings::world_keycards_enabled
        || settings::hitbox_enabled
        || settings::chams_enabled
        || settings::whitehits_enabled
        || settings::footsteps_enabled
        || settings::targethud_enabled
        || settings::kitesp_enabled
        || settings::tickrate_enabled
        || settings::speed_enabled;
}

static bool AnyGameFeatureActive()
{
    using namespace rpbrx;
    if (NeedsGameData())
        return true;
    return Menu::g_movement
            || Menu::g_explorerOpen
            || Menu::g_playerPreviewOpen;
}

void rpbrx::frame::tick(ImDrawList* dl, int width, int height, float fps, bool menuOpen)
{
    (void)menuOpen;
    app::g_firstFrame.store(true);

    static double lastAttachTry = -10.0;
    static double lastRefresh = -10.0;
    static double lastPfCache = -10.0;
    double now = ImGui::GetTime();

    bool needGame = NeedsGameData();
    bool overlayActive = needGame || Menu::g_movement || Menu::g_explorerOpen || Menu::g_playerPreviewOpen;

    static LARGE_INTEGER qpf{};
    if (!qpf.QuadPart) QueryPerformanceFrequency(&qpf);
    auto qnow = []() { LARGE_INTEGER c; QueryPerformanceCounter(&c); return c; };
    auto qms = [&](LARGE_INTEGER s) { LARGE_INTEGER e; QueryPerformanceCounter(&e); return (e.QuadPart - s.QuadPart) * 1000.0 / (double)qpf.QuadPart; };
    auto qema = [](double prev, double v) { return prev * 0.92 + v * 0.08; };
    LARGE_INTEGER t0 = qnow();

    if ((!rpbrx::g_proc || !rpbrx::g_proc->connected()))
    {
        if (now - lastAttachTry > 2.0)
        {
            lastAttachTry = now;
            rpbrx::g_proc->attach("RobloxPlayerBeta.exe");
        }
    }
    else if (needGame)
    {

        if (now - lastRefresh >= 0.008)
        {
            lastRefresh = now;

            rpbrx::g_ctx.refresh();

            rpbrx::math::vec3 lv = rpbrx::g_ctx.local_velocity;
            bool ok = rpbrx::g_ctx.ready && rpbrx::g_ctx.local_root
                      && std::isfinite(lv.x) && std::isfinite(lv.y) && std::isfinite(lv.z);
            g_localSpeed = ok ? sqrtf(lv.x * lv.x + lv.z * lv.z) : -1.0f;
        }
    }
    else
    {
        g_localSpeed = -1.0f;
    }
    Menu::g_dbgMsRefresh = qema(Menu::g_dbgMsRefresh, qms(t0));

    t0 = qnow();
    if (rpbrx::g_proc && rpbrx::g_proc->connected() && now - lastPfCache >= 0.016)
    {
        lastPfCache = now;
        PfCache::update();
    }
    Menu::g_dbgMsPf = qema(Menu::g_dbgMsPf, qms(t0));

    {
        using namespace rpbrx;
        static double lastSessionCheck = -10.0;
        static uint64_t s_place = 0;
        static uint64_t s_dm = 0;
        static int s_zeroCount = 0;
        if (now - lastSessionCheck >= 0.5)
        {
            lastSessionCheck = now;
            bool conn = g_proc && g_proc->connected();
            uint64_t dmAddr = 0;
            uint64_t place = 0;
            if (conn)
            {
                dmAddr = g_ctx.datamodel.addr;
                if (!dmAddr)
                {
                    uint64_t fake = g_proc->read<uint64_t>(g_proc->base() + Offsets::FakeDataModel::Pointer);
                    if (fake) dmAddr = g_proc->read<uintptr_t>(fake + Offsets::FakeDataModel::RealDataModel);
                }
                if (dmAddr) place = g_proc->read<uint64_t>(dmAddr + Offsets::DataModel::PlaceId);
            }
            if (!conn)
            {
                if (s_place != 0)
                {
                    settings::push_state();
                    settings::disable_all();
                    app::badge("left", app::C_YELLOW, "disconnected - all features disabled");
                    LOG("game", "disconnected - all features disabled");
                    Logger::note_session("disconnected");
                }
                s_place = 0; s_dm = 0; s_zeroCount = 0;
            }
            else if (place != 0 && place != s_place)
            {
                if (s_place != 0)
                {
                    settings::push_state();
                    settings::disable_all();
                    char lb[96];
                    snprintf(lb, sizeof(lb), "left %llu - all features disabled", (unsigned long long)s_place);
                    app::badge("left", app::C_YELLOW, lb);
                    LOG("game", lb);
                }
                s_place = place; s_dm = dmAddr; s_zeroCount = 0;
                char jb[96];
                snprintf(jb, sizeof(jb), "joined place id %llu", (unsigned long long)place);
                app::badge("game", app::C_GREEN, jb);
                LOG("game", jb);
                Logger::note_session(jb);
                rpbrx::visual::on_game_joined();
                if (settings::restore_state())
                {
                    app::badge("game", app::C_CYAN, "features restored");
                    LOG("game", "features restored");
                }
            }
            else if (place != 0 && dmAddr != 0 && dmAddr != s_dm)
            {

                s_dm = dmAddr; s_zeroCount = 0;
                rpbrx::visual::on_game_joined();
                char jb[128];
                snprintf(jb, sizeof(jb), "rejoined place id %llu (new server)", (unsigned long long)place);
                app::badge("game", app::C_YELLOW, jb);
                LOG("game", jb);
                Logger::note_session(jb);
            }
            else if (place == 0 && s_place != 0)
            {
                if (++s_zeroCount >= 2)
                {
                    settings::push_state();
                    settings::disable_all();
                    char lb[96];
                    snprintf(lb, sizeof(lb), "left %llu - all features disabled", (unsigned long long)s_place);
                    app::badge("left", app::C_YELLOW, lb);
                    LOG("game", lb);
                    Logger::note_session("left game");
                    s_place = 0; s_dm = 0; s_zeroCount = 0;
                }
            }
            else s_zeroCount = 0;
        }
    }

    bool anythingToDraw = g_menuOpen || Menu::g_watermark || Menu::g_keybinds
                          || Menu::g_movement || Menu::g_internalDebug || overlayActive;
    Overlay::SetRenderEnabled(anythingToDraw);

    Overlay::SetFrameInterval(0.0f);

    static double lastDebugBuild = -10.0;
    if (Menu::g_internalDebug && now - lastDebugBuild >= 1.0)
    {
        lastDebugBuild = now;
        PrintDebugLines(fps);
    }

    Logger::quiet = true;

    t0 = qnow();
    Menu::RenderWatermark(dl, width, fps);
    Menu::RenderKeybinds(dl, width, height);
    Menu::RenderMovement(dl, width, height);
    Menu::RenderInternalDebug(dl, width, height, fps);
    Menu::RenderNavBar(dl, width, height);
    Menu::Render();
    Menu::RenderSettingsPanel();
    Menu::RenderThemePanel();
    Menu::RenderExplorerPanel();
    Menu::RenderPlayerPreviewPanel();
    Menu::g_dbgMsMenu = qema(Menu::g_dbgMsMenu, qms(t0));

    t0 = qnow();
    if (needGame)
    {
        rpbrx::aim::run(width, height);
        rpbrx::visual::render(dl, width, height);
        rpbrx::ore::tick(dl, width, height);
        rpbrx::plants::tick(dl, width, height);
        rpbrx::bots::tick(dl, width, height);
        rpbrx::animals::tick(dl, width, height);
        rpbrx::tools::tick(dl, width, height);
        rpbrx::crates::tick(dl, width, height);
        rpbrx::keycards::tick(dl, width, height);
    }
    Menu::g_dbgMsAim = qema(Menu::g_dbgMsAim, qms(t0));

    t0 = qnow();
    if (needGame)
    {
        rpbrx::visual::render(dl, width, height);
    }
    Menu::g_dbgMsVisual = qema(Menu::g_dbgMsVisual, qms(t0));

    t0 = qnow();
    rpbrx::misc::run();
    Menu::g_dbgMsMisc = qema(Menu::g_dbgMsMisc, qms(t0));
}
