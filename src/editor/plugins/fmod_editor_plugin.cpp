#define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include "fmod_editor_plugin.h"
#include "fmod_bank_importer.h"
#include <classes/project_settings.hpp>
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#include "fmod_string_names.h"
#include "fmod_editor_interface.h"
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
        // eventInspector = memnew(EventInspector);
        // add_inspector_plugin(eventInspector);
        // bankInspector = memnew(BankInspectorPlugin);
        // add_inspector_plugin(bankInspector);
        // bankImporter = memnew(FmodBankImporter);
        // add_import_plugin(bankImporter);
        // add_control_to_bottom_panel()
    }
    FmodEditorPlugin::~FmodEditorPlugin()
    {
        // remove_inspector_plugin(eventInspector);
        // remove_inspector_plugin(bankInspector);
        // remove_import_plugin(bankImporter);
        // memdelete(eventInspector);
        // memdelete(bankLoaderInspector);
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
    }
    void FmodEditorPlugin::_exit_tree()
    {
        remove_inspector_plugin(eventInspector);
        remove_inspector_plugin(bankInspector);
        remove_import_plugin(bankImporter);
    }
}
#endif