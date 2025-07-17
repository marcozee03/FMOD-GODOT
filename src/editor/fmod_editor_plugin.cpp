#ifdef TOOLS_ENABLED
#include "fmod_editor_plugin.h"
#include "fmod_bank_importer.h"
#include <classes/project_settings.hpp>
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#include "fmod_string_names.h"
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
        bankInspector = memnew(BankInspectorPlugin);
        add_inspector_plugin(bankInspector);
        bankImporter = memnew(FmodBankImporter);
        add_import_plugin(bankImporter);
    }
    FmodEditorPlugin::~FmodEditorPlugin()
    {
        remove_inspector_plugin(eventInspector);
        remove_inspector_plugin(bankInspector);
        remove_import_plugin(bankImporter);
        // memdelete(eventInspector);
        // memdelete(bankLoaderInspector);
    }

}
#endif