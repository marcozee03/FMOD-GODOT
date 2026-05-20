#pragma once
#include <godot_cpp/variant/string_name.hpp>
using namespace godot;
namespace FmodGodot
{
inline constexpr const char *LOGGING_LEVEL = "Fmod/Debug/logging_level";
inline constexpr const char *DEBUG_TYPE = "Fmod/Debug/type";
inline constexpr const char *DEBUG_DISPLAY = "Fmod/Debug/display";
inline constexpr const char *SOURCE_TYPE = "Fmod/Banks/Import/source_type";
inline constexpr const char *FMOD_STUDIO_PATH = "Fmod/Banks/Import/fmod_studio_path";
inline constexpr const char *FMOD_PROJECT_PATH = "Fmod/Banks/Import/fmod_project_path";
inline constexpr const char *BANK_DIRECTORY = "Fmod/Banks/Import/bank_directory";
inline constexpr const char *LOAD_BANKS = "Fmod/Banks/Initialization/load_banks";
inline constexpr const char *SPECIFIED_BANKS = "Fmod/Banks/Initialization/specified_banks";
inline constexpr const char *LOAD_SAMPLE_DATA = "Fmod/Banks/Initialization/load_sample_data";
inline constexpr const char *ENCRYPTION_KEY = "Fmod/Banks/Initialization/encryption_key";
inline constexpr const char *LIVE_UPDATE = "Fmod/General/live_update";
inline constexpr const char *LIVE_UPDATE_PORT = "Fmod/General/live_update_port";
inline constexpr const char *SAMPLE_RATE = "Fmod/General/sample_rate";
inline constexpr const char *REAL_COUNT = "Fmod/General/Channels/real_count";
inline constexpr const char *VIRTUAL_COUNT = "Fmod/General/Channels/virtual_count";
inline constexpr const char *BUFFER_LENGTH = "Fmod/General/Dsp/buffer_length";
inline constexpr const char *BUFFER_COUNT = "Fmod/General/Dsp/buffer_count";
inline constexpr const char *DOPPLER_SCALE = "Fmod/General/3D Settings/doppler_scale";
inline constexpr const char *DISTANCE_FACTOR = "Fmod/General/3D Settings/distance_factor";
inline constexpr const char *ROLLOFF_SCALE = "Fmod/General/3D Settings/rollof_scale";
} // namespace FmodGodot
