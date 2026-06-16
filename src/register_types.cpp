#include "register_types.h"
#include "classes/global_constants.hpp"
#include "core/class_db.hpp"
#include "core/property_info.hpp"
#include "fmod_bank_loader.h"
// #include "fmodeventemitter2d.h"
#include "fmod_audio_server.h"
#include "fmod_console.h"
#include "fmod_event_emitter_2d.h"
#include "fmod_event_emitter_3d.h"
#include "fmod_event_previewer.h"
#include "fmod_event_selector.h"
#include <classes/engine.hpp>
#include <gdextension_interface.h>
#include <godot.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/defs.hpp>
// #include "resource_saver.hpp"
#include "fmod_bank_format_loader.h"
#include "fmod_listener_2d.h"
#include "fmod_listener_3d.h"
#include "fmod_project_explorer.h"
#include "fmod_string_names.h"
#include "variant/array.hpp"
#include "variant/variant.hpp"
#include <classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#ifdef TOOLS_ENABLED
#include "fmod_script_client.h"
#include "live_update_indicator.h"
#include "bank_inspector_plugin.h"
#include "fmod_bank_importer.h"
#include "fmod_editor_interface.h"
#include "fmod_editor_plugin.h"
#include "fmod_event_browser.h"
#include "fmod_event_guid_selector_property.h"
#include "fmod_event_inspector_plugin.h"
#include "fmod_event_panner.h"
#include "fmod_event_path_selector_property.h"
#include "fmod_object_details.h"
#include <classes/editor_interface.hpp>
#include <classes/editor_plugin_registration.hpp>
#include <classes/editor_settings.hpp>
#include <fmod_event_panner.h>
#endif

using namespace godot;
using namespace FmodGodot;

static FmodAudioServer *audio_server;
Ref<FmodBankFormatLoader> bank_format_loader;
#ifdef TOOLS_ENABLED
FmodEditorInterface *editor_interface;
#endif

namespace FmodGodot
{
void loadSettings()
{
    GLOBAL_DEF_RST_BASIC(PropertyInfo(Variant::Type::INT, LIVE_UPDATE, godot::PROPERTY_HINT_ENUM,
                                      "Disabled:0, Enabled:1, Development Build Only:2"),
                         0);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::INT, LIVE_UPDATE_PORT, godot::PROPERTY_HINT_RANGE, "0,65536"), 9264);
    GLOBAL_DEF_BASIC(
        PropertyInfo(Variant::Type::INT, LIVE_UPDATE_PORT + String(".editor"), godot::PROPERTY_HINT_RANGE, "0,65536"),
        9265);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, SAMPLE_RATE, PROPERTY_HINT_RANGE, "8000,192000"), 48000);
    GLOBAL_DEF_BASIC(REAL_COUNT, 64);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::INT, VIRTUAL_COUNT, PROPERTY_HINT_RANGE, "0,4095"), 1024);
    GLOBAL_DEF_BASIC(BUFFER_LENGTH, 1024);
    GLOBAL_DEF_BASIC(BUFFER_COUNT, 4);
    GLOBAL_DEF_BASIC(DOPPLER_SCALE, 1.0);
    GLOBAL_DEF_BASIC(DISTANCE_FACTOR, 1.0);
    GLOBAL_DEF_BASIC(ROLLOFF_SCALE, 1.0);

    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::INT, SOURCE_TYPE, PROPERTY_HINT_ENUM,
                                  "Single Platform Build:0, "
                                  "Multiple Platform Build(WIP):1"),
                     1);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::STRING, FMOD_STUDIO_PATH, PROPERTY_HINT_GLOBAL_FILE, ""), "");
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::STRING, FMOD_PROJECT_PATH, PROPERTY_HINT_GLOBAL_FILE, ""), "");
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, BANK_DIRECTORY, PROPERTY_HINT_DIR, ""), "res://banks");
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::INT, LOAD_BANKS, PROPERTY_HINT_ENUM, "None:0,Specified:1 ,All:2,"), 0);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::Type::INT, LOAD_BANKS + String(".editor"), PROPERTY_HINT_ENUM,
                                  "None:0,Specified:1 ,All:2,"),
                     2);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::ARRAY, SPECIFIED_BANKS, PROPERTY_HINT_TYPE_STRING,
                                  vformat("%d/%d:*.bank", Variant::STRING, PROPERTY_HINT_FILE)),
                     Array());
    GLOBAL_DEF_BASIC(LOAD_SAMPLE_DATA, false);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, ENCRYPTION_KEY, godot::PROPERTY_HINT_PASSWORD, ""), "");

    GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, LOGGING_LEVEL, PROPERTY_HINT_ENUM, "NONE:0,ERROR:1,WARNING:2 ,LOG:4"),
                     1);
    GLOBAL_DEF_BASIC(
        PropertyInfo(Variant::INT, DEBUG_TYPE, PROPERTY_HINT_FLAGS, "MEMORY:256,FILE:512,CODEC:1024,TRACE:2048"), 0);
    GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, DEBUG_DISPLAY, PROPERTY_HINT_FLAGS,
                                  "TIMESTAMPS:65536,LINENUMBERS:131072,THREAD:262144"),
                     0);
}
} // namespace FmodGodot
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
        GDREGISTER_CLASS(FmodEventEmitter2D);
        GDREGISTER_CLASS(FmodEventEmitter3D);
        bank_format_loader.instantiate();
        ResourceLoader::get_singleton()->add_resource_format_loader(bank_format_loader);
        GDREGISTER_CLASS(FmodBankLoader);
        audio_server->load_start_up_banks();
    }

    if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
#ifdef TOOLS_ENABLED
        GDREGISTER_INTERNAL_CLASS(FmodEventSelector);
        GDREGISTER_INTERNAL_CLASS(EventTree);

        GDREGISTER_INTERNAL_CLASS(FmodScriptClient)
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
        GDREGISTER_INTERNAL_CLASS(FmodEventPanner);
        GDREGISTER_INTERNAL_CLASS(FmodEventPreviewer);
        GDREGISTER_INTERNAL_CLASS(FmodProjectExplorer);
        GDREGISTER_INTERNAL_CLASS(FmodConsole);
        GDREGISTER_INTERNAL_CLASS(CommandInput);
        GDREGISTER_INTERNAL_CLASS(LiveUpdateIndicator);
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
        ResourceLoader::get_singleton()->remove_resource_format_loader(bank_format_loader);
        bank_format_loader.unref();
    }
}

extern "C"
{
    // Initialization.
    GDExtensionBool GDE_EXPORT fmod_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                 const GDExtensionClassLibraryPtr p_library,
                                                 GDExtensionInitialization *r_initialization)
    {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_fmod_module);
        init_obj.register_terminator(uninitialize_fmod_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_CORE);

        return init_obj.init();
    }
}
