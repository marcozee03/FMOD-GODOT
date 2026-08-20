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
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_description, "handle");
    BIND_STATIC_METHOD(get_system, "handle");
    BIND_STATIC_METHOD(get_volume, "handle");
    BIND_STATIC_METHOD(set_volume, "handle", "volume");
    BIND_STATIC_METHOD(get_final_volume, "handle");
    BIND_STATIC_METHOD(get_pitch, "handle");
    BIND_STATIC_METHOD(set_pitch, "handle", "pitch");
    BIND_STATIC_METHOD(get_final_pitch, "handle");
    BIND_STATIC_METHOD(get_transform, "handle");
    BIND_STATIC_METHOD(set_transform, "handle", "transform");
    BIND_STATIC_METHOD(set_3d_attributes, "handle", "transform", "velocity");
    BIND_STATIC_METHOD(get_velocity, "handle");
    BIND_STATIC_METHOD(set_velocity, "handle", "velocity");
    BIND_STATIC_METHOD(get_listener_mask, "handle");
    BIND_STATIC_METHOD(set_listener_mask, "handle", "listener_mask");
    BIND_STATIC_METHOD(get_property, "handle", "index");
    BIND_STATIC_METHOD(set_property, "handle", "index", "level");
    BIND_STATIC_METHOD(get_reverb_level, "handle", "index");
    BIND_STATIC_METHOD(set_reverb_level, "handle", "index", "level");
    BIND_STATIC_METHOD(get_paused, "handle");
    BIND_STATIC_METHOD(set_paused, "handle", "paused");
    BIND_STATIC_METHOD(start, "handle");
    BIND_STATIC_METHOD(stop, "handle");
    BIND_STATIC_METHOD(get_timeline_position, "handle");
    BIND_STATIC_METHOD(set_timeline_position, "handle", "timeline_position");
    BIND_STATIC_METHOD(get_playback_state, "handle");
    BIND_STATIC_METHOD(get_min_distance, "handle");
    BIND_STATIC_METHOD(get_max_distance, "handle");
    BIND_STATIC_METHOD(release, "handle");
    BIND_STATIC_METHOD(is_virtual, "handle");
    BIND_STATIC_METHOD(get_parameter_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_final_parameter_by_id, "handle", "id");
    BIND_STATIC_METHOD(set_parameter_by_id, "handle", "id", "value");
    ClassDB ::bind_static_method(get_class_static(),
                                 D_METHOD("set_parameter_by_id_with_label", "handle", "id", "label", "ignoreseekspeed"),
                                 &self_type ::set_parameter_by_id_with_label, DEFVAL(false));
    // BIND_STATIC_METHOD(set_parameters_by_ids, "ids", "values", "ignore_seek_speed")
    BIND_STATIC_METHOD(get_parameter_by_name, "handle", "name");
    ClassDB ::bind_static_method(get_class_static(),
                                 D_METHOD("set_parameter_by_name", "handle", "name", "value", "ignore_seek_speed"),
                                 &self_type ::set_parameter_by_name, DEFVAL(false));
    ClassDB ::bind_static_method(
        get_class_static(),
        D_METHOD("set_parameter_by_name_with_label", "handle", "name", "label", "ignore_seek_speed"),
        &self_type ::set_parameter_by_name_with_label, DEFVAL(false));
    BIND_STATIC_METHOD(key_off, "handle");
    BIND_STATIC_METHOD(get_inclusive_cpu_usage, "handle");
    BIND_STATIC_METHOD(get_exclusive_cpu_usage, "handle");
}
bool EventInstance::is_valid(size_t p_handle)
{
    return FMOD_Studio_EventInstance_IsValid((FMOD_STUDIO_EVENTINSTANCE *)p_handle);
}
FMOD_RESULT FmodGodot::Studio::EventInstance::set_3d_attributes(size_t p_handle, const Transform3D &p_transform,
                                                                const Vector3 &p_velocity)
{
    FMOD_3D_ATTRIBUTES attr = to_3d_attributes(p_transform);
    attr.velocity = to_fmod_vector(p_velocity);
    return FMOD_Studio_EventInstance_Set3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
}

size_t EventInstance::get_description(size_t p_handle)
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_EventInstance_GetDescription((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &description);
    return (size_t)description;
}
size_t EventInstance::get_system(size_t p_handle)
{
    FMOD_STUDIO_SYSTEM *system;
    FMOD_Studio_EventInstance_GetSystem((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &system);
    return (size_t)system;
}
float EventInstance::get_volume(size_t p_handle)
{
    float volume;
    FMOD_Studio_EventInstance_GetVolume((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &volume, nullptr);
    return volume;
}
float EventInstance::get_final_volume(size_t p_handle)
{
    float volume;
    FMOD_Studio_EventInstance_GetVolume((FMOD_STUDIO_EVENTINSTANCE *)p_handle, nullptr, &volume);
    return volume;
}
FMOD_RESULT EventInstance::set_volume(size_t p_handle, float p_volume)
{
    return FMOD_Studio_EventInstance_SetVolume((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_volume);
}
float EventInstance::get_pitch(size_t p_handle)
{
    float pitch;
    FMOD_Studio_EventInstance_GetPitch((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &pitch, nullptr);
    return pitch;
}
float EventInstance::get_final_pitch(size_t p_handle)
{
    float pitch;
    FMOD_Studio_EventInstance_GetPitch((FMOD_STUDIO_EVENTINSTANCE *)p_handle, nullptr, &pitch);
    return pitch;
}
FMOD_RESULT EventInstance::set_pitch(size_t p_handle, float p_pitch)
{
    return FMOD_Studio_EventInstance_SetPitch((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_pitch);
}
Transform3D EventInstance::get_transform(size_t p_handle)
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
    const Vector3 position = FmodGodot::to_godot_vector(attr.position);
    const Vector3 up = FmodGodot::to_godot_vector(attr.up);
    const Vector3 forward = FmodGodot::to_godot_vector(attr.forward);
    return Transform3D(up.cross(forward), up, forward, position);
}
Vector3 EventInstance::get_velocity(size_t p_handle)
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
    return FmodGodot::to_godot_vector(attr.velocity);
}
FMOD_RESULT EventInstance::set_transform(size_t p_handle, const Transform3D &p_transform)
{
    FMOD_3D_ATTRIBUTES attr = FmodGodot::to_3d_attributes(p_transform);
    attr.velocity = FmodGodot::to_fmod_vector(get_velocity(p_handle));
    return FMOD_Studio_EventInstance_Set3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
}
FMOD_RESULT EventInstance::set_velocity(size_t p_handle, const Vector3 &p_velocity)
{

    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_EventInstance_Get3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
    attr.velocity = FmodGodot::to_fmod_vector(p_velocity);
    return FMOD_Studio_EventInstance_Set3DAttributes((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &attr);
}

unsigned int EventInstance::get_listener_mask(size_t p_handle)
{
    unsigned int mask;
    FMOD_Studio_EventInstance_GetListenerMask((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &mask);
    return mask;
}
FMOD_RESULT EventInstance::set_listener_mask(size_t p_handle, unsigned int p_mask)
{
    return FMOD_Studio_EventInstance_SetListenerMask((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_mask);
}
float EventInstance::get_property(size_t p_handle, FMOD_STUDIO_EVENT_PROPERTY p_index)
{
    float value;
    FMOD_Studio_EventInstance_GetProperty((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_index, &value);
    return value;
}
FMOD_RESULT EventInstance::set_property(size_t p_handle, FMOD_STUDIO_EVENT_PROPERTY p_index, float p_value)
{
    return FMOD_Studio_EventInstance_SetProperty((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_index, p_value);
}
float EventInstance::get_reverb_level(size_t p_handle, int p_index)
{
    float level;
    FMOD_Studio_EventInstance_GetReverbLevel((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_index, &level);
    return level;
}
FMOD_RESULT EventInstance::set_reverb_level(size_t p_handle, int p_index, float p_level)
{
    return FMOD_Studio_EventInstance_SetReverbLevel((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_index, p_level);
}
bool EventInstance::get_paused(size_t p_handle)
{
    FMOD_BOOL paused;
    FMOD_Studio_EventInstance_GetPaused((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &paused);
    return paused;
}
FMOD_RESULT EventInstance::set_paused(size_t p_handle, bool p_paused)
{
    return FMOD_Studio_EventInstance_SetPaused((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_paused);
}
FMOD_RESULT EventInstance::start(size_t p_handle)
{
    return FMOD_Studio_EventInstance_Start((FMOD_STUDIO_EVENTINSTANCE *)p_handle);
}
FMOD_RESULT EventInstance::stop(size_t p_handle, FMOD_STUDIO_STOP_MODE p_mode)
{
    return FMOD_Studio_EventInstance_Stop((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_mode);
}
int EventInstance::get_timeline_position(size_t p_handle)
{
    int position;
    FMOD_Studio_EventInstance_GetTimelinePosition((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &position);
    return position;
}
FMOD_RESULT EventInstance::set_timeline_position(size_t p_handle, int p_position)
{
    return FMOD_Studio_EventInstance_SetTimelinePosition((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_position);
}
FMOD_STUDIO_PLAYBACK_STATE EventInstance::get_playback_state(size_t p_handle)
{
    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_Studio_EventInstance_GetPlaybackState((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &state);
    return state;
}
float EventInstance::get_min_distance(size_t p_handle)
{
    float min;
    FMOD_Studio_EventInstance_GetMinMaxDistance((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &min, nullptr);
    return min;
}
float EventInstance::get_max_distance(size_t p_handle)
{
    float max;
    FMOD_Studio_EventInstance_GetMinMaxDistance((FMOD_STUDIO_EVENTINSTANCE *)p_handle, nullptr, &max);
    return max;
}
FMOD_RESULT EventInstance::release(size_t p_handle)
{
    return FMOD_Studio_EventInstance_Release((FMOD_STUDIO_EVENTINSTANCE *)p_handle);
}
bool EventInstance::is_virtual(size_t p_handle)
{
    FMOD_BOOL virtualState;
    FMOD_Studio_EventInstance_IsVirtual((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &virtualState);
    return virtualState;
}
float EventInstance::get_parameter_by_id(size_t p_handle, uint64_t p_id)
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByID((FMOD_STUDIO_EVENTINSTANCE *)p_handle,
                                               FmodGodot::cast_to_parameter_id(p_id), &value, nullptr);
    return value;
}
float EventInstance::get_final_parameter_by_id(size_t p_handle, uint64_t p_id)
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByID((FMOD_STUDIO_EVENTINSTANCE *)p_handle,
                                               FmodGodot::cast_to_parameter_id(p_id), nullptr, &value);
    return value;
}
FMOD_RESULT EventInstance::set_parameter_by_id(size_t p_handle, uint64_t p_id, float p_value, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByID(
        (FMOD_STUDIO_EVENTINSTANCE *)p_handle, FmodGodot::cast_to_parameter_id(p_id), p_value, p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameter_by_id_with_label(size_t p_handle, uint64_t p_id, const String &p_label,
                                                          bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByIDWithLabel((FMOD_STUDIO_EVENTINSTANCE *)p_handle,
                                                               FmodGodot::cast_to_parameter_id(p_id),
                                                               p_label.utf8().ptr(), p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameters_by_ids(size_t p_handle, const Vector<uint64_t> &p_ids,
                                                 PackedFloat32Array &p_values, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParametersByIDs((FMOD_STUDIO_EVENTINSTANCE *)p_handle,
                                                        (FMOD_STUDIO_PARAMETER_ID *)p_ids.ptr(), p_values.ptrw(),
                                                        Math::min(p_values.size(), p_ids.size()), p_ignoreseekspeed);
}
float EventInstance::get_parameter_by_name(size_t p_handle, const String &p_name)
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByName((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_name.utf8().ptr(), &value,
                                                 nullptr);
    return value;
}
float EventInstance::get_final_parameter_by_name(size_t p_handle, const String &p_name)
{
    float value;
    FMOD_Studio_EventInstance_GetParameterByName((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_name.utf8().ptr(), nullptr,
                                                 &value);
    return value;
}
FMOD_RESULT EventInstance::set_parameter_by_name(size_t p_handle, const String &p_name, float p_value,
                                                 bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByName((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_name.utf8().ptr(),
                                                        p_value, p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::set_parameter_by_name_with_label(size_t p_handle, const String &p_name,
                                                            const String &p_label, bool p_ignoreseekspeed)
{
    return FMOD_Studio_EventInstance_SetParameterByNameWithLabel(
        (FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_name.utf8().ptr(), p_label.utf8().ptr(), p_ignoreseekspeed);
}
FMOD_RESULT EventInstance::key_off(size_t p_handle)
{
    return FMOD_Studio_EventInstance_KeyOff((FMOD_STUDIO_EVENTINSTANCE *)p_handle);
}
unsigned int EventInstance::get_inclusive_cpu_usage(size_t p_handle)
{
    unsigned int usage;
    FMOD_Studio_EventInstance_GetCPUUsage((FMOD_STUDIO_EVENTINSTANCE *)p_handle, nullptr, &usage);
    return usage;
}
unsigned int EventInstance::get_exclusive_cpu_usage(size_t p_handle)
{
    unsigned int usage;
    FMOD_Studio_EventInstance_GetCPUUsage((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &usage, nullptr);
    return usage;
}
void *EventInstance::get_user_data(size_t p_handle)
{
    void *user_data;
    FMOD_Studio_EventInstance_GetUserData((FMOD_STUDIO_EVENTINSTANCE *)p_handle, &user_data);
    return user_data;
}
FMOD_RESULT EventInstance::set_user_data(size_t p_handle, void *p_userdata)
{
    return FMOD_Studio_EventInstance_SetUserData((FMOD_STUDIO_EVENTINSTANCE *)p_handle, p_userdata);
}
