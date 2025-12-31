
#include "classes/popup.hpp"
#include "variant/callable_method_pointer.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_bank_importer.h"
#include "fmod_editor_interface.h"
#include "fmod_editor_plugin.h"
#include "fmod_string_names.h"
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
using namespace godot;
namespace FmodGodot
{
void FmodEditorPlugin::_bind_methods()
{
}

String FmodEditorPlugin::_get_plugin_name() const
{
    return "FMOD";
}

Ref<Texture2D> FmodEditorPlugin::_get_plugin_icon() const
{
    return FmodEditorInterface::get_singleton()->get_theme()->fmod_icon;
}

FmodEditorPlugin::FmodEditorPlugin()
{
}
FmodEditorPlugin::~FmodEditorPlugin()
{
}

void FmodEditorPlugin::_enter_tree()
{
    eventInspector = memnew(EventInspector);
    add_inspector_plugin(eventInspector);
    bankInspector = memnew(BankInspectorPlugin);
    add_inspector_plugin(bankInspector);
    bankImporter = memnew(FmodBankImporter);
    add_import_plugin(bankImporter);
    browser = memnew(FmodEventBrowser());
    add_control_to_bottom_panel(browser, "Fmod Event Browser");
    Ref<PackedScene> installer_scene =
        ResourceLoader::get_singleton()->load("res://addons/FmodGodot/fmod_installer.tscn");
    installer = (Popup *)installer_scene->instantiate();

    get_editor_interface()->get_base_control()->add_child(installer);
    add_tool_menu_item("Finish FMOD Godot setup", Callable(installer,"popup"));
}
void FmodEditorPlugin::_exit_tree()
{
    remove_inspector_plugin(eventInspector);
    remove_inspector_plugin(bankInspector);
    remove_import_plugin(bankImporter);
    remove_tool_menu_item("Finish FMOD Godot setup");
}
} // namespace FmodGodot
#endif