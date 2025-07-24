#include "register_types.h"
#include "fmod_bank_loader.h"
// #include "fmodeventemitter2d.h"
#include "fmod_audio_server.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot.hpp>
#include "fmod_event_tree.h"
#include "fmod_event_selector.h"
#include <classes/engine.hpp>
#include "fmod_event_emitter_2d.h"
#include "fmod_event_emitter_3d.h"
#include <godot_cpp/classes/resource_saver.hpp>
// #include "resource_saver.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_string_names.h"
#include <classes/project_settings.hpp>
#include "fmod_bank_format_saver.h"
#include "fmod_bank_format_loader.h"
#include "fmod_listener_2d.h"
#include "fmod_listener_3d.h"
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin_registration.hpp>
#include "fmod_editor_plugin.h"
#include "fmod_event_path_selector_property.h"
#include "fmod_event_guid_selector_property.h"
#include "fmod_event_inspector_plugin.h"
#include "fmod_bank_importer.h"
#include "bank_inspector_plugin.h"
#include <classes/editor_settings.hpp>
#include <classes/editor_interface.hpp>
#include "fmod_object_details.h"
#include "fmod_editor_interface.h"
#include "fmod_event_browser.h"
#endif

using namespace godot;
using namespace FmodGodot;

static FmodAudioServer *audio_server;
FmodBankFormatSaver *bankSaver;
FmodBankFormatLoader *bankLoader;
#ifdef TOOLS_ENABLED
FmodEditorInterface *editor_interface;
#endif
#define ADD_PROJECT_SETTING(setting_str, bool_basic, bool_internal, bool_restart_if_changed, default_value, variant_type, property_hint, hint_string, dictionary) \
  if (!godot::ProjectSettings::get_singleton()->has_setting(setting_str))                                                                                         \
  {                                                                                                                                                               \
    godot::ProjectSettings::get_singleton()->set_setting(setting_str, default_value);                                                                             \
  }                                                                                                                                                               \
  godot::ProjectSettings::get_singleton()->set_initial_value(setting_str, default_value);                                                                         \
  godot::ProjectSettings::get_singleton()->set_as_basic(setting_str, bool_basic);                                                                                 \
  godot::ProjectSettings::get_singleton()->set_as_internal(setting_str, bool_internal);                                                                           \
  godot::ProjectSettings::get_singleton()->set_restart_if_changed(setting_str, bool_restart_if_changed);                                                          \
  dictionary = godot::Dictionary();                                                                                                                               \
  dictionary.get_or_add("name", setting_str);                                                                                                                     \
  dictionary.get_or_add("type", variant_type);                                                                                                                    \
  dictionary.get_or_add("hint", property_hint);                                                                                                                   \
  dictionary.get_or_add("hint_string", hint_string);                                                                                                              \
  godot::ProjectSettings::get_singleton()->add_property_info(dictionary)
namespace FmodGodot
{
  void loadSettings()
  {
    godot::Dictionary propinfo = Dictionary();
    // ADD_EDITOR_SETTING("Fmod/Debug/logging_level", true, false, true, 1, Variant::Type::INT, PROPERTY_HINT_ENUM, "NONE:0,ERROR:1,WARNING:2,LOG:4", propinfo);
    ADD_PROJECT_SETTING(LIVE_UPDATE, true, false, true, 0, Variant::Type::INT, PROPERTY_HINT_ENUM, "Disabled:0, Enabled:1, Development Build Only:2", propinfo);
    ADD_PROJECT_SETTING(LIVE_UPDATE + String(".editor"), true, false, true, 0, Variant::Type::INT, PROPERTY_HINT_ENUM, "Disabled:0, Enabled:1, Development Build Only:2", propinfo);
    ADD_PROJECT_SETTING(LIVE_UPDATE_PORT, true, false, false, 9264, Variant::Type::INT, PROPERTY_HINT_RANGE, "0,65536", propinfo);
    ADD_PROJECT_SETTING(LIVE_UPDATE_PORT + String(".editor"), true, false, false, 9265, Variant::Type::INT, PROPERTY_HINT_RANGE, "0,65536", propinfo);
    ADD_PROJECT_SETTING(SAMPLE_RATE, true, false, false, 48000, Variant::Type::INT, PROPERTY_HINT_RANGE, "8000,192000", propinfo);
    ADD_PROJECT_SETTING(REAL_COUNT, true, false, false, 64, Variant::Type::INT, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(VIRTUAL_COUNT, true, false, false, 1024, Variant::Type::INT, PROPERTY_HINT_RANGE, "0,4095", propinfo);
    ADD_PROJECT_SETTING(BUFFER_LENGTH, true, false, false, 1024, Variant::Type::INT, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(BUFFER_COUNT, true, false, false, 4, Variant::Type::INT, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(DOPPLER_SCALE, true, false, false, 1, Variant::Type::FLOAT, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(DISTANCE_FACTOR, true, false, false, 1, Variant::Type::FLOAT, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(ROLLOFF_SCALE, true, false, false, 1, Variant::Type::FLOAT, PROPERTY_HINT_NONE, "", propinfo);

    ADD_PROJECT_SETTING(SOURCE_TYPE, true, false, false, 1, Variant::Type::INT, PROPERTY_HINT_ENUM, "FMOD Studio Project(WIP):0,Single Platform Build:1, Multiple Platform Build(WIP):2", propinfo);
    ADD_PROJECT_SETTING(FMOD_STUDIO_PATH, true, false, false, "", Variant::Type::STRING, PROPERTY_HINT_GLOBAL_FILE, "", propinfo);
    ADD_PROJECT_SETTING(FMOD_PROJECT_PATH, true, false, false, "", Variant::Type::STRING, PROPERTY_HINT_GLOBAL_FILE, "", propinfo);
    ADD_PROJECT_SETTING(BANK_DIRECTORY, true, false, false, "res://banks/", Variant::Type::STRING, PROPERTY_HINT_DIR, "", propinfo);
    ADD_PROJECT_SETTING(LOAD_BANKS, true, false, false, 0, Variant::Type::INT, PROPERTY_HINT_ENUM, "None:0,Specified:1,All:2", propinfo);

    // if (!godot::ProjectSettings::get_singleton()->has_setting(SPECIFIED_BANKS))
    // {
    //   godot::ProjectSettings::get_singleton()->set_setting(SPECIFIED_BANKS, {});
    // }
    // godot::ProjectSettings::get_singleton()->set_initial_value(SPECIFIED_BANKS, {});
    // godot::ProjectSettings::get_singleton()->set_as_basic(SPECIFIED_BANKS, true);
    // godot::ProjectSettings::get_singleton()->set_as_internal(SPECIFIED_BANKS, false);
    // godot::ProjectSettings::get_singleton()->set_restart_if_changed(SPECIFIED_BANKS, false);
    // propinfo = godot::Dictionary();
    // propinfo.get_or_add("name", SPECIFIED_BANKS);
    // propinfo.get_or_add("type", Variant::Type::PACKED_STRING_ARRAY);
    // propinfo.get_or_add("hint", PROPERTY_HINT_NONE);
    // propinfo.get_or_add("hint_string", "");
    // godot::ProjectSettings::get_singleton()->add_property_info(propinfo);

    ADD_PROJECT_SETTING(SPECIFIED_BANKS, true, false, false, PackedStringArray(), Variant::Type::PACKED_STRING_ARRAY, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(LOAD_SAMPLE_DATA, true, false, false, false, Variant::Type::BOOL, PROPERTY_HINT_NONE, "", propinfo);
    ADD_PROJECT_SETTING(ENCRYPTION_KEY, true, false, false, "", Variant::Type::STRING, PROPERTY_HINT_NONE, "", propinfo);

    ADD_PROJECT_SETTING(LOGGING_LEVEL, true, false, false, 1, Variant::Type::INT, PROPERTY_HINT_ENUM, "NONE:0,ERROR:1,WARNING:2,LOG:4", propinfo);
    ADD_PROJECT_SETTING(DEBUG_TYPE, true, false, false, 0, Variant::Type::INT, PROPERTY_HINT_FLAGS, "MEMORY:256,FILE:512,CODEC:1024,TRACE:2048", propinfo);
    ADD_PROJECT_SETTING(DEBUG_DISPLAY, true, false, false, 0, Variant::Type::INT, PROPERTY_HINT_FLAGS, "TIMESTAMPS:65536,LINENUMBERS:131072,THREAD:262144", propinfo);
  }
}
void initialize_fmod_module(ModuleInitializationLevel p_level)
{
  if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
  {
    loadSettings();
    GDREGISTER_CLASS(FmodAudioServer);
    audio_server = memnew(FmodAudioServer);
    FmodAudioServer::singleton = audio_server;
    audio_server->init_with_project_settings();
    Engine::get_singleton()->register_singleton("FmodAudioServer", audio_server);
  }
  if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
  {

    GDREGISTER_CLASS(FmodListener2D);
    GDREGISTER_CLASS(FmodListener3D);
    GDREGISTER_ABSTRACT_CLASS(FmodBank);
    GDREGISTER_INTERNAL_CLASS(FmodBankFormatLoader);
    GDREGISTER_INTERNAL_CLASS(FmodBankFormatSaver);
    GDREGISTER_CLASS(FmodEventEmitter2D);
    GDREGISTER_CLASS(FmodEventEmitter3D);
    bankLoader = memnew(FmodBankFormatLoader());
    bankSaver = memnew(FmodBankFormatSaver());
    ResourceSaver::get_singleton()->add_resource_format_saver(bankSaver);
    ResourceLoader::get_singleton()->add_resource_format_loader(bankLoader);
    GDREGISTER_CLASS(FmodBankLoader);
    audio_server->load_start_up_banks();
  }

  if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
  {
#ifdef TOOLS_ENABLED
    GDREGISTER_INTERNAL_CLASS(FmodEventPathSelector);
    GDREGISTER_INTERNAL_CLASS(EventTree);

    GDREGISTER_INTERNAL_CLASS(FmodEditorInterface);
    editor_interface = memnew(FmodEditorInterface);
    editor_interface->refresh();
    Engine::get_singleton()->register_singleton("FmodEditorInterface", editor_interface);

    GDREGISTER_INTERNAL_CLASS(FmodEditorPlugin)
    GDREGISTER_INTERNAL_CLASS(EventInspector)
    GDREGISTER_INTERNAL_CLASS(EventPathSelectorProperty)
    GDREGISTER_INTERNAL_CLASS(EventGUIDSelectorProperty)
    GDREGISTER_INTERNAL_CLASS(FmodBankImporter);
    GDREGISTER_INTERNAL_CLASS(BankInspectorPlugin);
    GDREGISTER_INTERNAL_CLASS(FmodObjectDetails);
    GDREGISTER_INTERNAL_CLASS(FmodEventBrowser);
    EditorPlugins::add_by_type<FmodEditorPlugin>();
#endif // TOOLS_ENABLED
  }
}
void uninitialize_fmod_module(ModuleInitializationLevel p_level)
{
  if (p_level == MODULE_INITIALIZATION_LEVEL_SERVERS)
  {
    audio_server->finish();
    Engine::get_singleton()->unregister_singleton("FmodAudioServer");
    memdelete(audio_server);
    return;
  }
  if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
  {
#ifdef TOOLS_ENABLED
    Engine::get_singleton()->unregister_singleton("FmodEditorInterface");
    memdelete(editor_interface);
#endif // TOOLS_ENABLED
  }
  if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
  {
    ResourceLoader::get_singleton()->remove_resource_format_loader(bankLoader);
    ResourceSaver::get_singleton()->remove_resource_format_saver(bankSaver);
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