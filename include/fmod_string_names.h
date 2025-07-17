#pragma once
#include <godot_cpp/variant/string_name.hpp>
using namespace godot;
namespace FmodGodot
{
    static const char *LOGGING_LEVEL = "Fmod/Debug/logging_level";
    static const char *DEBUG_TYPE = "Fmod/Debug/type";
    static const char *DEBUG_DISPLAY = "Fmod/Debug/display";
    static const char *SOURCE_TYPE = "Fmod/Banks/Import/source_type";
    static const char *FMOD_STUDIO_PATH = "Fmod/Banks/Import/fmod_studio_path";
    static const char *FMOD_PROJECT_PATH = "Fmod/Banks/Import/fmod_project_path";
    static const char *BANK_DIRECTORY = "Fmod/Banks/Import/bank_directory";
    static const char *LOAD_BANKS = "Fmod/Banks/Initialization/load_banks";
    static const char *SPECIFIED_BANKS = "Fmod/Banks/Initialization/specified_banks";
    static const char *LOAD_SAMPLE_DATA = "Fmod/Banks/Initialization/load_sample_data";
    static const char *ENCRYPTION_KEY = "Fmod/Banks/Initialization/encryption_key";
    static const char *LIVE_UPDATE = "Fmod/General/live_update";
    static const char *LIVE_UPDATE_PORT = "Fmod/General/live_update_port";
    static const char *SAMPLE_RATE = "Fmod/General/sample_rate";
    static const char *REAL_COUNT = "Fmod/General/Channels/real_count";
    static const char *VIRTUAL_COUNT = "Fmod/General/Channels/virtual_count";
    static const char *BUFFER_LENGTH = "Fmod/General/Dsp/buffer_length";
    static const char *BUFFER_COUNT = "Fmod/General/Dsp/buffer_count";
    static const char *DOPPLER_SCALE = "Fmod/General/3D Settings/doppler_scale";
    static const char *DISTANCE_FACTOR = "Fmod/General/3D Settings/distance_factor";
    static const char *ROLLOFF_SCALE = "Fmod/General/3D Settings/rollof_scale";
}
