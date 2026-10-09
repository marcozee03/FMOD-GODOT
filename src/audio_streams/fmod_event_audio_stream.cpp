
#include "fmod_event_audio_stream.h"

#include "audio_streams/fmod_event_audio_stream_playback.h"
#include "classes/engine.hpp"
#include "event_description.h"
#include "fmod_audio_server.h"
#ifdef TOOLS_ENABLED
#include "fmod_editor_interface.h"
#endif
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include "parameter_cache.h"
#include "studio_system.h"
#include "variant/variant.hpp"
using namespace godot;
namespace FmodGodot
{
FMOD_STUDIO_EVENTDESCRIPTION *FmodEventAudioStream::_get_event_description() const
{
    return bit_cast<FMOD_STUDIO_EVENTDESCRIPTION *>(Studio::StudioSystem::get_event_by_id(
        bit_cast<Handle>(FmodAudioServer::get_singleton()->get_studio()), event_guid));
}
void FmodEventAudioStream::_bind_methods()
{
    BIND_PROPERTY_WITH_HINT(event_guid, Variant::VECTOR4I, PROPERTY_HINT_NONE, "FmodEvent");
}
void FmodEventAudioStream::set_event_guid(Vector4i p_event_guid)
{
    event_guid = p_event_guid;
    emit_signal("parameter_list_changed");
}
godot::Vector4i FmodEventAudioStream::get_event_guid() const
{
    return event_guid;
}

Ref<AudioStreamPlayback> FmodEventAudioStream::_instantiate_playback() const
{
    return memnew(FmodEventAudioStreamPlayback(event_guid));
}
double FmodGodot::FmodEventAudioStream::_get_length() const
{
    int length;
    FMOD_Studio_EventDescription_GetLength(_get_event_description(), &length);
    length = max(1, length);
    return length / 1000.0;
}
godot::TypedArray<Dictionary> FmodEventAudioStream::_get_parameter_list() const
{
    TypedArray<Dictionary> list;
    FMOD_STUDIO_EVENTDESCRIPTION *event_description = _get_event_description();
    if (!FMOD_Studio_EventDescription_IsValid(event_description))
    {
        if (!Engine::get_singleton()->is_editor_hint())
        {
            UtilityFunctions::push_warning("event description was not valid defaulting to cache");
        }
        else
        {
#ifdef TOOLS_ENABLED
            auto event = FmodEditorInterface::get_singleton()->get_cache()->get_event(event_guid);
            for (auto param : event.parameters)
            {
                if (param.type == FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
                {
                    Dictionary info = static_cast<Dictionary>(static_cast<PropertyInfo>(param));
                    info["name"] = param.name;
                    info["default_value"] = param.default_value;
                    list.push_back(info);
                }
            }
#endif
        }
        return list;
    }
    int count;
    FMOD_Studio_EventDescription_GetParameterDescriptionCount(event_description, &count);
    for (int parameter_index = 0; parameter_index < count; parameter_index++)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION param;
        FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(event_description, parameter_index, &param);
        if (param.type != FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
        {
            continue;
        }
        Dictionary info = static_cast<Dictionary>(ParameterCache::propInfo(param, event_description));
        info["name"] = param.name;
        info["default_value"] = param.defaultvalue;
        list.push_back(info);
    }
    return list;
}
} // namespace FmodGodot
