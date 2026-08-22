
#include "fmod_editor_interface.h"
#include "binding/conversions.h"
#include "binding/studio/event_description.h"
#include "binding/studio/parameter_cache.h"
#include "binding/studio/vca.h"
#include "core/memory.hpp"
#include "fmod_audio_server.h"
#include "fmod_bank.h"
#include "fmod_script_client.h"
#include "fmod_studio_common.h"
#include "variant/utility_functions.hpp"
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
void FmodEditorInterface::print(const String &p_message)
{
    console->add_message(p_message);
}
void FmodEditorInterface::print_warning(const String &p_message)
{
    console->add_message(p_message, FmodConsole::MSG_TYPE_WARNING);
}
void FmodEditorInterface::print_error(const String &p_message)
{
    console->add_message(p_message, FmodConsole::MSG_TYPE_ERROR);
}
void FmodEditorInterface::print_rich(const String &p_message)
{
    console->add_message(p_message, FmodConsole::MSG_TYPE_STD_RICH);
}
FmodEditorInterface *FmodEditorInterface::get_singleton()
{
    return singleton;
}
// namespace
void FmodEditorInterface::refresh(bool p_load_start_up_banks)
{
    FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_singleton()->get_studio();
    if (!studio)
    {
        return;
    }
    if (p_load_start_up_banks)
    {
        FmodAudioServer::get_singleton()->load_start_up_banks();
    }
    else
    {
        FmodAudioServer::get_singleton()->reload_start_up_banks();
    }
    int bank_count;
    FMOD_Studio_System_GetBankCount(studio, &bank_count);
    if (bank_count < 0)
    {
        FmodAudioServer::get_singleton()->unload_start_up_banks();
        return;
    }
    FMOD_STUDIO_BANK **banks = memnew_arr(FMOD_STUDIO_BANK *, bank_count);
    FMOD_Studio_System_GetBankList(studio, banks, bank_count, &bank_count);
    int size = 64;
    int retrieved = 0;
    char *str = memnew_arr(char, size);
    for (int i = 0; i < bank_count; i++)
    {

        Studio::StudioBank::Cache bank(banks[i]);
        cache.add(bank);
        {
            int event_count;
            FMOD_Studio_Bank_GetEventCount(banks[i], &event_count);
            if (event_count > 0)
            {
                FMOD_STUDIO_EVENTDESCRIPTION **descriptions = memnew_arr(FMOD_STUDIO_EVENTDESCRIPTION *, event_count);
                FMOD_Studio_Bank_GetEventList(banks[i], descriptions, event_count, &event_count);
                for (int j = 0; j < event_count; j++)
                {
                    FmodGodot::Studio::StudioEventDescription::Cache event(descriptions[j]);
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
                    FmodGodot::Studio::StudioVCA::Cache vca(vcas[j]);
                    cache.add(vca);
                }
                memdelete_arr(vcas);
            }
        }
    }

    int p_count;
    FMOD_Studio_System_GetParameterDescriptionCount(studio, &p_count);
    if (p_count > 0)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION *params = memnew_arr(FMOD_STUDIO_PARAMETER_DESCRIPTION, p_count);
        FMOD_Studio_System_GetParameterDescriptionList(studio, params, p_count, &p_count);
        for (int i = 0; i < p_count; i++)
        {
            cache.add(ParameterCache(params[i]));
        }
        memdelete_arr(params);
    }

    memdelete_arr(banks);
    memdelete_arr(str);

    FmodAudioServer::get_singleton()->unload_start_up_banks();
}
void FmodEditorInterface::show_event_in_fmod_studio(Vector4i p_guid)
{
    String cmd = "studio.window.navigateTo(studio.project.lookup(\"" + fmod_guid_to_string(p_guid) + "\"));";
    if (!script->send_script_command(cmd))
    {
        UtilityFunctions::print_rich("[color=grey] Command: \"" + cmd + "\" failed[/color]");
    }
}
} // namespace FmodGodot
