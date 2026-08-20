#pragma once
#include "classes/object.hpp"
#include "classes/ref_counted.hpp"
#include "classes/wrapped.hpp"
#include "fmod_common.h"
#include "fmod_studio_common.h"
#include "variant/vector4i.hpp"
#include <cstdint>
using namespace godot;
namespace FmodGodot
{
namespace Studio
{

class Bus : public Object
{
    GDCLASS(Bus, Object);

  protected:
    static void _bind_methods();

  public:
    Bus() = default;
    ~Bus() = default;
    // Handle validity
    static bool is_valid(size_t p_handle);

    // Property access
    static Vector4i get_id(size_t p_handle);
    static String get_path(size_t p_handle);

    // Playback control
    static float get_volume(size_t p_handle);
    static float get_final_volume(size_t p_handle);
    static FMOD_RESULT set_volume(size_t p_handle, float p_volume);

    static bool get_paused(size_t p_handle);
    static FMOD_RESULT set_paused(size_t p_handle, bool p_paused);

    static bool get_mute(size_t p_handle);
    static FMOD_RESULT set_mute(size_t p_handle, bool p_mute);

    static FMOD_RESULT stop_all_events(size_t p_handle, FMOD_STUDIO_STOP_MODE p_mode);

    // Output port
    static uint64_t get_port_index(size_t p_handle);
    static FMOD_RESULT set_port_index(size_t p_handle, uint64_t p_index);

    // Low-level API access
    static FMOD_RESULT lock_channel_group(size_t p_handle);
    static FMOD_RESULT unlock_channel_group(size_t p_handle);
    // TODO:
    //  ChannelGroup *getChannelGroup() const
    //  {
    //      FMOD_CHANNELGROUP *group;
    //      FMOD_Studio_Bus_GetChannelGroup(handle, &group);
    //      return memnew(ChannelGroup(group));
    //  }

    // Monitoring
    static unsigned int getInclusiveCPUUsage(size_t p_handle);
    static unsigned int getExclusiveCPUUsage(size_t p_handle);
    // TODO:
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *memoryusage) const;
};
} // namespace Studio
} // namespace FmodGodot
