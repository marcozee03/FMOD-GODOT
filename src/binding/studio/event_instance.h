#pragma once
#include "classes/wrapped.hpp"
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/transform3d.hpp"
using namespace godot;
namespace FmodGodot
{
namespace Studio
{
class StudioSystem;
class EventDescription;

class EventInstance : public Object
{
    GDCLASS(EventInstance, Object)
  private:
  protected:
    static void _bind_methods();

  public:
    EventInstance() = default;
    ~EventInstance() = default;
    // Handle validity
    static bool is_valid(Handle p_handle);

    // Property access
    static Handle get_description(Handle p_handle);
    static Handle get_system(Handle p_handle);

    // Playback control
    static float get_volume(Handle p_handle);
    static float get_final_volume(Handle p_handle);
    static FMOD_RESULT set_volume(Handle p_handle, float p_volume);

    static float get_pitch(Handle p_handle);
    static float get_final_pitch(Handle p_handle);
    static FMOD_RESULT set_pitch(Handle p_handle, float p_pitch);

    static Transform3D get_transform(Handle p_handle);
    static Vector3 get_velocity(Handle p_handle);
    static FMOD_RESULT set_transform(Handle p_handle, const Transform3D &p_transform);
    static FMOD_RESULT set_3d_attributes(Handle p_handle, const Transform3D &p_transform, const Vector3 &p_velocity);
    static FMOD_RESULT set_velocity(Handle p_handle, const Vector3 &p_velocity);

    static unsigned int get_listener_mask(Handle p_handle);
    static FMOD_RESULT set_listener_mask(Handle p_handle, unsigned int p_mask);

    static float get_property(Handle p_handle, FMOD_STUDIO_EVENT_PROPERTY p_index);
    static FMOD_RESULT set_property(Handle p_handle, FMOD_STUDIO_EVENT_PROPERTY p_index, float p_value);

    static float get_reverb_level(Handle p_handle, int p_index);
    static FMOD_RESULT set_reverb_level(Handle p_handle, int p_index, float p_level);

    static bool get_paused(Handle p_handle);
    static FMOD_RESULT set_paused(Handle p_handle, bool p_paused);

    static FMOD_RESULT start(Handle p_handle);
    static FMOD_RESULT stop(Handle p_handle, FMOD_STUDIO_STOP_MODE p_mode);

    static int get_timeline_position(Handle p_handle);
    static FMOD_RESULT set_timeline_position(Handle p_handle, int p_position);

    static FMOD_STUDIO_PLAYBACK_STATE get_playback_state(Handle p_handle);

    // TODO:
    // FMOD_RESULT getChannelGroup(ChannelGroup **group) const;

    static float get_min_distance(Handle p_handle);
    static float get_max_distance(Handle p_handle);

    static FMOD_RESULT release(Handle p_handle);

    static bool is_virtual(Handle p_handle);

    static float get_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id);
    static float get_final_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id);
    static FMOD_RESULT set_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id, float p_value,
                                           bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameter_by_id_with_label(Handle p_handle, GD_PARAMETER_ID p_id, const String &p_label,
                                                      bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameters_by_ids(Handle p_handle, const PackedInt64Array &p_ids,
                                             PackedFloat32Array &p_values, bool p_ignoreseekspeed = false);

    static float get_parameter_by_name(Handle p_handle, const String &p_name);
    static float get_final_parameter_by_name(Handle p_handle, const String &p_name);
    static FMOD_RESULT set_parameter_by_name(Handle p_handle, const String &p_name, float p_value,
                                             bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameter_by_name_with_label(Handle p_handle, const String &p_name, const String &p_label,
                                                        bool p_ignoreseekspeed = false);

    static FMOD_RESULT key_off(Handle p_handle);

    // Monitoring
    static unsigned int get_inclusive_cpu_usage(Handle p_handle);
    static unsigned int get_exclusive_cpu_usage(Handle p_handle);
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *memoryusage) const;

    // Callbacks
    // FMOD_RESULT setCallback(FMOD_STUDIO_EVENT_CALLBACK callback,
    // FMOD_STUDIO_EVENT_CALLBACK_TYPE callbackmask = FMOD_STUDIO_EVENT_CALLBACK_ALL);
    void *get_user_data(Handle p_handle);
    static FMOD_RESULT set_user_data(Handle p_handle, void *p_userdata);
};
} // namespace Studio
} // namespace FmodGodot
