
#include "fmod_editor_interface.h"
#include "binding/conversions.h"
#include "core/memory.hpp"
#include "fmod_audio_server.h"
#include "fmod_script_client.h"
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
        UtilityFunctions::push_warning("Failed to build banks. Fmod Studio might not be open");
    }
    // OS::get_singleton()->execute(
    // ProjectSettings::get_singleton()->get_setting_with_override(FMOD_STUDIO_PATH),
    // {"-build", ProjectSettings::get_singleton()->get_setting_with_override(FMOD_PROJECT_PATH)});
}
void FmodEditorInterface::_bind_methods()
{
}
const FmodEditorIndex *FmodEditorInterface::get_cache() const
{
    return &cache;
}
FmodScriptClient *FmodEditorInterface::get_script_client() const
{
    return script;
}
void FmodEditorInterface::register_console(FmodConsole *p_console)
{
    console = p_console;
}
void FmodEditorInterface::register_debugger(Ref<FmodDebuggerPlugin> p_debugger)
{
    debugger = p_debugger;
}
bool FmodEditorInterface::get_mute_remote(bool p_muted) const
{
    return debugger->get_mute();
}

void FmodEditorInterface::set_mute_remote(bool p_muted)
{
    debugger->set_mute(p_muted);
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
    if (p_load_start_up_banks)
    {
        FmodAudioServer::get_singleton()->_load_start_up_banks();
    }
    else
    {
        FmodAudioServer::get_singleton()->_reload_start_up_banks();
    }
    int bank_count;
    FMOD_Studio_System_GetBankCount(FmodAudioServer::get_singleton()->get_studio(), &bank_count);
    if (bank_count <= 0)
    {
        return;
    }
    cache.refresh(FmodAudioServer::get_singleton()->get_studio());
    if (p_load_start_up_banks)
    {
        FmodAudioServer::get_singleton()->_load_start_up_banks();
    }
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
