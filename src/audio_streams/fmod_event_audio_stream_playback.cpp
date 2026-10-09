#include "fmod_event_audio_stream_playback.h"
#include "event_instance.h"
#include "fmod_audio_server.h"
#include "fmod_studio.h"
#include <bit>
void FmodGodot::FmodEventAudioStreamPlayback::_bind_methods()
{
}
FmodGodot::FmodEventAudioStreamPlayback::FmodEventAudioStreamPlayback()
{
}
FmodGodot::FmodEventAudioStreamPlayback::FmodEventAudioStreamPlayback(Vector4i p_guid)
{
    instance = FmodAudioServer::get_singleton()->create_instance(p_guid);
}
FmodGodot::FmodEventAudioStreamPlayback::~FmodEventAudioStreamPlayback()
{
    FMOD_Studio_EventInstance_Release(instance);
}
void FmodGodot::FmodEventAudioStreamPlayback::_start(double p_from_pos)
{
    FMOD_Studio_EventInstance_Start(instance);
    FMOD_Studio_EventInstance_SetTimelinePosition(instance, p_from_pos * 1000);
}
void FmodGodot::FmodEventAudioStreamPlayback::_stop()
{
    FMOD_Studio_EventInstance_Stop(instance, FMOD_STUDIO_STOP_ALLOWFADEOUT);
}
bool FmodGodot::FmodEventAudioStreamPlayback::_is_playing() const
{
    FMOD_STUDIO_PLAYBACK_STATE state;
    FMOD_Studio_EventInstance_GetPlaybackState(instance, &state);
    return state == FMOD_STUDIO_PLAYBACK_PLAYING;
}
double FmodGodot::FmodEventAudioStreamPlayback::_get_playback_position() const
{
    int position;
    FMOD_Studio_EventInstance_GetTimelinePosition(instance, &position);
    return Math::max(0.001, position / 1000.0);
}
void FmodGodot::FmodEventAudioStreamPlayback::_seek(double p_position)
{
    FMOD_Studio_EventInstance_SetTimelinePosition(instance, p_position * 1000);
}
void FmodGodot::FmodEventAudioStreamPlayback::_set_parameter(const StringName &p_name, const Variant &p_value)
{
    Studio::StudioEventInstance::set_parameter_by_name(std::bit_cast<Handle>(instance), p_name, p_value);
}
godot::Variant FmodGodot::FmodEventAudioStreamPlayback::_get_parameter(const StringName &p_name) const
{
    return Studio::StudioEventInstance::get_parameter_by_name(std::bit_cast<Handle>(instance), p_name);
}
