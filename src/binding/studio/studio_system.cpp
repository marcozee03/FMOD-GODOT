#include "studio_system.h"
#include "fmod_defs.h"
#include "fmod_enums.h"
#include "globals.h"
#include "variant/packed_float32_array.hpp"
namespace FmodGodot
{
namespace Studio
{

void StudioSystem::_bind_methods()
{
    BIND_STATIC_METHOD(create, "header_version");
    BIND_STATIC_METHOD(initialize, "handle", "max_channels", "studio_flags", "flags");
    BIND_STATIC_METHOD(release, "handle");
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(update, "handle");
    BIND_STATIC_METHOD(flush_commands, "handle");
    BIND_STATIC_METHOD(flush_sample_loading, "handle");
    BIND_STATIC_METHOD(get_event, "handle", "path");
    BIND_STATIC_METHOD(get_bus, "handle", "path");
    BIND_STATIC_METHOD(get_vca, "handle", "path");
    BIND_STATIC_METHOD(get_bank, "handle", "path");
    BIND_STATIC_METHOD(get_event_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_bus_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_vca_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_bank_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_parameter_label_by_name, "handle", "name", "label_index");
    BIND_STATIC_METHOD(get_parameter_label_by_id, "handle", "id", "label_index");
    //
    BIND_STATIC_METHOD(get_parameter_by_id, "handle", "id");
    BIND_STATIC_METHOD(get_final_parameter_by_id, "handle", "id");
    ClassDB ::bind_static_method(get_class_static(),
                                 D_METHOD("set_parameter_by_id", "handle", "id", "value", "ignore_seek_speed"),
                                 &self_type ::set_parameter_by_id, DEFVAL(false));
    ClassDB ::bind_static_method(
        get_class_static(), D_METHOD("set_parameter_by_id_with_label", "handle", "id", "label", "ignore_seek_speed"),
        &self_type ::set_parameter_by_id_with_label, DEFVAL(false));
    BIND_STATIC_METHOD(set_parameters_by_ids);
    // ClassDB ::bind_static_method(get_class_static(),
    //                              D_METHOD("set_parameters_by_ids", "handle", "ids", "values", "ignore_seek_speed"),
    //                              &self_type::set_parameters_by_ids, DEFVAL(false));
    BIND_STATIC_METHOD(lookup_id, "handle", "path");
    BIND_STATIC_METHOD(lookup_path, "handle", "id");
    //
    BIND_STATIC_METHOD(get_num_listeners, "handle");
    BIND_STATIC_METHOD(set_num_listeners, "handle", "num_listeners");
    BIND_STATIC_METHOD(get_listener_transform, "handle", "listener");
    BIND_STATIC_METHOD(get_listener_velocity, "handle", "listener");
    BIND_STATIC_METHOD(get_listener_attenuation_position, "handle", "listener");
    //
    BIND_STATIC_METHOD(set_listener_attributes, "handle", "listener", "transform", "velocity");
    BIND_STATIC_METHOD(set_listener_attributes_with_attenuation, "handle", "listener", "transform", "velocity",
                       "attenuation_position");
    BIND_STATIC_METHOD(load_bank_file, "handle", "filename", "flags");
    BIND_STATIC_METHOD(load_bank_memory, "handle", "buffer", "flags");
    BIND_STATIC_METHOD(unload_all, "handle");
    BIND_STATIC_METHOD(get_bank_count, "handle");
    BIND_STATIC_METHOD(get_bank_list, "handle");
    //
    // // TODO: Param Description functions;
    //
    BIND_STATIC_METHOD(start_command_capture, "handle", "filename", "flags");
    BIND_STATIC_METHOD(stop_command_capture, "handle");
    BIND_STATIC_METHOD(load_command_replay, "handle", "filename", "flags");
}
Handle StudioSystem::create(unsigned int p_headerversion)
{

    FMOD_STUDIO_SYSTEM *system;
    FMOD_Studio_System_Create(&system, FMOD_VERSION);
    return std::bit_cast<Handle>(system);
}
FMOD_RESULT StudioSystem::initialize(Handle p_handle, int p_maxchannels, FMOD_STUDIO_INITFLAGS p_studioflags,
                                     FMOD_INITFLAGS p_flags)
{
    return FMOD_Studio_System_Initialize(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), p_maxchannels, p_studioflags,
                                         p_flags, nullptr);
}
FMOD_RESULT StudioSystem::release(Handle p_handle)
{
    return FMOD_Studio_System_Release((FMOD_STUDIO_SYSTEM *)p_handle);
}
bool StudioSystem::is_valid(Handle p_handle)
{
    return FMOD_Studio_System_IsValid((FMOD_STUDIO_SYSTEM *)p_handle);
}
FMOD_RESULT StudioSystem::update(Handle p_handle)
{
    return FMOD_Studio_System_Update((FMOD_STUDIO_SYSTEM *)p_handle);
}
FMOD_RESULT StudioSystem::flush_commands(Handle p_handle)
{
    return FMOD_Studio_System_FlushCommands((FMOD_STUDIO_SYSTEM *)p_handle);
}
FMOD_RESULT StudioSystem::flush_sample_loading(Handle p_handle)
{
    return FMOD_Studio_System_FlushSampleLoading((FMOD_STUDIO_SYSTEM *)p_handle);
}
Handle StudioSystem::get_event(Handle p_handle, const String &p_path)
{
    FMOD_STUDIO_EVENTDESCRIPTION *event;
    FMOD_Studio_System_GetEvent((FMOD_STUDIO_SYSTEM *)p_handle, p_path.utf8().ptr(), &event);
    return (Handle)(event);
}
Handle StudioSystem::get_bus(Handle p_handle, const String &p_path)
{
    FMOD_STUDIO_BUS *bus;
    FMOD_Studio_System_GetBus((FMOD_STUDIO_SYSTEM *)p_handle, p_path.utf8().ptr(), &bus);
    return (Handle)bus;
}
Handle StudioSystem::get_vca(Handle p_handle, const String &p_path)
{
    FMOD_STUDIO_VCA *vca;
    FMOD_Studio_System_GetVCA((FMOD_STUDIO_SYSTEM *)p_handle, p_path.utf8().ptr(), &vca);
    return (Handle)vca;
}
Handle StudioSystem::get_bank(Handle p_handle, const String &p_path)
{
    FMOD_STUDIO_BANK *bank;
    FMOD_Studio_System_GetBank((FMOD_STUDIO_SYSTEM *)p_handle, p_path.utf8().ptr(), &bank);
    return (Handle)(bank);
}
Handle StudioSystem::get_event_by_id(Handle p_handle, const Vector4i &p_id)
{
    FMOD_STUDIO_EVENTDESCRIPTION *event_description;
    FMOD_Studio_System_GetEventByID((FMOD_STUDIO_SYSTEM *)p_handle, (FMOD_GUID *)&p_id, &event_description);
    return (Handle)event_description;
}
Handle StudioSystem::get_bus_by_id(Handle p_handle, const Vector4i &p_id)
{
    FMOD_STUDIO_BUS *bus;
    FMOD_Studio_System_GetBusByID((FMOD_STUDIO_SYSTEM *)p_handle, (FMOD_GUID *)&p_id, &bus);
    return (Handle)bus;
}
Handle StudioSystem::get_vca_by_id(Handle p_handle, const Vector4i &p_id)
{
    FMOD_STUDIO_VCA *vca;
    FMOD_Studio_System_GetVCAByID((FMOD_STUDIO_SYSTEM *)p_handle, (FMOD_GUID *)&p_id, &vca);
    return (Handle)vca;
}
Handle StudioSystem::get_bank_by_id(Handle p_handle, const Vector4i &p_id)
{
    FMOD_STUDIO_BANK *bank;
    FMOD_Studio_System_GetBankByID((FMOD_STUDIO_SYSTEM *)p_handle, (FMOD_GUID *)&p_id, &bank);
    return (Handle)bank;
}
godot::String StudioSystem::get_parameter_label_by_name(Handle p_handle, const String &p_name, int p_labelindex)
{
    FMOD_LOOKUP_STRING(FMOD_Studio_System_GetParameterLabelByName, (FMOD_STUDIO_SYSTEM *)p_handle, label,
                       p_name.utf8().ptr(), p_labelindex)
    return label;
}
godot::String StudioSystem::get_parameter_label_by_id(Handle p_handle, GD_PARAMETER_ID p_id, int p_labelindex)
{

    FMOD_LOOKUP_STRING(FMOD_Studio_System_GetParameterLabelByID, std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), label,
                       std::bit_cast<FMOD_STUDIO_PARAMETER_ID>(p_id), p_labelindex)
    return label;
}
float StudioSystem::get_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id)
{
    float value;
    FMOD_Studio_System_GetParameterByID((FMOD_STUDIO_SYSTEM *)p_handle, cast_to_parameter_id(p_id), &value, nullptr);
    return value;
}
float StudioSystem::get_final_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id)
{
    float value;
    FMOD_Studio_System_GetParameterByID((FMOD_STUDIO_SYSTEM *)p_handle, cast_to_parameter_id(p_id), nullptr, &value);
    return value;
}
FMOD_RESULT StudioSystem::set_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id, float p_value,
                                              bool p_ignoreseekspeed)
{
    return FMOD_Studio_System_SetParameterByID((FMOD_STUDIO_SYSTEM *)p_handle, cast_to_parameter_id(p_id), p_value,
                                               p_ignoreseekspeed);
}
FMOD_RESULT StudioSystem::set_parameter_by_id_with_label(Handle p_handle, GD_PARAMETER_ID p_id, const String &p_label,
                                                         bool p_ignoreseekspeed)
{
    return FMOD_Studio_System_SetParameterByIDWithLabel((FMOD_STUDIO_SYSTEM *)p_handle, cast_to_parameter_id(p_id),
                                                        p_label.utf8().ptr(), p_ignoreseekspeed);
}
FMOD_RESULT StudioSystem::set_parameters_by_ids(Handle p_handle, const PackedInt64Array &p_ids,
                                                PackedFloat32Array p_values, bool p_ignoreseekspeed)
{
    return FMOD_Studio_System_SetParametersByIDs(
        std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), std::bit_cast<FMOD_STUDIO_PARAMETER_ID *>(p_ids.ptr()),
        p_values.ptrw(), Math::min(p_ids.size(), p_values.size()), p_ignoreseekspeed);
}
float StudioSystem::get_parameter_by_name(Handle p_handle, const String &p_name)
{
    float value;
    FMOD_Studio_System_GetParameterByName((FMOD_STUDIO_SYSTEM *)p_handle, p_name.utf8().ptr(), &value, nullptr);
    return value;
}
float StudioSystem::get_final_parameter_by_name(Handle p_handle, const String &p_name)
{
    float value;
    FMOD_Studio_System_GetParameterByName((FMOD_STUDIO_SYSTEM *)p_handle, p_name.utf8().ptr(), nullptr, &value);
    return value;
}
FMOD_RESULT StudioSystem::set_parameter_by_name(Handle p_handle, const String &p_name, float p_value,
                                                bool p_ignoreseekspeed)
{
    return FMOD_Studio_System_SetParameterByName((FMOD_STUDIO_SYSTEM *)p_handle, p_name.utf8().ptr(), p_value,
                                                 p_ignoreseekspeed);
}
FMOD_RESULT StudioSystem::set_parameter_by_name_with_label(Handle p_handle, const String &p_name, const String &p_label,
                                                           bool p_ignoreseekspeed)
{
    return FMOD_Studio_System_SetParameterByNameWithLabel((FMOD_STUDIO_SYSTEM *)p_handle, p_name.utf8().ptr(),
                                                          p_label.utf8().ptr(), p_ignoreseekspeed);
}
godot::Vector4i StudioSystem::lookup_id(Handle p_handle, const String &p_path)
{
    FMOD_GUID guid;
    FMOD_Studio_System_LookupID((FMOD_STUDIO_SYSTEM *)p_handle, p_path.utf8().ptr(), &guid);
    return cast_to_vector4i(guid);
}
godot::String StudioSystem::lookup_path(Handle p_handle, const Vector4i &p_id)
{
    FMOD_LOOKUP_STRING(FMOD_Studio_System_LookupPath, (FMOD_STUDIO_SYSTEM *)p_handle, path, (FMOD_GUID *)&p_id)
    return path;
}
int StudioSystem::get_num_listeners(Handle p_handle)
{
    int num_listeners;
    FMOD_Studio_System_GetNumListeners((FMOD_STUDIO_SYSTEM *)p_handle, &num_listeners);
    return num_listeners;
}
FMOD_RESULT StudioSystem::set_num_listeners(Handle p_handle, int p_numlisteners)
{
    return FMOD_Studio_System_SetNumListeners((FMOD_STUDIO_SYSTEM *)p_handle, p_numlisteners);
}
godot::Transform3D StudioSystem::get_listener_transform(Handle p_handle, int p_listener)
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_System_GetListenerAttributes((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &attr, nullptr);
    return to_transform3d(attr);
}
godot::Vector3 StudioSystem::get_listener_velocity(Handle p_handle, int p_listener)
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_Studio_System_GetListenerAttributes((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &attr, nullptr);
    return to_godot_vector(attr.velocity);
}
godot::Vector3 StudioSystem::get_listener_attenuation_position(Handle p_handle, int p_listener)
{
    FMOD_3D_ATTRIBUTES attr;
    FMOD_VECTOR attenuation;
    FMOD_Studio_System_GetListenerAttributes((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &attr, &attenuation);
    return to_godot_vector(attenuation);
}
FMOD_RESULT StudioSystem::set_listener_attributes(Handle p_handle, int p_listener, const Transform3D &p_transform,
                                                  const Vector3 &p_velocity)
{
    FMOD_3D_ATTRIBUTES attr = to_3d_attributes(p_transform);
    attr.velocity = to_fmod_vector(p_velocity);
    return FMOD_Studio_System_SetListenerAttributes((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &attr, nullptr);
}
FMOD_RESULT StudioSystem::set_listener_attributes_with_attenuation(Handle p_handle, int p_listener,
                                                                   const Transform3D &p_transform,
                                                                   const Vector3 &p_velocity,
                                                                   const Vector3 &p_attenuationposition)
{
    FMOD_3D_ATTRIBUTES attr = to_3d_attributes(p_transform);
    attr.velocity = to_fmod_vector(p_velocity);
    return FMOD_Studio_System_SetListenerAttributes((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &attr,
                                                    (FMOD_VECTOR *)&p_attenuationposition);
}
float StudioSystem::get_listener_weight(Handle p_handle, int p_listener)
{
    float weight;
    FMOD_Studio_System_GetListenerWeight((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, &weight);
    return weight;
}
FMOD_RESULT StudioSystem::set_listener_weight(Handle p_handle, int p_listener, float p_weight)
{
    return FMOD_Studio_System_SetListenerWeight((FMOD_STUDIO_SYSTEM *)p_handle, p_listener, p_weight);
}
Handle StudioSystem::load_bank_file(Handle p_handle, const String &p_filename, FMOD_STUDIO_LOAD_BANK_FLAGS p_flags)
{
    FMOD_STUDIO_BANK *bank;
    if (FMOD_Studio_System_LoadBankFile((FMOD_STUDIO_SYSTEM *)p_handle, p_filename.utf8().ptr(), p_flags, &bank) ==
        FMOD_OK)
    {
        return (Handle)bank;
    }
    return 0;
}
Handle StudioSystem::load_bank_memory(Handle p_handle, const PackedByteArray &p_buffer,
                                      FMOD_STUDIO_LOAD_MEMORY_MODE p_mode, FMOD_STUDIO_LOAD_BANK_FLAGS p_flags)
{
    FMOD_STUDIO_BANK *bank;
    if (FMOD_OK == FMOD_Studio_System_LoadBankMemory((FMOD_STUDIO_SYSTEM *)p_handle, (const char *)p_buffer.ptr(),
                                                     p_buffer.size(), p_mode, p_flags, &bank))
    {
        return (Handle)bank;
    }
    return 0;
}
FMOD_RESULT StudioSystem::unload_all(Handle p_handle)
{
    return FMOD_Studio_System_UnloadAll((FMOD_STUDIO_SYSTEM *)p_handle);
}
int StudioSystem::get_bank_count(Handle p_handle)
{
    int count;
    FMOD_Studio_System_GetBankCount(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), &count);
    return count;
}
godot::PackedInt64Array StudioSystem::get_bank_list(Handle p_handle)
{
    PackedInt64Array banks;
    banks.resize(get_bank_count(p_handle));
    if (banks.size() == 0)
    {
        return banks;
    }
    FMOD_Studio_System_GetBankList(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle),
                                   std::bit_cast<FMOD_STUDIO_BANK **>(banks.ptr()), banks.size(), nullptr);
    return banks;
}
int StudioSystem::get_parameter_description_count(Handle p_handle)
{
    int count;
    FMOD_Studio_System_GetParameterDescriptionCount(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), &count);
    return count;
}
godot::LocalVector<FMOD_STUDIO_PARAMETER_DESCRIPTION> StudioSystem::get_parameter_description_list(Handle p_handle)
{
    LocalVector<FMOD_STUDIO_PARAMETER_DESCRIPTION> parameters;
    parameters.resize(get_parameter_description_count(p_handle));
    if (parameters.size() == 0)
    {
        return parameters;
    }
    FMOD_Studio_System_GetParameterDescriptionList(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), parameters.ptr(),
                                                   parameters.size(), nullptr);
    return parameters;
}
FMOD_RESULT StudioSystem::start_command_capture(Handle p_handle, const String &p_filename,
                                                FMOD_STUDIO_COMMANDCAPTURE_FLAGS p_flags)
{
    return FMOD_Studio_System_StartCommandCapture(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle),
                                                  p_filename.utf8().ptr(), p_flags);
}
FMOD_RESULT StudioSystem::stop_command_capture(Handle p_handle)
{
    return FMOD_Studio_System_StopCommandCapture(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle));
}
Handle StudioSystem::load_command_replay(Handle p_handle, const String &p_filename,
                                         FMOD_STUDIO_COMMANDREPLAY_FLAGS p_flags)
{
    FMOD_STUDIO_COMMANDREPLAY *replay;
    FMOD_Studio_System_LoadCommandReplay(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), p_filename.utf8().ptr(),
                                         p_flags, &replay);
    return std::bit_cast<Handle>(replay);
}
void *StudioSystem::getUserData(Handle p_handle)
{
    void *userdata;
    FMOD_Studio_System_GetUserData((FMOD_STUDIO_SYSTEM *)p_handle, &userdata);
    return userdata;
}
FMOD_RESULT StudioSystem::setUserData(Handle p_handle, void *p_userdata)
{
    return FMOD_Studio_System_SetUserData(std::bit_cast<FMOD_STUDIO_SYSTEM *>(p_handle), p_userdata);
}

} // namespace Studio
} // namespace FmodGodot
