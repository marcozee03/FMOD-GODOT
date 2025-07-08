#include "register_types.h"
#include "bankloader.h"
#include "fmod_audio_server_proxy.h"
// #include "fmodeventemitter2d.h"
#include "fmod_audio_server.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot.hpp>
#include "fmod_event_tree.h"
#include "fmod_event_selector.h"
#include "fmod_banks_explorer.h"
#include <classes/engine.hpp>
// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin_registration.hpp>
#include "fmod_editor_plugin.h"
#include "fmod_event_path_selector_property.h"
#include "fmod_event_guid_selector_property.h"
#include "fmod_event_inspector_plugin.h"
#include "banks_explorer_property.h"
#include "bank_loader_inspector_plugin.h"
#include "fmod_bank_importer.h"
#include "bank_inspector_plugin.h"
#include "bank_inspector.h"
#endif
#include "fmod_bank_format_saver.h"
#include "fmod_bank_format_loader.h"
#include "fmod_listener.h"
using namespace godot;
using namespace FmodGodot;
#include <godot_cpp/classes/resource_saver.hpp>
// #include "resource_saver.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
static FmodAudioServer *audio_server;
FmodBankFormatSaver *bankSaver;
FmodBankFormatLoader *bankLoader;
void initialize_fmod_module(ModuleInitializationLevel p_level)
{
  if (p_level == MODULE_INITIALIZATION_LEVEL_CORE)
  {
    GDREGISTER_CLASS(FmodAudioServer);
    GDREGISTER_CLASS(FmodAudioServerProxy);
    audio_server = memnew(FmodAudioServer);
    audio_server->init();
    FmodAudioServer::singleton = audio_server;
    Engine::get_singleton()->register_singleton("FmodAudioServer", audio_server);
  }
  if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
  {
    GDREGISTER_CLASS(BankLoader);

    GDREGISTER_CLASS(FmodListener);

    GDREGISTER_CLASS(FmodEventPathSelector);
    GDREGISTER_CLASS(EventTree);
    GDREGISTER_CLASS(BanksExplorer);
    GDREGISTER_INTERNAL_CLASS(FmodBankFormatLoader);
    GDREGISTER_INTERNAL_CLASS(FmodBankFormatSaver);
    GDREGISTER_CLASS(FmodBank);
    bankSaver = memnew(FmodBankFormatSaver);
    bankLoader = memnew(FmodBankFormatLoader);
    ResourceSaver::get_singleton()->add_resource_format_saver(bankSaver);
    ResourceLoader::get_singleton()->add_resource_format_loader(bankLoader);
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
    GDREGISTER_INTERNAL_CLASS(FmodBankImporter);
    GDREGISTER_CLASS(BankInspector);
    GDREGISTER_INTERNAL_CLASS(BankInspectorPlugin);
    EditorPlugins::add_by_type<FmodEditorPlugin>();
#endif // TOOLS_ENABLED
  }
}
void uninitialize_fmod_module(ModuleInitializationLevel p_level)
{
  if (p_level == MODULE_INITIALIZATION_LEVEL_CORE)
  {
    audio_server->finish();
    Engine::get_singleton()->unregister_singleton("FmodAudioServer");
    memdelete(audio_server);
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

    init_obj.register_initializer(initialize_fmod_module);
    init_obj.register_terminator(uninitialize_fmod_module);
    init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_CORE);

    return init_obj.init();
  }
}