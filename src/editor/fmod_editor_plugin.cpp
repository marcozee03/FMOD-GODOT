#ifdef TOOLS_ENABLED
#include "fmod_editor_plugin.h"

using namespace godot;
namespace FMODGodot
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
    }
    void FmodEditorPlugin::_enter_tree()
    {
        fmodEngine = memnew(FMODEngine);
        add_child(fmodEngine);
        std::cout << "addengine";
    }
    void FmodEditorPlugin::_exit_tree()
    {
        remove_child(fmodEngine);
        fmodEngine->queue_free();
        std::cout << "remove engine";
    }
}
#endif