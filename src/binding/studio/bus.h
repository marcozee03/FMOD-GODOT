#pragma once
#include "classes/object.hpp"
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

class Bus : Object
{
    GDCLASS(Bus, Object);

  private:
    FMOD_STUDIO_BUS *handle;
    // Constructor made private so user cannot statically instance the class.

  protected:
    static void _bind_methods();

  public:
    operator FMOD_STUDIO_BUS *() const
    {
        return handle;
    }
    Bus() = default;
    ~Bus() = default;
    Bus(FMOD_STUDIO_BUS *p_bus);
    // Handle validity
    bool is_valid() const;

    // Property access
    godot::Vector4i get_id() const;
    String get_path() const;

    // Playback control
    float get_volume() const;
    float get_final_volume() const;
    FMOD_RESULT set_volume(float p_volume);

    bool get_paused() const;
    FMOD_RESULT set_paused(bool p_paused);

    bool get_mute() const;
    FMOD_RESULT set_mute(bool p_mute);

    FMOD_RESULT stop_all_events(FMOD_STUDIO_STOP_MODE p_mode);

    // Output port
    uint64_t get_port_index() const;
    FMOD_RESULT set_port_index(uint64_t p_index);

    // Low-level API access
    FMOD_RESULT lock_channel_group();
    FMOD_RESULT unlock_channel_group();
    // TODO:
    //  ChannelGroup *getChannelGroup() const
    //  {
    //      FMOD_CHANNELGROUP *group;
    //      FMOD_Studio_Bus_GetChannelGroup(handle, &group);
    //      return memnew(ChannelGroup(group));
    //  }

    // Monitoring
    unsigned int getInclusiveCPUUsage() const;
    unsigned int getExclusiveCPUUsage() const;
    // TODO:
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *memoryusage) const;
};
} // namespace Studio
} // namespace FmodGodot
