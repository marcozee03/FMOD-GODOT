#include "bus.h"
#include "fmod_enums.h"
#include "fmod_globals.h"
#include "binding/conversions.h"
#include "fmod_studio.h"
#include "globals.h"
using namespace FmodGodot::Studio;
void FmodGodot::Studio::Bus::_bind_methods()
{
    BIND_METHOD(is_valid);
    BIND_METHOD(get_id);
    BIND_METHOD(get_path);
    BIND_METHOD(get_final_volume);
    BIND_PROPERTY(volume, Variant::FLOAT);
    BIND_PROPERTY(paused, Variant::BOOL);
    BIND_PROPERTY(mute, Variant::BOOL);
    BIND_PROPERTY(port_index, Variant::INT);
    BIND_METHOD(stop_all_events);
    BIND_METHOD(lock_channel_group);
    BIND_METHOD(unlock_channel_group);
}
FmodGodot::Studio::Bus::Bus(FMOD_STUDIO_BUS *bus) : handle(bus)
{
}
bool FmodGodot::Studio::Bus::is_valid() const
{
    return FMOD_Studio_Bus_IsValid(handle);
}
godot::Vector4i FmodGodot::Studio::Bus::get_id() const
{
    FMOD_GUID guid;
    FMOD_Studio_Bus_GetID(handle, &guid);
    return FmodGodot::cast_to_vector4i(guid);
}
godot::String FmodGodot::Studio::Bus::get_path() const
{
    FMOD_GET_STRING(FMOD_Studio_Bus_GetPath, handle, path);
    return path;
}
float FmodGodot::Studio::Bus::get_volume() const
{
    float volume;
    FMOD_Studio_Bus_GetVolume(handle, &volume, nullptr);
    return volume;
}
float FmodGodot::Studio::Bus::get_final_volume() const
{
    float finalVolume;
    FMOD_Studio_Bus_GetVolume(handle, nullptr, &finalVolume);
    return finalVolume;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_volume(float volume)
{
    return FMOD_Studio_Bus_SetVolume(handle, volume);
}
bool FmodGodot::Studio::Bus::get_paused() const
{
    FMOD_BOOL paused;
    FMOD_Studio_Bus_GetPaused(handle, &paused);
    return paused;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_paused(bool paused)
{
    return FMOD_Studio_Bus_SetPaused(handle, paused);
}
bool FmodGodot::Studio::Bus::get_mute() const
{
    FMOD_BOOL mute;
    FMOD_Studio_Bus_GetMute(handle, &mute);
    return mute;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_mute(bool mute)
{
    return FMOD_Studio_Bus_SetMute(handle, mute);
}
FMOD_RESULT FmodGodot::Studio::Bus::stop_all_events(FMOD_STUDIO_STOP_MODE mode)
{
    return FMOD_Studio_Bus_StopAllEvents(handle, mode);
}
uint64_t FmodGodot::Studio::Bus::get_port_index() const
{
    FMOD_PORT_INDEX index;
    FMOD_FAIL_IF_ERR(FMOD_Studio_Bus_GetPortIndex(handle, &index), index);
    return index;
}
FMOD_RESULT FmodGodot::Studio::Bus::set_port_index(uint64_t index)
{
    return FMOD_Studio_Bus_SetPortIndex(handle, index);
}
FMOD_RESULT FmodGodot::Studio::Bus::lock_channel_group()
{
    return FMOD_Studio_Bus_LockChannelGroup(handle);
}
FMOD_RESULT FmodGodot::Studio::Bus::unlock_channel_group()
{
    return FMOD_Studio_Bus_UnlockChannelGroup(handle);
}
unsigned int FmodGodot::Studio::Bus::getInclusiveCPUUsage() const
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage(handle, nullptr, &usage);
    return usage;
}
unsigned int FmodGodot::Studio::Bus::getExclusiveCPUUsage() const
{
    unsigned int usage;
    FMOD_Studio_Bus_GetCPUUsage(handle, &usage, nullptr);
    return usage;
}
