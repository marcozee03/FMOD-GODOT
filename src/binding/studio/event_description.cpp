#include "event_description.h"
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_enums.h"
#include "globals.h"
using namespace godot;
using namespace FmodGodot::Studio;
using namespace FmodGodot;
#ifdef TOOLS_ENABLED
FmodGodot::Studio::StudioEventDescription::Cache::Cache(FMOD_STUDIO_EVENTDESCRIPTION *p_description)
{

    FMOD_GET_OUT_STRING(FMOD_Studio_EventDescription_GetPath, p_description, full_path);
    FMOD_GUID fguid;
    FMOD_Studio_EventDescription_GetID(p_description, &fguid);
    guid = FmodGodot::cast_to_vector4i(fguid);
    FMOD_BOOL is3D, isDopplerEnabled, isStream, isOneShot;
    FMOD_Studio_EventDescription_Is3D(p_description, &is3D);
    FMOD_Studio_EventDescription_IsDopplerEnabled(p_description, &isDopplerEnabled);
    FMOD_Studio_EventDescription_IsOneshot(p_description, &isOneShot);
    FMOD_Studio_EventDescription_IsStream(p_description, &isStream);
    FMOD_Studio_EventDescription_GetLength(p_description, &lengthMS);

    FMOD_Studio_EventDescription_GetMinMaxDistance(p_description, &min, &max);
    is3d = is3D;
    doppler_enabled = isDopplerEnabled;
    stream = isStream;
    one_shot = isOneShot;
    int parameter_count;
    FMOD_Studio_EventDescription_GetParameterDescriptionCount(p_description, &parameter_count);
    for (int k = 0; k < parameter_count; k++)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION parameter_description;
        FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(p_description, k, &parameter_description);
        FmodGodot::ParameterCache parameter(parameter_description);
        parameters.append(parameter);
    }
}

#endif
void FmodGodot::Studio::StudioEventDescription::_bind_methods()
{
    // ADD_SIGNAL(MethodInfo("created"));
    // ADD_SIGNAL(MethodInfo("destroyed"));
    // ADD_SIGNAL(MethodInfo("starting"));
    // ADD_SIGNAL(MethodInfo("started"));
    // ADD_SIGNAL(MethodInfo("restarted"));
    // ADD_SIGNAL(MethodInfo("stopped"));
    // ADD_SIGNAL(MethodInfo("start_failed"));
    // // TODO:FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES;
    // ADD_SIGNAL(MethodInfo("create_programmer_sound", PropertyInfo()));
    // // TODO:FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES;
    // ADD_SIGNAL(MethodInfo("destroy_programmer_sound", PropertyInfo()));
    // // TODO:FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES;
    // ADD_SIGNAL(MethodInfo("plugin_created", PropertyInfo()));
    // // TODO:FMOD_STUDIO_PLUGIN_INSTANCE_PROPERTIES;
    // ADD_SIGNAL(MethodInfo("plugin_destroyed", PropertyInfo()));
    // // TODO: FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES
    // ADD_SIGNAL(MethodInfo("timeline_marker", PropertyInfo()));
    // // TODO: FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES
    // ADD_SIGNAL(MethodInfo("timeline_beat", PropertyInfo()));
    // // TODO:Sound
    // ADD_SIGNAL(MethodInfo("sound_played", PropertyInfo()));
    // // TODO:Sound
    // ADD_SIGNAL(MethodInfo("sound_stopped", PropertyInfo()));
    // ADD_SIGNAL(MethodInfo("real_to_virtual"));
    // ADD_SIGNAL(MethodInfo("virtual_to_real"));
    // // TODO:EventInstance
    // ADD_SIGNAL(MethodInfo("start_event_command"));
    // // TODO:FMOD_STUDIO_TIMELINE_NESTED_BEAT_PROPERTIES.
    // ADD_SIGNAL(MethodInfo("nested_timeline_beat"));
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_id, "handle");
    BIND_STATIC_METHOD(get_path, "handle");
    // TODO:
    BIND_STATIC_METHOD(get_parameter_description_count, "handle");
    //  int get_parameter_description_count() const;
    //  FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByIndex(int p_index) const;
    //  FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByName(const String &p_name) const;
    //  FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByID(GD_PARAMETER_ID p_id) const;
    BIND_STATIC_METHOD(get_parameter_label_by_index, "handle", "index", "label_index");
    BIND_STATIC_METHOD(get_parameter_label_by_name, "handle", "name", "label_index");
    BIND_STATIC_METHOD(get_parameter_label_by_id, "handle", "id", "label_index");
    BIND_STATIC_METHOD(get_user_property_count, "handle");
    BIND_STATIC_METHOD(get_user_property_by_index, "handle", "index");
    BIND_STATIC_METHOD(get_user_property, "handle", "name");
    BIND_STATIC_METHOD(get_length, "handle");
    BIND_STATIC_METHOD(get_min_distance, "handle");
    BIND_STATIC_METHOD(get_max_distance, "handle");
    BIND_STATIC_METHOD(get_sound_size, "handle");
    BIND_STATIC_METHOD(is_snapshot, "handle");
    BIND_STATIC_METHOD(is_oneshot, "handle");
    BIND_STATIC_METHOD(is_stream, "handle");
    BIND_STATIC_METHOD(is3D, "handle");
    BIND_STATIC_METHOD(is_doppler_enabled, "handle");
    BIND_STATIC_METHOD(has_sustain_point, "handle");
    BIND_STATIC_METHOD(create_instance, "handle");

    BIND_STATIC_METHOD(get_instance_count, "handle");
    BIND_STATIC_METHOD(get_instance_list, "handle");

    BIND_STATIC_METHOD(load_sample_data, "handle");
    BIND_STATIC_METHOD(unload_sample_data, "handle");
    BIND_STATIC_METHOD(get_sample_loading_state, "handle");

    BIND_STATIC_METHOD(release_all_instances, "handle");

    // void *get_user_data() const;
    // FMOD_RESULT set_user_data(void *p_userdata);
}
bool FmodGodot::Studio::StudioEventDescription::is_valid(Handle p_handle)
{
    return FMOD_Studio_EventDescription_IsValid((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle);
}
godot::Vector4i FmodGodot::Studio::StudioEventDescription::get_id(Handle p_handle)
{
    FMOD_GUID guid;
    FMOD_Studio_EventDescription_GetID((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &guid);
    return FmodGodot::cast_to_vector4i(guid);
}
String FmodGodot::Studio::StudioEventDescription::get_path(Handle p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_EventDescription_GetPath, (FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, path);
    return path;
}
int FmodGodot::Studio::StudioEventDescription::get_parameter_description_count(Handle p_handle)
{
    int count;
    FMOD_Studio_EventDescription_GetParameterDescriptionCount((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &count);
    return count;
}
FMOD_STUDIO_PARAMETER_DESCRIPTION FmodGodot::Studio::StudioEventDescription::getParameterDescriptionByIndex(Handle p_handle,
                                                                                                      int p_index)
{
    FMOD_STUDIO_PARAMETER_DESCRIPTION parameter;
    FMOD_Studio_EventDescription_GetParameterDescriptionByIndex((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, p_index,
                                                                &parameter);
    return parameter;
}
FMOD_STUDIO_PARAMETER_DESCRIPTION FmodGodot::Studio::StudioEventDescription::getParameterDescriptionByName(
    Handle p_handle, const String &p_name)
{
    FMOD_STUDIO_PARAMETER_DESCRIPTION parameter;
    FMOD_Studio_EventDescription_GetParameterDescriptionByName((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                                                               p_name.utf8().ptr(), &parameter);
    return parameter;
}
FMOD_STUDIO_PARAMETER_DESCRIPTION FmodGodot::Studio::StudioEventDescription::getParameterDescriptionByID(Handle p_handle,
                                                                                                   GD_PARAMETER_ID p_id)
{
    FMOD_STUDIO_PARAMETER_DESCRIPTION parameter;
    FMOD_Studio_EventDescription_GetParameterDescriptionByID((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                                                             FmodGodot::cast_to_parameter_id(p_id), &parameter);
    return parameter;
}
godot::String FmodGodot::Studio::StudioEventDescription::get_parameter_label_by_index(Handle p_handle, int p_index,
                                                                                int p_labelindex)
{
    FMOD_LOOKUP_STRING(FMOD_Studio_EventDescription_GetParameterLabelByIndex, (FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                       label, p_index, p_labelindex);
    return label;
}
godot::String FmodGodot::Studio::StudioEventDescription::get_parameter_label_by_name(Handle p_handle, const String &p_name,
                                                                               int p_labelindex)
{
    FMOD_LOOKUP_STRING(FMOD_Studio_EventDescription_GetParameterLabelByName, (FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                       label, p_name.utf8().ptr(), p_labelindex);
    return label;
}
godot::String FmodGodot::Studio::StudioEventDescription::get_parameter_label_by_id(Handle p_handle, GD_PARAMETER_ID p_id,
                                                                             int p_labelindex)
{

    FMOD_LOOKUP_STRING(FMOD_Studio_EventDescription_GetParameterLabelByID, (FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                       label, FmodGodot::cast_to_parameter_id(p_id), p_labelindex);
    return label;
}
int FmodGodot::Studio::StudioEventDescription::get_user_property_count(Handle p_handle)
{
    int count;
    FMOD_Studio_EventDescription_GetUserPropertyCount((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &count);
    return count;
}
godot::Variant FmodGodot::Studio::StudioEventDescription::get_user_property_by_index(Handle p_handle, int p_index)
{
    FMOD_STUDIO_USER_PROPERTY property;
    FMOD_Studio_EventDescription_GetUserPropertyByIndex((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, p_index, &property);
    return to_variant(property);
}
godot::Variant FmodGodot::Studio::StudioEventDescription::get_user_property(Handle p_handle, const String &p_name)
{
    FMOD_STUDIO_USER_PROPERTY property;
    FMOD_Studio_EventDescription_GetUserProperty((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, p_name.utf8().ptr(),
                                                 &property);
    return to_variant(property);
}
int FmodGodot::Studio::StudioEventDescription::get_length(Handle p_handle)
{
    int length;
    FMOD_Studio_EventDescription_GetLength((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &length);
    return length;
}
float FmodGodot::Studio::StudioEventDescription::get_min_distance(Handle p_handle)
{
    float min;
    FMOD_Studio_EventDescription_GetMinMaxDistance((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &min, nullptr);
    return min;
}
float FmodGodot::Studio::StudioEventDescription::get_max_distance(Handle p_handle)
{
    float max;
    FMOD_Studio_EventDescription_GetMinMaxDistance((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, nullptr, &max);
    return max;
}
float FmodGodot::Studio::StudioEventDescription::get_sound_size(Handle p_handle)
{
    float soundsize;
    FMOD_Studio_EventDescription_GetSoundSize((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &soundsize);
    return soundsize;
}
bool FmodGodot::Studio::StudioEventDescription::is_snapshot(Handle p_handle)
{
    FMOD_BOOL snapshot;
    FMOD_Studio_EventDescription_IsSnapshot((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &snapshot);
    return snapshot;
}
bool FmodGodot::Studio::StudioEventDescription::is_oneshot(Handle p_handle)
{
    FMOD_BOOL oneshot;
    FMOD_Studio_EventDescription_IsOneshot((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &oneshot);
    return oneshot;
}
bool FmodGodot::Studio::StudioEventDescription::is_stream(Handle p_handle)
{
    FMOD_BOOL isStream;
    FMOD_Studio_EventDescription_IsStream((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &isStream);
    return isStream;
}
bool FmodGodot::Studio::StudioEventDescription::is3D(Handle p_handle)
{
    FMOD_BOOL is3D;
    FMOD_Studio_EventDescription_Is3D((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &is3D);
    return is3D;
}
bool FmodGodot::Studio::StudioEventDescription::is_doppler_enabled(Handle p_handle)
{
    FMOD_BOOL doppler;
    FMOD_Studio_EventDescription_IsDopplerEnabled((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &doppler);
    return doppler;
}
bool FmodGodot::Studio::StudioEventDescription::has_sustain_point(Handle p_handle)
{
    FMOD_BOOL sustainPoint;
    FMOD_Studio_EventDescription_HasSustainPoint((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &sustainPoint);
    return sustainPoint;
}
Handle FmodGodot::Studio::StudioEventDescription::create_instance(Handle p_handle)
{
    FMOD_STUDIO_EVENTINSTANCE *instance;
    FMOD_Studio_EventDescription_CreateInstance((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &instance);
    return (Handle)instance;
}
int FmodGodot::Studio::StudioEventDescription::get_instance_count(Handle p_handle)
{
    int count;
    FMOD_Studio_EventDescription_GetInstanceCount((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &count);
    return count;
}
PackedInt64Array FmodGodot::Studio::StudioEventDescription::get_instance_list(Handle p_handle)
{
    PackedInt64Array instance_list;
    instance_list.resize(get_instance_count(p_handle));
    FMOD_Studio_EventDescription_GetInstanceList((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle,
                                                 (FMOD_STUDIO_EVENTINSTANCE **)instance_list.ptrw(),
                                                 get_instance_count(p_handle), nullptr);
    return instance_list;
}
FMOD_RESULT FmodGodot::Studio::StudioEventDescription::load_sample_data(Handle p_handle)
{
    return FMOD_Studio_EventDescription_LoadSampleData((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle);
}
FMOD_RESULT FmodGodot::Studio::StudioEventDescription::unload_sample_data(Handle p_handle)
{
    return FMOD_Studio_EventDescription_UnloadSampleData((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle);
}
FMOD_STUDIO_LOADING_STATE FmodGodot::Studio::StudioEventDescription::get_sample_loading_state(Handle p_handle)
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_EventDescription_GetSampleLoadingState((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &state);
    return state;
}
FMOD_RESULT FmodGodot::Studio::StudioEventDescription::release_all_instances(Handle p_handle)
{
    return FMOD_Studio_EventDescription_ReleaseAllInstances((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle);
}
void *FmodGodot::Studio::StudioEventDescription::get_user_data(Handle p_handle)
{
    void *userdata;
    FMOD_Studio_EventDescription_GetUserData((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, &userdata);
    return userdata;
}
FMOD_RESULT FmodGodot::Studio::StudioEventDescription::set_user_data(Handle p_handle, void *p_userdata)
{
    return FMOD_Studio_EventDescription_SetUserData((FMOD_STUDIO_EVENTDESCRIPTION *)p_handle, p_userdata);
}
