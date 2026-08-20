#include "bus.h"
#include "binding/conversions.h"
#include "core/class_db.hpp"
#include "fmod_enums.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "globals.h"
using namespace FmodGodot::Studio;
void FmodGodot::Studio::Bus::_bind_methods()
{
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_id, "handle");
    BIND_STATIC_METHOD(get_path, "handle");
    BIND_STATIC_METHOD(get_final_volume, "handle");
    BIND_STATIC_METHOD(get_volume, "handle");
    BIND_STATIC_METHOD(set_volume, "handle");
    BIND_STATIC_METHOD(get_paused, "handle");
    BIND_STATIC_METHOD(set_paused, "handle", "paused");
    BIND_STATIC_METHOD(get_mute, "handle");
    BIND_STATIC_METHOD(set_mute, "handle", "mute");
    BIND_STATIC_METHOD(get_port_index, "handle");
    BIND_STATIC_METHOD(set_port_index, "handle", "port_index");
    BIND_STATIC_METHOD(stop_all_events, "handle");
    BIND_STATIC_METHOD(lock_channel_group, "handle");
    BIND_STATIC_METHOD(unlock_channel_group, "handle");
}
bool FmodGodot::Studio::Bus::is_valid(size_t p_handle)
{
    return FMOD_Studio_Bus_IsValid((FMOD_STUDIO_BUS *)p_handle);
}
godot::Vector4i FmodGodot::Studio::Bus::get_id(size_t p_handle)
{
    FMOD_GUID guid;
    FMOD_Studio_Bus_GetID((FMOD_STUDIO_BUS *)p_handle, &guid);
    return FmodGodot::cast_to_vector4i(guid);
}
godot::String FmodGodot::Studio::Bus::get_path(size_t p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_Bus_GetPath, (FMOD_STUDIO_BUS *)p_handle, path);
    return path;
}
float FmodGodot::Studio::Bus::get_volume(size_t p_handle)
{
    float volume;
    FMOD_Studio_Bus_GetVolume((FMOD_STUDIO_BUS *)p_handle, &volume, nullptr);
    return volume;
}
float FmodGodot::Studio::Bus::get_final_volume(size_t p_handle)
{
    float finalVolume;
    FMOD_Studio_Bus_GetVolume((FMOD_STUDIO_BUS *)p_handle, nullptr, &finalVolume);
    return finalVolume;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_volume(size_t p_handle, float p_volume)
{
    return FMOD_Studio_Bus_SetVolume((FMOD_STUDIO_BUS *)p_handle, p_volume);
}
bool FmodGodot::Studio::Bus::get_paused(size_t p_handle)
{
    FMOD_BOOL paused;
    FMOD_Studio_Bus_GetPaused((FMOD_STUDIO_BUS *)p_handle, &paused);
    return paused;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_paused(size_t p_handle, bool p_paused)
{
    return FMOD_Studio_Bus_SetPaused((FMOD_STUDIO_BUS *)p_handle, p_paused);
}
bool FmodGodot::Studio::Bus::get_mute(size_t p_handle)
{
    FMOD_BOOL mute;
    FMOD_Studio_Bus_GetMute((FMOD_STUDIO_BUS *)p_handle, &mute);
    return mute;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_mute(size_t p_handle, bool p_mute)
{
    return FMOD_Studio_Bus_SetMute((FMOD_STUDIO_BUS *)p_handle, p_mute);
}
FMOD_RESULT FmodGodot::Studio::Bus::stop_all_events(size_t p_handle, FMOD_STUDIO_STOP_MODE p_mode)
{
    return FMOD_Studio_Bus_StopAllEvents((FMOD_STUDIO_BUS *)p_handle, p_mode);
}
uint64_t FmodGodot::Studio::Bus::get_port_index(size_t p_handle)
{
    FMOD_PORT_INDEX index;
    FMOD_Studio_Bus_GetPortIndex((FMOD_STUDIO_BUS *)p_handle, &index);
    return index;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_port_index(size_t p_handle, uint64_t p_index)
{
    return FMOD_Studio_Bus_SetPortIndex((FMOD_STUDIO_BUS *)p_handle, p_index);
}
FMOD_RESULT FmodGodot::Studio::Bus::lock_channel_group(size_t p_handle)
{
    return FMOD_Studio_Bus_LockChannelGroup((FMOD_STUDIO_BUS *)p_handle);
}
FMOD_RESULT FmodGodot::Studio::Bus::unlock_channel_group(size_t p_handle)
{
    return FMOD_Studio_Bus_UnlockChannelGroup((FMOD_STUDIO_BUS *)p_handle);
}
unsigned int FmodGodot::Studio::Bus::getInclusiveCPUUsage(size_t p_handle)
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage((FMOD_STUDIO_BUS *)p_handle, nullptr, &usage);
    return usage;
}
unsigned int FmodGodot::Studio::Bus::getExclusiveCPUUsage(size_t p_handle)
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage((FMOD_STUDIO_BUS *)p_handle, &usage, nullptr);
    return usage;
}
