#pragma once

#include "fmod.h"
#include "fmod_common.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include <classes/project_settings.hpp>
namespace FmodGodot
{
struct InitSettings
{
    enum LiveUpdate : unsigned int
    {
        LIVE_UPDATE_DISABLED = 0,
        LIVE_UPDATE_ENABLED = 1,
        LIVE_UPDATE_DEV_ONLY = 2
    };
    InitSettings() = default;
    static InitSettings create_from_project_settings();
    void fill_from_project_settings();
    FMOD_DEBUG_FLAGS logging_level = FMOD_DEBUG_LEVEL_WARNING;
    FMOD_DEBUG_FLAGS debug_type = 0;
    FMOD_DEBUG_FLAGS debug_display = 0;
    int sample_rate = 48000;
    int dspbuffer_length = 1024;
    int dspbuffer_count = 5;
    int software_channels = 64;
    int virtual_channels = 1024;
    LiveUpdate live_update = LIVE_UPDATE_DISABLED;
    int live_update_port = 9264;
    godot::String encryption_key = "";
    godot::String bank_directory = "res://";
    float doppler_scale = 1;
    float distance_factor = 1;
    float rolloff_scale = 1;

  public:
    FMOD_ADVANCEDSETTINGS get_fmod_advanced_settings() const;
    FMOD_ADVANCEDSETTINGS get_fmod_advanced_settings(FMOD_SYSTEM *p_system) const;
    FMOD_STUDIO_ADVANCEDSETTINGS get_fmod_studio_advanced_settings() const;
    FMOD_STUDIO_ADVANCEDSETTINGS get_fmod_studio_advanced_settings(FMOD_STUDIO_SYSTEM *p_system) const;
};
} // namespace FmodGodot
