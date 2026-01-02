#include "fmod_event_audio_stream.h"
#include "fmod_editor_interface.h"
#include "fmod_event_audio_stream_playback.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "globals.h"
#include "variant/typed_array.hpp"
#include <classes/engine.hpp>
using namespace godot;
int FmodEventAudioStream::_get_bar_beats() const
{
    return 0;
}
int FmodEventAudioStream::_get_beat_count() const
{
    return 0;
}
double FmodEventAudioStream::_get_bpm() const
{
    return 0;
}
double FmodEventAudioStream::_get_length() const
{
    int lengthMS;
    FMOD_Studio_EventDescription_GetLength(description, &lengthMS);
    return lengthMS / 1000.0;
}
TypedArray<Dictionary> FmodEventAudioStream::_get_parameter_list() const
{
    TypedArray<Dictionary> list;
    int count;
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint())
    {
        auto EI = FmodGodot::FmodEditorInterface::get_singleton();
        auto cache = EI->get_cache();
        auto event_cache = cache->get_event_cache();
    }
#endif
    FMOD_Studio_EventDescription_GetParameterDescriptionCount(description, &count);
    for (int i = 0; i < count; i++)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION param;
        FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(description, i, &param);
        if (param.type != FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
        {
            continue;
        }
        Dictionary info;
        info["name"] = String(param.name);
        info["hint"] = PROPERTY_HINT_RANGE;
        info["type"] = Variant::FLOAT;
        info["usage"] = PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;
        info["default_value"] = param.defaultvalue;
        if (param.flags & FMOD_STUDIO_PARAMETER_DISCRETE)
        {
            info["hint_string"] = String(",").join({Variant(param.minimum), Variant(param.maximum), Variant(1)});
        }
        else
        {
            info["hint_string"] = String(",").join({Variant(param.minimum), Variant(param.maximum)});
        }
        list.push_back(info);
    }
    return list;
}
String FmodEventAudioStream::_get_stream_name() const
{

    int size;
    char *str = new char[size];
    int retrieved;
    FMOD_GET_FULL_STRING(FMOD_Studio_EventDescription_GetPath, description, str, size, retrieved);
    String name = str;
    delete[] str;
    return name;
}
bool FmodEventAudioStream::_has_loop() const
{
    return false;
}
Ref<AudioStreamPlayback> FmodEventAudioStream::_instantiate_playback() const
{
    return memnew(FmodEventAudioStreamPlayback(description));
}
bool FmodEventAudioStream::_is_monophonic() const
{
    return false;
}
Vector4i FmodEventAudioStream::get_event_id() const
{
    return event_id;
}
void FmodEventAudioStream::set_event_id(Vector4i p_id)
{
    if (event_id == p_id)
    {
        return;
    }
    event_id = p_id;
    emit_signal("parameter_list_changed");
}
void FmodEventAudioStream::_bind_methods()
{
    BIND_PROPERTY_WITH_HINT(event_id, Variant::VECTOR4I, PROPERTY_HINT_NONE, "FmodEvent");
}
FmodEventAudioStream::FmodEventAudioStream()
{
}
FmodEventAudioStream::~FmodEventAudioStream()
{
}
