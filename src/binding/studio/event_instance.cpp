#include "event_instance.h"
#include "binding/studio/fmod_studio_system.h"
#include "core/class_db.hpp"
#include "event_description.h"
#include "fmod_enums.h"
#include "globals.h"
#include "variant/variant.hpp"
using namespace FmodGodot::Studio;
using namespace godot;
void FmodGodot::Studio::EventInstance::_bind_methods()
{
    BIND_METHOD(is_valid);
    BIND_METHOD(get_description);
    BIND_METHOD(get_system);
    BIND_PROPERTY(volume, Variant::FLOAT);
    BIND_METHOD(get_final_volume);
    BIND_PROPERTY(pitch, Variant::FLOAT);
    BIND_METHOD(get_final_pitch);
    BIND_PROPERTY(transform, Variant::TRANSFORM3D);
    BIND_PROPERTY(velocity, Variant::VECTOR3);
    BIND_PROPERTY(listener_mask, Variant::INT);
    BIND_METHOD(get_property, "index");
    BIND_METHOD(set_property, "index", "level");
    BIND_METHOD(get_reverb_level, "index");
    BIND_METHOD(set_reverb_level, "index", "level");
    BIND_PROPERTY(paused, Variant::BOOL);
    BIND_METHOD(start);
    BIND_METHOD(stop);
    BIND_PROPERTY(timeline_position, Variant::INT);
    BIND_METHOD(get_playback_state);
    BIND_METHOD(get_min_distance);
    BIND_METHOD(get_max_distance);
    BIND_METHOD(release);
    BIND_METHOD(is_virtual);
    BIND_METHOD(get_parameter_by_id, "id");
    BIND_METHOD(get_final_parameter_by_id, "id");
    BIND_METHOD(set_parameter_by_id, "id", "value");
    ClassDB ::bind_method(D_METHOD("set_parameter_by_id_with_label", "id", "label", "ignoreseekspeed"),
                          &self_type ::set_parameter_by_id_with_label, DEFVAL(false));
    // BIND_METHOD(set_parameters_by_ids, "ids", "values", "ignore_seek_speed")
    BIND_METHOD(get_parameter_by_name, "name")
    ClassDB ::bind_method(D_METHOD("set_parameter_by_name", "name", "value", "ignore_seek_speed"),
                          &self_type ::set_parameter_by_name, DEFVAL(false));
    ClassDB ::bind_method(D_METHOD("set_parameter_by_name_with_label", "name", "label", "ignore_seek_speed"),
                          &self_type ::set_parameter_by_name_with_label, DEFVAL(false));
    BIND_METHOD(key_off)
    BIND_METHOD(get_inclusive_cpu_usage)
    BIND_METHOD(get_exclusive_cpu_usage)
}
EventInstance::EventInstance(FMOD_STUDIO_EVENTINSTANCE *p_instance) : handle(p_instance)
{
}
bool EventInstance::is_valid() const
{
    return FMOD_Studio_EventInstance_IsValid(handle);
}
Ref<EventDescription> EventInstance::get_description() const
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_EventInstance_GetDescription(handle, &description);
    return memnew(EventDescription(description));
}
Ref<StudioSystem> EventInstance::get_system() const
{
    FMOD_STUDIO_SYSTEM *system;
    FMOD_Studio_EventInstance_GetSystem(handle, &system);
    return memnew(StudioSystem(system));
}
float EventInstance::get_volume() const
{
    float volume;
    FMOD_Studio_EventInstance_GetVolume(handle, &volume, nullptr);
    return volume;
}
float EventInstance::get_final_volume() const
{
    float volume;
    FMOD_Studio_EventInstance_GetVolume(handle, nullptr, &volume);
    return volume;
}
FMOD_RESULT EventInstance::set_volume(float p_volume)
{
    return FMOD_Studio_EventInstance_SetVolume(handle, p_volume);
}
float EventInstance::get_pitch() const
{
    float pitch;
    FMOD_Studio_EventInstance_GetPitch(handle, &pitch, nullptr);
    return pitch;
}
float EventInstance::get_final_pitch() const
{
    float pitch;
    FMOD_Studio_EventInstance_GetPitch(handle, nullptr, &pitch);
    return pitch;
}
FMOD_RESULT EventInstance::set_pitch(float p_pitch)
{
    return FMOD_Studio_EventInstance_SetPitch(handle, p_pitch);
}
Transform3D EventInstance::get_transform() const
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes(handle, &attr);
    const Vector3 position = FmodGodot::to_godot_vector(attr.position);
    const Vector3 up = FmodGodot::to_godot_vector(attr.up);
    const Vector3 forward = FmodGodot::to_godot_vector(attr.forward);
    return Transform3D(up.cross(forward), up, forward, position);
}
Vector3 EventInstance::get_velocity() const
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes(handle, &attr);
    return FmodGodot::to_godot_vector(attr.velocity);
}
FMOD_RESULT EventInstance::set_transform(const Transform3D &p_transform)
{
    FMOD_3D_ATTRIBUTES attr = FmodGodot::to_3d_attributes(p_transform);
    attr.velocity = FmodGodot::to_fmod_vector(get_velocity());
    return FMOD_Studio_EventInstance_Set3DAttributes(handle, &attr);
}
FMOD_RESULT EventInstance::set_velocity(const Vector3 &p_velocity)
{

    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes(handle, &attr);
    attr.velocity = FmodGodot::to_fmod_vector(p_velocity);
    return FMOD_Studio_EventInstance_Set3DAttributes(handle, &attr);
}

unsigned int EventInstance::get_listener_mask() const
{
    unsigned int mask;
    FMOD_Studio_EventInstance_GetListenerMask(handle, &mask);
    return mask;
}
FMOD_RESULT EventInstance::set_listener_mask(unsigned int p_mask)
{
    return FMOD_Studio_EventInstance_SetListenerMask(handle, p_mask);
}
float EventInstance::get_property(FMOD_STUDIO_EVENT_PROPERTY p_index) const
{
    float value;
    FMOD_Studio_EventInstance_GetProperty(handle, p_index, &value);
    return value;
}
FMOD_RESULT EventInstance::set_property(FMOD_STUDIO_EVENT_PROPERTY p_index, float p_value)
{
    return FMOD_Studio_EventInstance_SetProperty(handle, p_index, p_value);
}
float EventInstance::get_reverb_level(int p_index) const
{
    float level;
    FMOD_Studio_EventInstance_GetReverbLevel(handle, p_index, &level);
    return level;
}
FMOD_RESULT EventInstance::set_reverb_level(int p_index, float p_level)
{
    return FMOD_Studio_EventInstance_SetReverbLevel(handle, p_index, p_level);
}
bool EventInstance::get_paused() const
{
    FMOD_BOOL paused;
    FMOD_Studio_EventInstance_GetPaused(handle, &paused);
    return paused;
}
FMOD_RESULT EventInstance::set_paused(bool p_paused)
{
    return FMOD_Studio_EventInstance_SetPaused(handle, p_paused);
}
FMOD_RESULT EventInstance::start()
{
    return FMOD_Studio_EventInstance_Start(handle);
}
FMOD_RESULT EventInstance::stop(FMOD_STUDIO_STOP_MODE p_mode)
{
    return FMOD_Studio_EventInstance_Stop(handle, p_mode);
}
int EventInstance::get_timeline_position() const
{
    int position;
    FMOD_Studio_EventInstance_GetTimelinePosition(handle, &position);
    return position;
}
FMOD_RESULT EventInstance::set_timeline_position(int p_position)
{
    return FMOD_Studio_EventInstance_SetTimelinePosition(handle, p_position);
}
FMOD_STUDIO_PLAYBACK_STATE EventInstance::get_playback_state() const
{
    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_Studio_EventInstance_GetPlaybackState(handle, &state);
    return state;
}
float EventInstance::get_min_distance() const
{
    float min;
    FMOD_Studio_EventInstance_GetMinMaxDistance(handle, &min, nullptr);
    return min;
}
float EventInstance::get_max_distance() const
{
    float max;
    FMOD_Studio_EventInstance_GetMinMaxDistance(handle, nullptr, &max);
    return max;
}
FMOD_RESULT EventInstance::release()
{
    return FMOD_Studio_EventInstance_Release(handle);
}
bool EventInstance::is_virtual() const
{
    FMOD_BOOL virtualState;
    FMOD_Studio_EventInstance_IsVirtual(handle, &virtualState);
    return virtualState;
}
float EventInstance::get_parameter_by_id(Vector2i p_id) const
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByID(handle, FmodGodot::cast_to_parameter_id(p_id), &value, nullptr);
    return value;
}
float EventInstance::get_final_parameter_by_id(Vector2i p_id) const
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByID(handle, FmodGodot::cast_to_parameter_id(p_id), nullptr, &value);
    return value;
}
FMOD_RESULT EventInstance::set_parameter_by_id(Vector2i p_id, float p_value, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByID(handle, FmodGodot::cast_to_parameter_id(p_id), p_value,
                                                      p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameter_by_id_with_label(Vector2i p_id, const String &p_label, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByIDWithLabel(handle, FmodGodot::cast_to_parameter_id(p_id),
                                                               p_label.utf8().ptr(), p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameters_by_ids(const Vector<Vector2i> &p_ids, PackedFloat32Array &p_values,
                                                 bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParametersByIDs(handle, (FMOD_STUDIO_PARAMETER_ID *)p_ids.ptr(), p_values.ptrw(),
                                                        Math::min(p_values.size(), p_ids.size()), p_ignoreseekspeed);
}
float EventInstance::get_parameter_by_name(const String &p_name) const
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByName(handle, p_name.utf8().ptr(), &value, nullptr);
    return value;
}
float EventInstance::get_final_parameter_by_name(const String &p_name) const
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByName(handle, p_name.utf8().ptr(), nullptr, &value);
    return value;
}
FMOD_RESULT EventInstance::set_parameter_by_name(const String &p_name, float p_value, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByName(handle, p_name.utf8().ptr(), p_value, p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameter_by_name_with_label(const String &p_name, const String &p_label,
                                                            bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByNameWithLabel(handle, p_name.utf8().ptr(), p_label.utf8().ptr(),
                                                                 p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::key_off()
{
    return FMOD_Studio_EventInstance_KeyOff(handle);
}
unsigned int EventInstance::get_inclusive_cpu_usage() const
{
    unsigned int usage;
    FMOD_Studio_EventInstance_GetCPUUsage(handle, nullptr, &usage);
    return usage;
}
unsigned int EventInstance::get_exclusive_cpu_usage() const
{
    unsigned int usage;
    FMOD_Studio_EventInstance_GetCPUUsage(handle, &usage, nullptr);
    return usage;
}
void *EventInstance::get_user_data() const
{
    void *user_data;
    FMOD_Studio_EventInstance_GetUserData(handle, &user_data);
    return user_data;
}
FMOD_RESULT EventInstance::set_user_data(void *p_userdata)
{
    return FMOD_Studio_EventInstance_SetUserData(handle, p_userdata);
}
