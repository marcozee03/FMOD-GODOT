#include "bus.h"
#include "binding/conversions.h"
#include "fmod_defs.h"
#include "fmod_enums.h"
#include "fmod_studio.h"
#include "globals.h"
#include <bit>
using namespace FmodGodot;
using namespace FmodGodot::Studio;
void FmodGodot::Studio::StudioBus::_bind_methods()
{
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_id, "handle");
    BIND_STATIC_METHOD(get_path, "handle");
    BIND_STATIC_METHOD(get_final_volume, "handle");
    BIND_STATIC_METHOD(get_volume, "handle");
    BIND_STATIC_METHOD(set_volume, "handle", "volume");
    BIND_STATIC_METHOD(get_paused, "handle");
    BIND_STATIC_METHOD(set_paused, "handle", "paused");
    BIND_STATIC_METHOD(get_mute, "handle");
    BIND_STATIC_METHOD(set_mute, "handle", "mute");
    BIND_STATIC_METHOD(get_port_index, "handle");
    BIND_STATIC_METHOD(set_port_index, "handle", "port_index");
    BIND_STATIC_METHOD(get_channel_group, "handle");
    BIND_STATIC_METHOD(stop_all_events, "handle", "stop_mode");
    BIND_STATIC_METHOD(lock_channel_group, "handle");
    BIND_STATIC_METHOD(unlock_channel_group, "handle");
}
bool FmodGodot::Studio::StudioBus::is_valid(Handle p_handle)
{
    return FMOD_Studio_Bus_IsValid(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle));
}
godot::Vector4i FmodGodot::Studio::StudioBus::get_id(Handle p_handle)
{
    FMOD_GUID guid;
    FMOD_Studio_Bus_GetID(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &guid);
    return FmodGodot::cast_to_vector4i(guid);
}
godot::String FmodGodot::Studio::StudioBus::get_path(Handle p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_Bus_GetPath, std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), path);
    return path;
}
float FmodGodot::Studio::StudioBus::get_volume(Handle p_handle)
{
    float volume;
    FMOD_Studio_Bus_GetVolume(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &volume, nullptr);
    return volume;
}
float FmodGodot::Studio::StudioBus::get_final_volume(Handle p_handle)
{
    float finalVolume;
    FMOD_Studio_Bus_GetVolume(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), nullptr, &finalVolume);
    return finalVolume;
}
FMOD_RESULT FmodGodot::Studio::StudioBus::set_volume(Handle p_handle, float p_volume)
{
    return FMOD_Studio_Bus_SetVolume(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), p_volume);
}
bool FmodGodot::Studio::StudioBus::get_paused(Handle p_handle)
{
    FMOD_BOOL paused;
    FMOD_Studio_Bus_GetPaused(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &paused);
    return paused;
}
FMOD_RESULT FmodGodot::Studio::StudioBus::set_paused(Handle p_handle, bool p_paused)
{
    return FMOD_Studio_Bus_SetPaused(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), p_paused);
}
bool FmodGodot::Studio::StudioBus::get_mute(Handle p_handle)
{
    FMOD_BOOL mute;
    FMOD_Studio_Bus_GetMute(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &mute);
    return mute;
}
FMOD_RESULT FmodGodot::Studio::StudioBus::set_mute(Handle p_handle, bool p_mute)
{
    return FMOD_Studio_Bus_SetMute(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), p_mute);
}
FMOD_RESULT FmodGodot::Studio::StudioBus::stop_all_events(Handle p_handle, FMOD_STUDIO_STOP_MODE p_mode)
{
    return FMOD_Studio_Bus_StopAllEvents(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), p_mode);
}
uint64_t FmodGodot::Studio::StudioBus::get_port_index(Handle p_handle)
{
    FMOD_PORT_INDEX index;
    FMOD_Studio_Bus_GetPortIndex(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &index);
    return index;
}
FMOD_RESULT FmodGodot::Studio::StudioBus::set_port_index(Handle p_handle, uint64_t p_index)
{
    return FMOD_Studio_Bus_SetPortIndex(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), p_index);
}
FMOD_RESULT FmodGodot::Studio::StudioBus::lock_channel_group(Handle p_handle)
{
    return FMOD_Studio_Bus_LockChannelGroup(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle));
}
FMOD_RESULT FmodGodot::Studio::StudioBus::unlock_channel_group(Handle p_handle)
{
    return FMOD_Studio_Bus_UnlockChannelGroup(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle));
}
Handle FmodGodot::Studio::StudioBus::get_channel_group(Handle p_handle)
{
    FMOD_CHANNELGROUP *group;
    FMOD_Studio_Bus_GetChannelGroup(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &group);
    return std::bit_cast<Handle>(group);
}

unsigned int FmodGodot::Studio::StudioBus::get_inclusive_c_p_u_usage(Handle p_handle)
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), nullptr, &usage);
    return usage;
}
unsigned int FmodGodot::Studio::StudioBus::get_exclusive_c_p_u_usage(Handle p_handle)
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage(std::bit_cast<FMOD_STUDIO_BUS *>(p_handle), &usage, nullptr);
    return usage;
}
