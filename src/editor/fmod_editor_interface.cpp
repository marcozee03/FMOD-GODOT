
#include "core/memory.hpp"
#include "fmod_globals.h"
#include "fmod_script_client.h"
#include "fmod_studio_common.h"
#include "variant/utility_functions.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_audio_server.h"
#include "fmod_editor_interface.h"
#include <classes/os.hpp>
#include <classes/project_settings.hpp>
namespace FmodGodot
{
FmodEditorInterface *FmodEditorInterface::singleton = nullptr;
FmodEditorInterface::FmodEditorInterface()
{
    if (singleton == nullptr)
    {
        singleton = this;
    }
    script = memnew(FmodScriptClient);
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
    FmodAudioServer::get_singleton()->finish();
    FmodAudioServer::get_singleton()->init_with_project_settings();
    FmodAudioServer::get_singleton()->load_start_up_banks();
}
void FmodEditorInterface::build_banks()
{
    if (!script->send_script_command("studio.project.build()"))
    {
        UtilityFunctions::push_warning("Failed to build banks. Fmod Studio may not be open");
    }
    // OS::get_singleton()->execute(
    // ProjectSettings::get_singleton()->get_setting_with_override(FMOD_STUDIO_PATH),
    // {"-build", ProjectSettings::get_singleton()->get_setting_with_override(FMOD_PROJECT_PATH)});
}
void FmodEditorInterface::_bind_methods()
{
}
const FmodEditorCache *FmodEditorInterface::get_cache() const
{
    return &cache;
}
FmodScriptClient *FmodEditorInterface::get_script_client() const
{
    return script;
}
void FmodEditorInterface::set_console(FmodConsole *p_console)
{
    console = p_console;
};
void FmodEditorInterface::print(const String &message)
{
    console->add_message(message);
}
void FmodEditorInterface::print_warning(const String &message)
{
    console->add_message(message, FmodConsole::MSG_TYPE_WARNING);
}
void FmodEditorInterface::print_error(const String &message)
{
    console->add_message(message, FmodConsole::MSG_TYPE_ERROR);
}
void FmodEditorInterface::print_rich(const String &message)
{
    console->add_message(message, FmodConsole::MSG_TYPE_STD_RICH);
}
const FmodTheme *FmodEditorInterface::get_theme() const
{
    return &theme;
}
FmodEditorInterface *FmodEditorInterface::get_singleton()
{
    return singleton;
}
namespace
{
Parameter to_cacheable_parameter(FMOD_STUDIO_PARAMETER_DESCRIPTION *description)
{
    Parameter parameter;
    parameter.full_path = String("param:/") + description->name;
    parameter.guid = cast_to_vector4i(description->guid);
    parameter.default_value = description->defaultvalue;
    parameter.discrete = description->flags & FMOD_STUDIO_PARAMETER_DISCRETE;
    parameter.min_value = description->minimum;
    parameter.max_value = description->maximum;
    return parameter;
}
Event to_cacheable_event(FMOD_STUDIO_EVENTDESCRIPTION *description)
{
    int size = 64;
    int retrieved = 0;
    char *str = memnew_arr(char, size);

    FMOD_GET_FULL_STRING(FMOD_Studio_EventDescription_GetPath, description, str, size, retrieved);
    Event event;
    event.full_path = str;
    FMOD_GUID guid;
    FMOD_Studio_EventDescription_GetID(description, &guid);
    event.guid = cast_to_vector4i(guid);
    FMOD_BOOL is3D, isDopplerEnabled, isStream, isOneShot;
    FMOD_Studio_EventDescription_Is3D(description, &is3D);
    FMOD_Studio_EventDescription_IsDopplerEnabled(description, &isDopplerEnabled);
    FMOD_Studio_EventDescription_IsOneshot(description, &isOneShot);
    FMOD_Studio_EventDescription_IsStream(description, &isStream);
    FMOD_Studio_EventDescription_GetLength(description, &event.lengthMS);

    FMOD_Studio_EventDescription_GetMinMaxDistance(description, &event.min, &event.max);
    event.is3d = is3D;
    event.doppler_enabled = isDopplerEnabled;
    event.stream = isStream;
    event.one_shot = isOneShot;
    int parameter_count;
    FMOD_Studio_EventDescription_GetParameterDescriptionCount(description, &parameter_count);
    for (int k = 0; k < parameter_count; k++)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION parameter_description;
        FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(description, k, &parameter_description);
        Parameter parameter = to_cacheable_parameter(&parameter_description);
        event.parameters.append(parameter);
    }
    memdelete_arr(str);
    return event;
}
} // namespace
void FmodEditorInterface::refresh()
{
    FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_singleton()->get_studio();
    if (!studio)
    {
        return;
    }
    int bank_count;
    FMOD_Studio_System_GetBankCount(studio, &bank_count);
    if (bank_count < 0)
    {
        return;
    }
    FMOD_STUDIO_BANK **banks = memnew_arr(FMOD_STUDIO_BANK *, bank_count);
    FMOD_Studio_System_GetBankList(studio, banks, bank_count, &bank_count);
    int size = 64;
    int retrieved = 0;
    char *str = memnew_arr(char, size);
    for (int i = 0; i < bank_count; i++)
    {

        FMOD_GET_FULL_STRING(FMOD_Studio_Bank_GetPath, banks[i], str, size, retrieved);
        FMOD_GUID guid;
        FMOD_Studio_Bank_GetID(banks[i], &guid);
        Bank bank;
        bank.full_path = str;
        bank.guid = cast_to_vector4i(guid);
        {
            int event_count;
            FMOD_Studio_Bank_GetEventCount(banks[i], &event_count);
            if (event_count > 0)
            {
                FMOD_STUDIO_EVENTDESCRIPTION **descriptions = memnew_arr(FMOD_STUDIO_EVENTDESCRIPTION *, event_count);
                FMOD_Studio_Bank_GetEventList(banks[i], descriptions, event_count, &event_count);
                for (int j = 0; j < event_count; j++)
                {
                    Event event = to_cacheable_event(descriptions[j]);
                    bank.children.push_back(event.full_path);
                    cache.add(event);
                }
                memdelete_arr(descriptions);
            }
        }
        {
            int vca_count;
            FMOD_Studio_Bank_GetVCACount(banks[i], &vca_count);
            if (vca_count > 0)
            {
                FMOD_STUDIO_VCA **vcas = memnew_arr(FMOD_STUDIO_VCA *, vca_count);
                FMOD_Studio_Bank_GetVCAList(banks[i], vcas, vca_count, &vca_count);
                for (int j = 0; j < vca_count; j++)
                {
                    VCA vca;
                    FMOD_GET_FULL_STRING(FMOD_Studio_VCA_GetPath, vcas[j], str, size, retrieved);
                    bank.children.push_back(str);
                    vca.full_path = str;
                    FMOD_Studio_VCA_GetID(vcas[i], &guid);
                    vca.guid = cast_to_vector4i(guid);
                    cache.add(vca);
                }
                memdelete_arr(vcas);
            }
        }
        cache.add(bank);
    }

    int p_count;
    FMOD_Studio_System_GetParameterDescriptionCount(studio, &p_count);
    if (p_count > 0)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION *params = memnew_arr(FMOD_STUDIO_PARAMETER_DESCRIPTION, p_count);
        FMOD_Studio_System_GetParameterDescriptionList(studio, params, p_count, &p_count);
        for (int i = 0; i < p_count; i++)
        {
            cache.add(to_cacheable_parameter(&params[i]));
        }
        memdelete_arr(params);
    }

    memdelete_arr(banks);
    memdelete_arr(str);
}
void FmodEditorInterface::show_event_in_fmod_studio(Vector4i guid)
{
    String cmd = "studio.window.navigateTo(studio.project.lookup(\"" + fmod_guid_to_string(guid) + "\"));";
    if (!script->send_script_command(cmd))
    {
        UtilityFunctions::print_rich("[color=grey] Command: \"" + cmd + "\" failed[/color]");
    }
}
} // namespace FmodGodot
#endif
