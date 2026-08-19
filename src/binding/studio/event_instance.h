#pragma once
#include "classes/ref.hpp"
#include "classes/wrapped.hpp"
#include "fmod_common.h"
#include "fmod_studio_common.h"
#include "variant/transform3d.hpp"
using namespace godot;
namespace FmodGodot
{
namespace Studio
{
class StudioSystem;
class EventDescription;

class EventInstance : public RefCounted
{
    GDCLASS(EventInstance, RefCounted)
  private:
    // Constructor made private so user cannot statically instance the class.
    FMOD_STUDIO_EVENTINSTANCE *handle;

  protected:
    static void _bind_methods();

  public:
    EventInstance() = default;
    ~EventInstance() = default;
    EventInstance(FMOD_STUDIO_EVENTINSTANCE *p_instance);
    operator FMOD_STUDIO_EVENTINSTANCE *() const
    {
        return handle;
    }
    // Handle validity
    bool is_valid() const;

    // Property access
    Ref<EventDescription> get_description() const;
    Ref<StudioSystem> get_system() const;

    // Playback control
    float get_volume() const;
    float get_final_volume() const;
    FMOD_RESULT set_volume(float p_volume);

    float get_pitch() const;
    float get_final_pitch() const;
    FMOD_RESULT set_pitch(float p_pitch);

    Transform3D get_transform() const;
    Vector3 get_velocity() const;
    FMOD_RESULT set_transform(const Transform3D &p_transform);
    FMOD_RESULT set_velocity(const Vector3 &p_velocity);

    unsigned int get_listener_mask() const;
    FMOD_RESULT set_listener_mask(unsigned int p_mask);

    float get_property(FMOD_STUDIO_EVENT_PROPERTY p_index) const;
    FMOD_RESULT set_property(FMOD_STUDIO_EVENT_PROPERTY p_index, float p_value);

    float get_reverb_level(int p_index) const;
    FMOD_RESULT set_reverb_level(int p_index, float p_level);

    bool get_paused() const;
    FMOD_RESULT set_paused(bool p_paused);

    FMOD_RESULT start();
    FMOD_RESULT stop(FMOD_STUDIO_STOP_MODE p_mode);

    int get_timeline_position() const;
    FMOD_RESULT set_timeline_position(int p_position);

    FMOD_STUDIO_PLAYBACK_STATE get_playback_state() const;

    // TODO:
    // FMOD_RESULT getChannelGroup(ChannelGroup **group) const;

    float get_min_distance() const;
    float get_max_distance() const;

    FMOD_RESULT release();

    bool is_virtual() const;

    float get_parameter_by_id(Vector2i p_id) const;
    float get_final_parameter_by_id(Vector2i p_id) const;
    FMOD_RESULT set_parameter_by_id(Vector2i p_id, float p_value, bool p_ignoreseekspeed = false);
    FMOD_RESULT set_parameter_by_id_with_label(Vector2i p_id, const String &p_label, bool p_ignoreseekspeed = false);
    FMOD_RESULT set_parameters_by_ids(const Vector<Vector2i> &p_ids, PackedFloat32Array &p_values,
                                      bool p_ignoreseekspeed = false);

    float get_parameter_by_name(const String &p_name) const;
    float get_final_parameter_by_name(const String &p_name) const;
    FMOD_RESULT set_parameter_by_name(const String &p_name, float p_value, bool p_ignoreseekspeed = false);
    FMOD_RESULT set_parameter_by_name_with_label(const String &p_name, const String &p_label,
                                                 bool p_ignoreseekspeed = false);

    FMOD_RESULT key_off();

    // Monitoring
    unsigned int get_inclusive_cpu_usage() const;
    unsigned int get_exclusive_cpu_usage() const;
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *memoryusage) const;

    // Callbacks
    // FMOD_RESULT setCallback(FMOD_STUDIO_EVENT_CALLBACK callback,
    // FMOD_STUDIO_EVENT_CALLBACK_TYPE callbackmask = FMOD_STUDIO_EVENT_CALLBACK_ALL);
    void *get_user_data() const;
    FMOD_RESULT set_user_data(void *p_userdata);
};
} // namespace Studio
} // namespace FmodGodot
