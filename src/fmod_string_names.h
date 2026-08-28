#pragma once
#include <godot_cpp/variant/string_name.hpp>
using namespace godot;
namespace FmodGodot
{
inline constexpr const char *LOGGING_LEVEL = "fmod/debug/logging_level";
inline constexpr const char *DEBUG_TYPE = "fmod/debug/type";
inline constexpr const char *DEBUG_DISPLAY = "fmod/debug/display";
inline constexpr const char *SOURCE_TYPE = "fmod/banks/import/source_type";
inline constexpr const char *FMOD_STUDIO_PATH = "fmod/banks/import/fmod_studio_path";
inline constexpr const char *FMOD_PROJECT_PATH = "fmod/banks/import/fmod_project_path";
inline constexpr const char *BANK_DIRECTORY = "fmod/banks/import/bank_directory";
inline constexpr const char *LOAD_BANKS = "fmod/banks/initialization/load_banks";
inline constexpr const char *SPECIFIED_BANKS = "fmod/banks/initialization/specified_banks";
inline constexpr const char *LOAD_SAMPLE_DATA = "fmod/banks/initialization/load_sample_data";
inline constexpr const char *ENCRYPTION_KEY = "fmod/banks/initialization/encryption_key";
inline constexpr const char *LIVE_UPDATE = "fmod/general/live_update";
inline constexpr const char *LIVE_UPDATE_PORT = "fmod/general/live_update_port";
inline constexpr const char *SAMPLE_RATE = "fmod/general/sample_rate";
inline constexpr const char *REAL_COUNT = "fmod/general/channels/real_count";
inline constexpr const char *VIRTUAL_COUNT = "fmod/general/channels/virtual_count";
inline constexpr const char *BUFFER_LENGTH = "fmod/general/dsp/buffer_length";
inline constexpr const char *BUFFER_COUNT = "fmod/general/dsp/buffer_count";
inline constexpr const char *DOPPLER_SCALE = "fmod/general/3d_settings/doppler_scale";
inline constexpr const char *DISTANCE_FACTOR = "fmod/general/3d_settings/distance_factor";
inline constexpr const char *ROLLOFF_SCALE = "fmod/general/3d_settings/rolloff_scale";
inline constexpr const char *PLUGIN_PATH = "fmod/plugins/path";
inline constexpr const char *PLUGINS = "fmod/plugins/plugins";

inline constexpr const char *LIVE_UPDATE_GROUP = "fmod_gd_live_update_indicator";
} // namespace FmodGodot
