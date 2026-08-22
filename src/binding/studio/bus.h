#pragma once
#include "classes/object.hpp"
#include "classes/wrapped.hpp"
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/vector4i.hpp"
#include <cstdint>
using namespace godot;
namespace FmodGodot
{
namespace Studio
{

class StudioBus : public Object
{
    GDCLASS(StudioBus, Object);

  protected:
    static void _bind_methods();

  public:
    StudioBus() = default;
    ~StudioBus() = default;
    // Handle validity
    static bool is_valid(Handle p_handle);

    // Property access
    static Vector4i get_id(Handle p_handle);
    static String get_path(Handle p_handle);

    // Playback control
    static float get_volume(Handle p_handle);
    static float get_final_volume(Handle p_handle);
    static FMOD_RESULT set_volume(Handle p_handle, float p_volume);

    static bool get_paused(Handle p_handle);
    static FMOD_RESULT set_paused(Handle p_handle, bool p_paused);

    static bool get_mute(Handle p_handle);
    static FMOD_RESULT set_mute(Handle p_handle, bool p_mute);

    static FMOD_RESULT stop_all_events(Handle p_handle, FMOD_STUDIO_STOP_MODE p_mode);

    // Output port
    static uint64_t get_port_index(Handle p_handle);
    static FMOD_RESULT set_port_index(Handle p_handle, uint64_t p_index);

    // Low-level API access
    static FMOD_RESULT lock_channel_group(Handle p_handle);
    static FMOD_RESULT unlock_channel_group(Handle p_handle);
    static Handle get_channel_group(Handle p_handle);

    // Monitoring
    static unsigned int get_inclusive_c_p_u_usage(Handle p_handle);
    static unsigned int get_exclusive_c_p_u_usage(Handle p_handle);
    // TODO:
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *memoryusage) const;
};
} // namespace Studio
} // namespace FmodGodot
