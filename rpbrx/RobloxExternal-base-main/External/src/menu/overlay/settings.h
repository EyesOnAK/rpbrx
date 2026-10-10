#pragma once

namespace rpbrx::settings
{
    // -- Aim Assist (from catsix) --
    inline bool  aim_enabled = false;
    inline int   aim_key = 0;
    inline int   aim_key_mode = 2;
    inline int   aim_part = 0;            // 0=RootPart, 1=Head
    inline float aim_fov = 100.0f;        // 0-1000 screen-space px radius
    inline float aim_speed = 15.0f;       // 0-30
    inline bool  aim_range_circle = false;
    inline float aim_circle_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline float aim_circle_transparency = 0.5f;
    inline float aim_circle_thickness = 1.0f;
    inline bool  aim_circle_filled = false;
    inline bool  aim_require_rclick = false;
    inline bool  aim_show_target = false;
    inline bool  aim_team_check = false;
    inline bool& team_check = aim_team_check;

    // -- ESP --
    inline bool  esp_enabled = false;
    inline int   esp_mode = 0;            // 0=2D, 1=3D, 2=Skeleton
    inline float esp_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline bool  esp_bounding_box = true;
    inline bool  esp_filled = false;
    inline float esp_fill_color[4] = { 0.0f, 0.0f, 0.0f, 0.35f };
    inline int   esp_box_style = 0;       // 0=Full, 1=Corner
    inline bool  esp_team_check = true;
    inline bool  esp_distance_check = false;
    inline float esp_distance_min = 0.0f;
    inline float esp_distance_max = 64.0f;

    // -- Health Bar --
    inline bool  health_enabled = false;

    // -- Skeleton (uses esp_color when esp_mode==2, standalone toggle for 2D mode overlay) --
    inline bool  skeleton_enabled = false;
    inline float skeleton_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // -- Chams --
    inline bool  chams_enabled = false;
    inline float chams_color[4] = { 1.0f, 0.2f, 0.2f, 0.5f };
    inline float chams_outline_color[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
    inline float chams_transparency = 0.5f;
    inline float chams_outline_transparency = 0.5f;
    inline bool  chams_render_walls = true;
    inline bool  chams_team_check = true;

    // -- Nametags --
    inline bool  name_enabled = false;
    inline float name_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline float name_scale = 1.0f;       // 0.1-1.5
    inline float name_bg_transparency = 0.5f;
    inline bool  name_show_health = true;
    inline bool  name_show_distance = true;
    inline bool  name_use_displayname = true;
    inline bool  name_show_background = true;
    inline bool  name_show_tool = true;   // equipped tool above nametag
    inline bool  name_team_check = true;
    inline bool  name_distance_check = false;
    inline float name_distance_min = 0.0f;
    inline float name_distance_max = 64.0f;

    // -- Kit ESP --
    inline bool  kitesp_enabled = false;
    inline float kitesp_color[4] = { 1.0f, 0.8f, 0.2f, 1.0f };
    inline float kitesp_scale = 1.0f;
    inline bool  kitesp_background = true;
    inline float kitesp_bg_color[4] = { 0.0f, 0.0f, 0.0f, 0.5f };
    inline bool  kitesp_alchemist = true;
    inline bool  kitesp_bee = true;
    inline bool  kitesp_bigman = true;
    inline bool  kitesp_ghost = true;
    inline bool  kitesp_metal = true;
    inline bool  kitesp_sheep = true;
    inline bool  kitesp_sorcerer = true;
    inline bool  kitesp_star = true;

    // -- WhiteHits --
    inline bool  whitehits_enabled = false;
    inline float whitehits_color[4] = { 1.0f, 1.0f, 1.0f, 0.6f };
    inline float whitehits_duration = 0.15f;

    // -- FOV (Camera) --
    inline bool  fov_enabled = false;
    inline float fov_value = 90.0f;       // 30-120
    inline int   fov_key = 0;
    inline int   fov_key_mode = 2;

    // -- Tracers --
    inline bool  tracer_enabled = false;
    inline float tracer_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline int   tracer_start = 1;        // 0=Middle, 1=Bottom, 2=Mouse
    inline int   tracer_end = 0;          // 0=Head, 1=Torso
    inline float tracer_transparency = 0.0f;
    inline bool  tracer_color_by_distance = false;
    inline bool  tracer_behind = true;
    inline bool  tracer_team_check = true;
    inline bool  tracer_distance_check = false;
    inline float tracer_distance_min = 0.0f;
    inline float tracer_distance_max = 64.0f;

    // -- Equipped Tool --
    inline bool  tool_enabled = false;
    inline float tool_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // -- Head Dot --
    inline bool  headdot_enabled = false;
    inline float headdot_color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline bool  headdot_fill = true;
    inline bool  headdot_outline = true;
    inline float headdot_outline_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    inline float headdot_thickness = 3.0f;

    // -- Footstep Circles --
    inline bool  footsteps_enabled = false;
    inline float footsteps_color[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
    inline float footsteps_radius = 0.5f;
    inline float footsteps_lifetime = 2.0f;
    inline float footsteps_max = 64.0f;

    // -- Global visual team check --
    inline bool  vis_team_check = false;

    inline void disable_all()
    {
        aim_enabled = false;
        esp_enabled = false;
        health_enabled = false;
        skeleton_enabled = false;
        chams_enabled = false;
        name_enabled = false;
        kitesp_enabled = false;
        whitehits_enabled = false;
        fov_enabled = false;
        tracer_enabled = false;
        tool_enabled = false;
        headdot_enabled = false;
        footsteps_enabled = false;
    }

    inline struct SavedFeatures
    {
        bool valid = false;
        bool aim = false;
        bool esp = false;
        bool health = false;
        bool skeleton = false;
        bool chams = false;
        bool name = false;
        bool kitesp = false;
        bool whitehits = false;
        bool fov = false;
        bool tracer = false;
        bool tool = false;
        bool headdot = false;
        bool footsteps = false;
    } saved;

    inline void push_state()
    {
        if (saved.valid) return;
        saved.valid = true;
        saved.aim = aim_enabled;
        saved.esp = esp_enabled;
        saved.health = health_enabled;
        saved.skeleton = skeleton_enabled;
        saved.chams = chams_enabled;
        saved.name = name_enabled;
        saved.kitesp = kitesp_enabled;
        saved.whitehits = whitehits_enabled;
        saved.fov = fov_enabled;
        saved.tracer = tracer_enabled;
        saved.tool = tool_enabled;
        saved.headdot = headdot_enabled;
        saved.footsteps = footsteps_enabled;
    }

    inline bool restore_state()
    {
        if (!saved.valid) return false;
        bool any = saved.aim || saved.esp || saved.health || saved.skeleton
            || saved.chams || saved.name || saved.kitesp || saved.whitehits
            || saved.fov || saved.tracer || saved.tool || saved.headdot
            || saved.footsteps;
        aim_enabled = aim_enabled || saved.aim;
        esp_enabled = esp_enabled || saved.esp;
        health_enabled = health_enabled || saved.health;
        skeleton_enabled = skeleton_enabled || saved.skeleton;
        chams_enabled = chams_enabled || saved.chams;
        name_enabled = name_enabled || saved.name;
        kitesp_enabled = kitesp_enabled || saved.kitesp;
        whitehits_enabled = whitehits_enabled || saved.whitehits;
        fov_enabled = fov_enabled || saved.fov;
        tracer_enabled = tracer_enabled || saved.tracer;
        tool_enabled = tool_enabled || saved.tool;
        headdot_enabled = headdot_enabled || saved.headdot;
        footsteps_enabled = footsteps_enabled || saved.footsteps;
        saved.valid = false;
        return any;
    }
}
