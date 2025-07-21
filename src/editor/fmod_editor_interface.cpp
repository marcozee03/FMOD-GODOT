
#ifdef TOOLS_ENABLED
#include "fmod_editor_interface.h"
#include "fmod_audio_server.h"
#include <classes/os.hpp>
#include <classes/project_settings.hpp>
#include "fmod_string_names.h"
namespace FmodGodot
{
    FmodEditorInterface *FmodEditorInterface::singleton = nullptr;
    FmodEditorInterface::FmodEditorInterface()
    {
        if (singleton == nullptr)
        {
            singleton = this;
        }
    }
    FmodEditorInterface::~FmodEditorInterface()
    {
        if (singleton == this)
        {
            singleton = nullptr;
        }
    }
    void FmodEditorInterface::restart_server()
    {
        FmodAudioServer::get_singleton()->finish();\
        FmodAudioServer::get_singleton()->init_with_project_settings();
        FmodAudioServer::get_singleton()->load_start_up_banks();
    }
    void FmodEditorInterface::build_banks()
    {
        //TODO fix don't want it to open fmod studio (will also halt godot till it is closed)
        OS::get_singleton()->execute(ProjectSettings::get_singleton()->get_setting_with_override(FMOD_STUDIO_PATH), {"-build", ProjectSettings::get_singleton()->get_setting_with_override(FMOD_PROJECT_PATH)});
    }
    void FmodEditorInterface::_bind_methods() {}
    const FmodEditorCache *FmodEditorInterface::get_cache() const
    {
        return &cache;
    }
    const FmodTheme *FmodEditorInterface::get_theme() const
    {
        return &theme;
    }
    FmodEditorInterface *FmodEditorInterface::get_singleton()
    {
        return singleton;
    }
    void FmodEditorInterface::refresh()
    {
        FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_singleton()->get_studio();
        if (!studio)
        {
            return;
        }
        int bank_count;
        FMOD_Studio_System_GetBankCount(studio, &bank_count);
        FMOD_STUDIO_BANK **banks = new FMOD_STUDIO_BANK *[bank_count];
        FMOD_Studio_System_GetBankList(studio, banks, bank_count, &bank_count);
        int size = 64;
        int retrieved = 0;
        char *str = new char[size];
        for (int i = 0; i < bank_count; i++)
        {

            FMOD_GET_FULL_STRING(FMOD_Studio_Bank_GetPath, banks[i], str, size, retrieved);
            FMOD_GUID guid;
            FMOD_Studio_Bank_GetID(banks[i], &guid);
            Bank bank;
            bank.full_path = str;
            bank.guid = cast_to_Vector4i(guid);
            int event_count;
            FMOD_Studio_Bank_GetEventCount(banks[i], &event_count);
            FMOD_STUDIO_EVENTDESCRIPTION **descriptions = new FMOD_STUDIO_EVENTDESCRIPTION *[event_count];
            FMOD_Studio_Bank_GetEventList(banks[i], descriptions, event_count, &event_count);
            for (int j = 0; j < event_count; j++)
            {

                FMOD_GET_FULL_STRING(FMOD_Studio_EventDescription_GetPath, descriptions[j], str, size, retrieved);
                bank.events.push_back(str);
                Event event;
                event.full_path = str;
                FMOD_Studio_EventDescription_GetID(descriptions[j], &guid);
                event.guid = cast_to_Vector4i(guid);
                FMOD_BOOL is3D, isDopplerEnabled, isStream, isOneShot;
                int length;
                FMOD_Studio_EventDescription_Is3D(descriptions[j], &is3D);
                FMOD_Studio_EventDescription_IsDopplerEnabled(descriptions[j], &isDopplerEnabled);
                FMOD_Studio_EventDescription_IsOneshot(descriptions[j], &isOneShot);
                FMOD_Studio_EventDescription_IsStream(descriptions[j], &isStream);
                FMOD_Studio_EventDescription_GetLength(descriptions[j], &event.lengthMS);

                FMOD_Studio_EventDescription_GetMinMaxDistance(descriptions[j], &event.min, &event.max);
                event.is3d = is3D;
                event.doppler_enabled = isDopplerEnabled;
                event.stream = isStream;
                event.one_shot = isOneShot;
                int parameter_count;
                FMOD_Studio_EventDescription_GetParameterDescriptionCount(descriptions[j], &parameter_count);
                for (int k = 0; k < parameter_count; k++)
                {
                    FMOD_STUDIO_PARAMETER_DESCRIPTION parameter_description;
                    FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(descriptions[j], k, &parameter_description);
                    Parameter parameter;
                    parameter.full_path = parameter_description.name;
                    parameter.guid = cast_to_Vector4i(parameter_description.guid);
                    parameter.default_value = parameter_description.defaultvalue;
                    parameter.discrete = parameter_description.flags & FMOD_STUDIO_PARAMETER_DISCRETE;
                    parameter.min_value = parameter_description.minimum;
                    parameter.max_value = parameter_description.maximum;
                    event.parameters.append(parameter);
                }
                cache.add(event);
            }
            cache.add(bank);
            delete[] descriptions;
        }
        delete[] banks;
        delete[] str;
    }
}
#endif
