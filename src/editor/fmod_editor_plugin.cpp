// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include "fmod_editor_plugin.h"
#include "fmod_bank_importer.h"
using namespace godot;
namespace FmodGodot
{
    void FmodEditorPlugin::_bind_methods()
    {
    }

    FmodEditorPlugin::FmodEditorPlugin()
    {
        eventInspector = memnew(EventInspector);
        add_inspector_plugin(eventInspector);
        bankLoaderInspector = memnew(BankLoaderInspector);
        add_inspector_plugin(bankLoaderInspector);
        bankInspector = memnew(BankInspectorPlugin);
        add_inspector_plugin(bankInspector);
        bankImporter = memnew(FmodBankImporter);
        add_import_plugin(bankImporter);
    }
    FmodEditorPlugin::~FmodEditorPlugin()
    {
        remove_inspector_plugin(eventInspector);
        remove_inspector_plugin(bankLoaderInspector);
        remove_inspector_plugin(bankInspector);
        remove_import_plugin(bankImporter);
        // memdelete(eventInspector);
        // memdelete(bankLoaderInspector);
    }
    void FmodEditorPlugin::_enter_tree()
    {
    }
    void FmodEditorPlugin::_exit_tree()
    {
    }
}
#endif