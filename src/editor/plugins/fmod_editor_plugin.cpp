#include "fmod_editor_plugin.h"
#include "classes/theme.hpp"
#include "editor/fmod_installer.h"
#include "fmod_bank_importer.h"
#include "fmod_editor_interface.h"
#include "variant/callable_method_pointer.hpp"
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
using namespace godot;
namespace FmodGodot
{
void FmodEditorPlugin::_open_installer()
{
    if (installer == nullptr || !UtilityFunctions::is_instance_id_valid(installer_id) ||
        installer->is_queued_for_deletion())
    {
        installer = memnew(FmodInstaller);
        installer_id = installer->get_instance_id();
        get_editor_interface()->get_base_control()->add_child(installer);
    }
    else
    {
        installer->popup();
    }
}
void FmodEditorPlugin::_bind_methods()
{
}

String FmodEditorPlugin::_get_plugin_name() const
{
    return "FMOD";
}

Ref<Texture2D> FmodEditorPlugin::_get_plugin_icon() const
{
    return ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/fmod_icon.svg");
}

FmodEditorPlugin::FmodEditorPlugin()
{
}
FmodEditorPlugin::~FmodEditorPlugin()
{
}

void FmodEditorPlugin::_enter_tree()
{
    Ref<Theme> editor_theme = get_editor_interface()->get_editor_theme();
    editor_theme->set_icon("FmodBank", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_icon.svg"));
    editor_theme->set_icon("FmodEvent", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg"));
    editor_theme->set_icon("FmodContinuousParameter", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/c_parameter_icon.svg"));
    editor_theme->set_icon("FmodDiscreteParameter", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/d_parameter_icon.svg"));
    editor_theme->set_icon("FmodBankLoaderIcon", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_loader_icon.svg"));
    editor_theme->set_icon("FmodSnapshot", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/snapshot_icon.svg"));
    editor_theme->set_icon("FmodVCA", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/vca_icon.svg"));
    editor_theme->set_icon("Fmod", "EditorIcons",
                           ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/fmod_icon.svg"));
    log = memnew(FmodConsole);
    FmodEditorInterface::get_singleton()->set_console(log);
    eventInspector = memnew(EventInspector);
    add_inspector_plugin(eventInspector);
    bankInspector = memnew(BankInspectorPlugin);
    add_inspector_plugin(bankInspector);
    bankImporter = memnew(FmodBankImporter);
    add_import_plugin(bankImporter);
    browser = memnew(FmodEventBrowser());
    add_dock(log);
    add_dock(browser);

    log->_set_fmod_script_client(FmodEditorInterface::get_singleton()->get_script_client());
    add_tool_menu_item("Finish FMOD Godot setup", callable_mp(this, &FmodEditorPlugin::_open_installer));
    export_plugin = memnew(FmodExportPlugin);
    add_export_plugin(export_plugin);
}
#define memdelete_notnull(m_v)                                                                                         \
    if (m_v)                                                                                                           \
    {                                                                                                                  \
        memdelete(m_v);                                                                                                \
        m_v = nullptr;                                                                                                 \
    }

void FmodEditorPlugin::_exit_tree()
{

    remove_inspector_plugin(eventInspector);
    remove_inspector_plugin(bankInspector);
    remove_export_plugin(export_plugin);
    remove_import_plugin(bankImporter);
    remove_tool_menu_item("Finish FMOD Godot setup");
    memdelete_notnull(log);
    memdelete_notnull(browser);
    memdelete_notnull(installer);
}
#undef memdelete_notnull
} // namespace FmodGodot
