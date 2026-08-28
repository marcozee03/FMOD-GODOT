#include "fmod_init_settings.h"
#include "fmod_defs.h"
#include "fmod_string_names.h"
#include <bit>
FmodGodot::InitSettings FmodGodot::InitSettings::create_from_project_settings()
{
    InitSettings settings;
    settings.fill_from_project_settings();
    return settings;
}
void FmodGodot::InitSettings::fill_from_project_settings()
{
    sample_rate = GLOBAL_GET(SAMPLE_RATE);
    dspbuffer_length = GLOBAL_GET(BUFFER_LENGTH);
    dspbuffer_count = GLOBAL_GET(BUFFER_COUNT);
    software_channels = GLOBAL_GET(REAL_COUNT);
    virtual_channels = GLOBAL_GET(VIRTUAL_COUNT);
    encryption_key = GLOBAL_GET(ENCRYPTION_KEY);
    rolloff_scale = GLOBAL_GET(ROLLOFF_SCALE);
    doppler_scale = GLOBAL_GET(DOPPLER_SCALE);
    distance_factor = GLOBAL_GET(DISTANCE_FACTOR);

    live_update = std::bit_cast<LiveUpdate>(static_cast<int>(GLOBAL_GET(LIVE_UPDATE)));
    live_update_port = GLOBAL_GET(LIVE_UPDATE_PORT);

    logging_level = GLOBAL_GET(LOGGING_LEVEL);
    debug_type = GLOBAL_GET(DEBUG_TYPE);
    debug_display = GLOBAL_GET(DEBUG_DISPLAY);
}
FMOD_ADVANCEDSETTINGS FmodGodot::InitSettings::get_fmod_advanced_settings() const
{
    FMOD_ADVANCEDSETTINGS fmod_settings = {0};
    fmod_settings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
    fmod_settings.profilePort = live_update_port;
    return fmod_settings;
}
FMOD_ADVANCEDSETTINGS FmodGodot::InitSettings::get_fmod_advanced_settings(FMOD_SYSTEM *p_system) const
{
    FMOD_ADVANCEDSETTINGS fmod_settings = {0};
    fmod_settings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
    FMOD_System_GetAdvancedSettings(p_system, &fmod_settings);
    fmod_settings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
    fmod_settings.profilePort = live_update_port;
    return fmod_settings;
}
FMOD_STUDIO_ADVANCEDSETTINGS FmodGodot::InitSettings::get_fmod_studio_advanced_settings() const
{
    FMOD_STUDIO_ADVANCEDSETTINGS studio_settings = {0};
    studio_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);
    // WARN: causes dangling reference
    // studio_settings.encryptionkey = encryption_key.utf8().ptr();
    return studio_settings;
}
FMOD_STUDIO_ADVANCEDSETTINGS FmodGodot::InitSettings::get_fmod_studio_advanced_settings(
    FMOD_STUDIO_SYSTEM *p_system) const
{
    FMOD_STUDIO_ADVANCEDSETTINGS studio_settings = {0};
    studio_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);
    FMOD_Studio_System_GetAdvancedSettings(p_system, &studio_settings);
    studio_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);
    // WARN: causes dangling reference
    // studio_settings.encryptionkey = encryption_key.utf8().ptr();
    return studio_settings;
}
