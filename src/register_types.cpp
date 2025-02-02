#include "register_types.h"
#include "bankloader.h"
// #include "fmodeventemitter2d.h"
#include "fmodengine.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot.hpp>
#include "nodes/editor_ui/fmod_event_selector.h"
#include "fmod_event_tree.h"
#include "fmod_event_selector.h"
#include "nodes/editor_ui/fmod_banks_explorer.h"
#include <classes/engine.hpp>
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin_registration.hpp>
#include "fmod_editor_plugin.h"
#include "fmod_event_path_selector_property.h"
#include "fmod_event_guid_selector_property.h"
#include "fmod_event_inspector_plugin.h"
#include "banks_explorer_property.h"
#include "bank_loader_inspector_plugin.h"
#endif

using namespace godot;
using namespace FMODGodot;
void initialize_example_module(ModuleInitializationLevel p_level)
{
  if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
  {
    GDREGISTER_CLASS(BankLoader);
    GDREGISTER_CLASS(FMODEngine);
    GDREGISTER_CLASS(FmodEventPathSelector);
    GDREGISTER_CLASS(EventTree);
    GDREGISTER_CLASS(BanksExplorer);
    // GDREGISTER_INTERNAL_CLASS(EventSelector)
  }

  // ClassDB::register_class<FMODEventEmitter2D>();

  if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
  {
#ifdef TOOLS_ENABLED
    // GDREGISTER_CLASS(FMODEDITOR)
    GDREGISTER_INTERNAL_CLASS(FmodEditorPlugin)
    GDREGISTER_INTERNAL_CLASS(EventInspector)
    GDREGISTER_INTERNAL_CLASS(BankLoaderInspector)
    GDREGISTER_INTERNAL_CLASS(EventPathSelectorProperty)
    GDREGISTER_INTERNAL_CLASS(EventGUIDSelectorProperty)
    GDREGISTER_INTERNAL_CLASS(BanksExplorerProperty)
    EditorPlugins::add_by_type<FmodEditorPlugin>();
#endif // TOOLS_ENABLED
  }
}
void uninitialize_fmod_module(ModuleInitializationLevel p_level)
{
  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
  {
    return;
  }
}

extern "C"
{
  // Initialization.
  GDExtensionBool GDE_EXPORT
  fmod_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                    const GDExtensionClassLibraryPtr p_library,
                    GDExtensionInitialization *r_initialization)
  {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library,
                                                   r_initialization);

    init_obj.register_initializer(initialize_example_module);
    init_obj.register_terminator(uninitialize_fmod_module);
    init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_CORE);

    return init_obj.init();
  }
}