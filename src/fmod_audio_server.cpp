#include "fmod_audio_server.h"
#include "classes/engine_debugger.hpp"
#include "classes/global_constants.hpp"
#include "classes/resource_loader.hpp"
#include "core/defs.hpp"
#include "core/error_macros.hpp"
#include "core/object.hpp"
#include "core/print_string.hpp"
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_string_names.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include "variant/callable.hpp"
#include "variant/dictionary.hpp"
#include "variant/typed_dictionary.hpp"
#include "variant/utility_functions.hpp"
#include "variant/variant.hpp"
#include <classes/dir_access.hpp>
#include <classes/os.hpp>
#include <cstdint>
#include <fmod_errors.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#ifdef TOOLS_ENABLED
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#endif
#ifdef DEBUG_ENABLED
#include "classes/performance.hpp"
#endif

using namespace std;
using namespace godot;
namespace FmodGodot
{

FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS p_flags, const char *p_file, int p_line, const char *p_func,
                                const char *p_message)
{
    if (p_flags & FMOD_DEBUG_LEVEL_ERROR)
    {
        _err_print_error(p_func, p_file, p_line, p_message, true, false);
    }
    else if (p_flags & FMOD_DEBUG_LEVEL_WARNING)
    {
        _err_print_error(p_func, p_file, p_line, p_message, true, true);
    }
    else
    {
        print_line(p_message);
    }
    return FMOD_OK;
}
FMOD_RESULT F_CALL fmod_studio_system_callback(FMOD_STUDIO_SYSTEM *p_system, FMOD_STUDIO_SYSTEM_CALLBACK_TYPE p_type,
                                               void *p_commanddata, void *p_userdata)
{
    FmodAudioServer *as = reinterpret_cast<FmodAudioServer *>(p_userdata);
    if (as)
    {
        switch (p_type)
        {
        case FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_CONNECTED: {
            as->emit_signal("live_update_connected");
#ifdef DEBUG_ENABLED
            if (!Engine::get_singleton()->is_editor_hint())
            {
                EngineDebugger *debugger = EngineDebugger::get_singleton();
                if (debugger != nullptr)
                {
                    debugger->send_message("fmod:live_update", {true});
                }
            }
#endif
            as->live_update_connected = true;
        }
        break;

        case FMOD_STUDIO_SYSTEM_CALLBACK_LIVEUPDATE_DISCONNECTED: {
            as->emit_signal("live_update_disconnected");
#ifdef DEBUG_ENABLED
            if (!Engine::get_singleton()->is_editor_hint())
            {
                EngineDebugger *debugger = EngineDebugger::get_singleton();
                if (debugger != nullptr)
                {
                    debugger->send_message("fmod:live_update", {false});
                }
            }
#endif

            as->live_update_connected = false;
        }
        break;
        case FMOD_STUDIO_SYSTEM_CALLBACK_BANK_UNLOAD: {
        }
        break;
        case FMOD_STUDIO_SYSTEM_CALLBACK_PREUPDATE: {
            as->emit_signal("preupdate");
        }
        break;
        case FMOD_STUDIO_SYSTEM_CALLBACK_POSTUPDATE: {
            as->emit_signal("postupdate");
        }
        break;
        }
        return FMOD_OK;
    }
    return FMOD_ERR_INVALID_HANDLE;
}

FMOD_RESULT open_callback(const char *p_name, unsigned int *p_filesize, void **p_handle, void *p_userdata)
{
    auto file = FileAccess::open(String(p_name), FileAccess::ModeFlags::READ);
    if (FileAccess::get_open_error() != OK)
    {
        return godot_file_error_to_fmod_file_error(FileAccess::get_open_error());
    }
    file->reference();
    *p_handle = file.ptr();
    *p_filesize = file->get_length();
    return godot_file_error_to_fmod_file_error(file->get_error());
}
FMOD_RESULT close_callback(void *p_handle, void *p_userdata)
{
    FileAccess *file = reinterpret_cast<FileAccess *>(p_handle);
    file->close();
    file->unreference();
    FMOD_RESULT result = godot_file_error_to_fmod_file_error(file->get_error());
    memdelete(file);
    return result;
}
FMOD_RESULT seek_callback(void *p_handle, unsigned int p_pos, void *p_userdata)
{
    FileAccess *file = reinterpret_cast<FileAccess *>(p_handle);
    file->seek(p_pos);
    return godot_file_error_to_fmod_file_error(file->get_error());
}
FMOD_RESULT read_callback(void *p_handle, void *p_buffer, unsigned int p_sizebytes, unsigned int *p_bytesread,
                          void *p_userdata)
{
    FileAccess *file = reinterpret_cast<FileAccess *>(p_handle);
    *p_bytesread = file->get_buffer(reinterpret_cast<uint8_t *>(p_buffer), p_sizebytes);
    return godot_file_error_to_fmod_file_error(file->get_error());
}

#pragma region Server Functionality
FmodAudioServer *FmodAudioServer::singleton = nullptr;

godot::String FmodAudioServer::_get_version_number()
{
    constexpr unsigned int major = (FMOD_VERSION & 0xffff0000) >> 16;
    constexpr unsigned int minor = (FMOD_VERSION & 0x0000ff00) >> 8;
    constexpr unsigned int patch = (FMOD_VERSION & 0x000000ff);
    return vformat("%x.%02x.%02x", major, minor, patch);
}
Handle FmodAudioServer::_get_core()
{
    return std::bit_cast<Handle>(core_system);
}
Handle FmodAudioServer::_get_studio()
{
    return std::bit_cast<Handle>(studio_system);
}
Handle FmodAudioServer::_create_instance(const Vector4i &p_guid) const
{
    return std::bit_cast<Handle>(create_instance(p_guid));
}
Handle FmodAudioServer::_create_instance_by_path(const String &p_path) const
{
    return _create_instance(path_to_guid(p_path));
}

void FmodAudioServer::_attach_instance_to_node3d(Node3D *p_node, Handle p_event, bool p_non_rigid_body_velocity)
{
    attach_instance(p_node, std::bit_cast<FMOD_STUDIO_EVENTINSTANCE *>(p_event), p_non_rigid_body_velocity);
}
void FmodAudioServer::_attach_instance_to_rigid_body3d(RigidBody3D *p_node, Handle p_event)
{
    attach_instance(p_node, std::bit_cast<FMOD_STUDIO_EVENTINSTANCE *>(p_event));
}
void FmodAudioServer::_attach_instance_to_node2d(Node2D *p_node, Handle p_event, bool p_non_rigid_body_velocity)
{
    attach_instance(p_node, std::bit_cast<FMOD_STUDIO_EVENTINSTANCE *>(p_event), p_non_rigid_body_velocity);
}
void FmodAudioServer::_attach_instance_to_rigid_body2d(RigidBody2D *p_node, Handle p_event)
{

    attach_instance(p_node, std::bit_cast<FMOD_STUDIO_EVENTINSTANCE *>(p_event));
}
void FmodAudioServer::_detach_instance(Handle p_event)
{
    detach_instance(std::bit_cast<FMOD_STUDIO_EVENTINSTANCE *>(p_event));
}
Handle FmodAudioServer::_get_event_description(const String &p_path) const
{
    return std::bit_cast<Handle>(get_event_description(p_path));
}
Handle FmodAudioServer::_get_event_description(const Vector4i &p_guid) const
{
    return std::bit_cast<Handle>(get_event_description(p_guid));
}
Handle FmodAudioServer::_get_bus(const Vector4i &p_path) const
{

    return std::bit_cast<Handle>(get_bus(p_path));
}
Handle FmodAudioServer::_get_vca(const Vector4i &p_path) const
{
    return std::bit_cast<Handle>(get_vca(p_path));
}
Handle FmodAudioServer::_get_bus_by_path(const String &p_path) const
{

    return std::bit_cast<Handle>(get_bus(p_path));
}
Handle FmodAudioServer::_get_vca_by_path(const String &p_path) const
{
    return std::bit_cast<Handle>(get_vca(p_path));
}

FmodAudioServer::FmodAudioServer()
{
}
FmodAudioServer::~FmodAudioServer()
{
    finish();
}

FMOD_RESULT FmodAudioServer::init_with_project_settings()
{
    return init(InitSettings::create_from_project_settings());
}

FMOD_RESULT FmodAudioServer::init(const InitSettings &p_settings)
{
    thread.instantiate();
    exit_thread = false;
    thread_exited = false;
    FMOD_STUDIO_INITFLAGS studio_init = FMOD_STUDIO_INIT_NORMAL;
    switch (p_settings.live_update)
    {
    case InitSettings::LIVE_UPDATE_DISABLED: // disabled
        studio_init = FMOD_STUDIO_INIT_NORMAL;
        break;
    case InitSettings::LIVE_UPDATE_ENABLED:
        studio_init = FMOD_STUDIO_INIT_LIVEUPDATE;
    case InitSettings::LIVE_UPDATE_DEV_ONLY: // dev
#if defined(TOOLS_ENABLED) || defined(DEBUG)
        studio_init = FMOD_STUDIO_INIT_LIVEUPDATE;
#else
        studio_init = FMOD_STUDIO_INIT_NORMAL;
#endif
        /* code */
        break;

    default:
        break;
    }
    FMOD_ERR_FAIL_VE_MSG(
        FMOD_Debug_Initialize(p_settings.logging_level | p_settings.debug_type | p_settings.debug_display,
                              FMOD_DEBUG_MODE_CALLBACK, fmod_debug_callback, nullptr),
        "Failed to initialize debug callbacks.");

    FMOD_ERR_FAIL_VE_MSG(FMOD_Studio_System_Create(&studio_system, FMOD_VERSION), "Failed to create studio system.");
    FMOD_ERR_FAIL_VE_MSG(FMOD_Studio_System_GetCoreSystem(studio_system, &core_system), "Failed to get core system.");

    // Core Settings | Initialize to zero/NUll since thats default according to FMOD docs. and as recommended;
    FMOD_ADVANCEDSETTINGS fmod_settings = p_settings.get_fmod_advanced_settings();

    FMOD_ERR_FAIL_VE(FMOD_System_SetAdvancedSettings(core_system, &fmod_settings))
    FMOD_ERR_FAIL_VE(FMOD_System_SetSoftwareFormat(core_system, p_settings.sample_rate, FMOD_SPEAKERMODE_DEFAULT, 0));
    FMOD_ERR_FAIL_VE(
        FMOD_System_SetDSPBufferSize(core_system, p_settings.dspbuffer_length, p_settings.dspbuffer_count));
    FMOD_ERR_FAIL_VE(FMOD_System_SetSoftwareChannels(core_system, p_settings.software_channels));
    FMOD_ERR_FAIL_VE(FMOD_System_Set3DSettings(core_system, p_settings.doppler_scale, p_settings.distance_factor,
                                               p_settings.rolloff_scale));
    FMOD_ERR_FAIL_VE(FMOD_System_SetFileSystem(core_system, open_callback, close_callback, read_callback, seek_callback,
                                               nullptr, nullptr, 2048));
    // studio settings
    FMOD_STUDIO_ADVANCEDSETTINGS studio_settings = p_settings.get_fmod_studio_advanced_settings(studio_system);
    // PRevents dangling pointer;
    encryption_key = p_settings.encryption_key.utf8();
    studio_settings.encryptionkey = encryption_key.ptr();

    FMOD_ERR_FAIL_VE(FMOD_Studio_System_SetAdvancedSettings(studio_system, &studio_settings));
    FMOD_ERR_FAIL_VE(
        FMOD_Studio_System_SetCallback(studio_system, fmod_studio_system_callback, FMOD_STUDIO_SYSTEM_CALLBACK_ALL))
    FMOD_ERR_FAIL_VE(FMOD_Studio_System_SetUserData(studio_system, this))

    FMOD_ERR_FAIL_VE_MSG(FMOD_Studio_System_Initialize(studio_system, p_settings.virtual_channels, studio_init,
                                                       FMOD_INIT_NORMAL & FMOD_INIT_3D_RIGHTHANDED, nullptr),
                         "Failed to initialized Fmod Studio System");
    initialized = true;

    thread->start(callable_mp(this, &FmodAudioServer::_thread_func), Thread::Priority::PRIORITY_NORMAL);
    String plugin_path = GLOBAL_GET(PLUGIN_PATH);
    ;
    if (!plugin_path.is_empty())
    {
        FMOD_System_SetPluginPath(core_system, plugin_path.utf8().ptr());
    }
    unsigned int handle;
    TypedDictionary<String, int> plugins = GLOBAL_GET(PLUGINS);
    for (auto plugin_file : plugins.keys())
    {
        FMOD_System_LoadPlugin(core_system, String(plugin_file).utf8().ptr(), &handle,
                               /*Priority*/ plugins[plugin_file]);
    }
#ifdef DEBUG_ENABLED
    Callable cpu = callable_mp(this, &FmodAudioServer::track_cpu);
    Performance::get_singleton()->add_custom_monitor(
        "fmod/memory_usage", callable_mp(this, &FmodAudioServer::track_memory), {}, Performance::MONITOR_TYPE_MEMORY);
    Performance::get_singleton()->add_custom_monitor("fmod/dsp_cpu_usage", cpu, {USAGE_DSP},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
    Performance::get_singleton()->add_custom_monitor("fmod/convulution1_cpu_usage", cpu, {USAGE_CONVULUTION1},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
    Performance::get_singleton()->add_custom_monitor("fmod/convulution2_cpu_usage", cpu, {USAGE_CONVULUTION1},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
    Performance::get_singleton()->add_custom_monitor("fmod/core_update_cpu_usage", cpu, {USAGE_UPDATE},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
    Performance::get_singleton()->add_custom_monitor("fmod/studio_update_cpu_usage", cpu, {USAGE_STUDIO_UPDATE},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
    Performance::get_singleton()->add_custom_monitor("fmod/stream_cpu_usage", cpu, {USAGE_STREAM},
                                                     Performance::MONITOR_TYPE_PERCENTAGE);
#endif
    return FMOD_OK;
}

void FmodAudioServer::_physics_process()
{
    if (thread_exited)
    {
        return;
    }
    lock();
    for (uint32_t i = 0; i < instances.size(); i++)
    {
        if (!FMOD_Studio_EventInstance_IsValid(instances[i].instance))
        {
            instances[i] = instances[instances.size() - 1];
            instances.remove_at(instances.size() - 1);
            i--;
            continue;
        }
        FMOD_3D_ATTRIBUTES attributes;
        Vector3 velocity;
        if (!instances[i].node->is_inside_tree())
        {
            FMOD_Studio_EventInstance_Stop(instances[i].instance, FMOD_STUDIO_STOP_IMMEDIATE);
            FMOD_Studio_EventInstance_Release(instances[i].instance);
            instances[i] = instances[instances.size() - 1];
            instances.remove_at(instances.size() - 1);
            i--;
            continue;
        }
        switch (instances[i].attachment)
        {
        case Attachment::NODE2D: {
            FMOD_Studio_EventInstance_Get3DAttributes(instances[i].instance, &attributes);
            Vector2 pos = instances[i].node2D->get_global_position();
            velocity = (Vector3(pos.x, pos.y, 0) - instances[i].lastFramePosition) /
                       instances[i].node2D->get_physics_process_delta_time();
            attributes.velocity = to_fmod_vector(velocity);
            instances[i].lastFramePosition = Vector3(pos.x, pos.y, 0);
            FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
            break;
        }
        case Attachment::NODE3D: {
            FMOD_Studio_EventInstance_Get3DAttributes(instances[i].instance, &attributes);
            velocity = (instances[i].node3D->get_global_position() - instances[i].lastFramePosition) /
                       instances[i].node3D->get_physics_process_delta_time();
            attributes.velocity = to_fmod_vector(velocity);
            instances[i].lastFramePosition = instances[i].node3D->get_global_position();
            FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
            break;
        }
        default:
            break;
        }
    }
    unlock();
}
#ifdef DEBUG_ENABLED
int FmodAudioServer::track_memory()
{
    int current;
    FMOD_Memory_GetStats(&current, nullptr, false);
    return current;
}
float FmodAudioServer::track_cpu(CPUUsage p_usage)
{

    FMOD_STUDIO_CPU_USAGE studio_usage;
    FMOD_CPU_USAGE usage;
    FMOD_Studio_System_GetCPUUsage(studio_system, &studio_usage, &usage);
    switch (p_usage)
    {
    case USAGE_STUDIO_UPDATE:
        return studio_usage.update;
    case USAGE_DSP:
        return usage.dsp;
    case USAGE_STREAM:
        return usage.stream;
    case USAGE_GEOMETRY:
        return usage.geometry;
    case USAGE_UPDATE:
        return usage.update;
    case USAGE_CONVULUTION1:
        return usage.convolution1;
    case USAGE_CONVULUTION2:
        return usage.convolution2;
        break;
    }
    return 0;
}
bool FmodAudioServer::debugger_capture(const String &p_message, const Array &p_data)
{
    if (p_message == "mute_audio")
    {
        set_muted(static_cast<bool>(p_data[0]));
    }
    return false;
}

#endif

void FmodAudioServer::_thread_func()
{
    thread->set_thread_safety_checks_enabled(false);
    SceneTree *tree = cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    while (!tree)
    {
        OS::get_singleton()->delay_usec(20);
        tree = cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    }
    uint64_t tree_instance_id = tree->get_instance_id();
    Callable physics_callback = callable_mp(this, &FmodAudioServer::_physics_process);
    tree->connect("physics_frame", physics_callback);
    while (!exit_thread)
    {
        FMOD_3D_ATTRIBUTES attributes;
        lock();
        for (uint32_t i = 0; i < instances.size(); i++)
        {
            if (!FMOD_Studio_EventInstance_IsValid(instances[i].instance))
            {
                instances[i] = instances[instances.size() - 1];
                instances.remove_at(instances.size() - 1);
                i--;
                continue;
            }
            if (!instances[i].node->is_inside_tree())
            {
                FMOD_Studio_EventInstance_Stop(instances[i].instance, FMOD_STUDIO_STOP_IMMEDIATE);
                FMOD_Studio_EventInstance_Release(instances[i].instance);
                instances[i] = instances[instances.size() - 1];
                instances.remove_at(instances.size() - 1);
                i--;
                continue;
            }
            switch (instances[i].attachment)
            {
            case NODE2D:
                attributes = to_3d_attributes(instances[i].node2D);
                FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
                break;
            case NODE3D:
                attributes = to_3d_attributes(instances[i].node3D);
                FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
                break;
            case RIGIDBODY2D:
                attributes = to_3d_attributes(instances[i].rigidBody2D);
                FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
                break;
            case RIGIDBODY3D:
                attributes = to_3d_attributes(instances[i].rigidBody3D);
                FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
                break;

            default:
                break;
            }
        }
        unlock();
        FMOD_Studio_System_Update(studio_system);
        OS::get_singleton()->delay_usec(20);
    }
    // mainly here for the editor
    if (UtilityFunctions::is_instance_id_valid(tree_instance_id))
    {
        tree->disconnect("physics_frame", physics_callback);
    }
    FMOD_Studio_System_Release(studio_system);
}

/**
 * @brief Returns index of event instance or -1 if not found
 *
 * @param p_event
 * @return
 */
int FmodAudioServer::_find_instance(FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    for (uint32_t i = 0; i < instances.size(); i++)
    {
        if (instances[i].instance == p_event)
        {
            return i;
        }
    }
    return -1;
}

void FmodAudioServer::unlock()
{
    if (!thread.is_valid())
    {
        return;
    }

    mutex.unlock();
}

void FmodAudioServer::lock()
{
    if (!thread.is_valid())
    {
        return;
    }

    mutex.lock();
}

void FmodAudioServer::finish()
{
    if (!thread.is_valid())
    {
        return;
    }

    exit_thread = true;
    thread->wait_to_finish();
    thread.unref();
    initialized = false;
    thread_exited = true;
}
void FmodAudioServer::_bind_methods()
{
    BIND_ENUM_CONSTANT(InitSettings::LIVE_UPDATE_DISABLED);
    BIND_ENUM_CONSTANT(InitSettings::LIVE_UPDATE_ENABLED);
    BIND_ENUM_CONSTANT(InitSettings::LIVE_UPDATE_DEV_ONLY);
    ClassDB ::bind_static_method(get_class_static(), D_METHOD("get_version_number"), &self_type ::_get_version_number);
    ClassDB ::bind_method(D_METHOD("get_core"), &self_type ::_get_core);
    ClassDB ::bind_method(D_METHOD("get_studio"), &self_type ::_get_studio);
    ClassDB ::bind_method(D_METHOD("create_instance", "guid"), &self_type ::_create_instance);
    ClassDB ::bind_method(D_METHOD("create_instance_by_path", "path"), &self_type ::_create_instance_by_path);

    BIND_METHOD(unload_banks);
    BIND_BOOL_PROPERTY(muted);
    BIND_METHOD_OL_WITH_DEF(play_one_shot, "play_one_shot", ARGS("guid", "position"), void,
                            ARGS(const Vector4i &, const Vector3 &), DEFVAL(Vector3()));
    BIND_METHOD_OL_WITH_DEF(play_one_shot, "play_one_shot_by_path", ARGS("path", "position"), void,
                            ARGS(const String &, const Vector3 &), DEFVAL(Vector3()));

    BIND_METHOD_OL_WITH_DEF(play_one_shot_attached, "play_one_shot_attached_3d",
                            ARGS("guid", "node", "non_rigid_body_velocity"), void,
                            ARGS(const Vector4i &, Node3D *, bool), DEFVAL(false));
    BIND_METHOD_OL_WITH_DEF(play_one_shot_attached, "play_one_shot_attached_3d_by_path",
                            ARGS("path", "node", "non_rigid_body_velocity"), void, ARGS(const String &, Node3D *, bool),
                            DEFVAL(false));

    BIND_METHOD_OL_WITH_DEF(play_one_shot_attached, "play_one_shot_attached_2d",
                            ARGS("guid", "node", "non_rigid_body_velocity"), void,
                            ARGS(const Vector4i &, Node2D *, bool), DEFVAL(false));
    BIND_METHOD_OL_WITH_DEF(play_one_shot_attached, "play_one_shot_attached_2d_by_path",
                            ARGS("path", "node", "non_rigid_body_velocity"), void, ARGS(const String &, Node2D *, bool),
                            DEFVAL(false));

    BIND_METHOD_OL(play_one_shot_attached, "play_one_shot_attached_rigidbody_3d", ARGS("guid", "rigidbody"), void,
                   const Vector4i &, RigidBody3D *);
    BIND_METHOD_OL(play_one_shot_attached, "play_one_shot_attached_rigidbody_3d_by_path", ARGS("path", "rigidbody"),
                   void, const String &, RigidBody3D *);

    BIND_METHOD_OL(play_one_shot_attached, "play_one_shot_attached_rigidbody_2d", ARGS("guid", "rigidbody"), void,
                   const Vector4i &, RigidBody2D *);
    BIND_METHOD_OL(play_one_shot_attached, "play_one_shot_attached_rigidbody_2d_by_path", ARGS("path", "rigidbody"),
                   void, const String &, RigidBody2D *);

    BIND_STATIC_METHOD(get_singleton);
    BIND_METHOD(any_sample_data_loading);

    ClassDB ::bind_method(D_METHOD("attach_instance_to_node3d", "node", "event", "non_rigid_body_velocity"),
                          &self_type::_attach_instance_to_node3d, (false));
    ClassDB ::bind_method(D_METHOD("attach_instance_to_node2d", "node", "event", "non_rigid_body_velocity"),
                          &self_type::_attach_instance_to_node2d, (false));
    ClassDB ::bind_method(D_METHOD("attach_instance_to_rigid_body2d", "rigid_body", "event"),
                          &self_type::_attach_instance_to_rigid_body2d);
    ClassDB ::bind_method(D_METHOD("attach_instance_to_rigid_body3d", "rigid_body", "event"),
                          &self_type::_attach_instance_to_rigid_body3d);
    ClassDB ::bind_method(D_METHOD("detach_instance", "handle"), &self_type ::_detach_instance);

    BIND_METHOD_OL_C(_get_event_description, "get_event_description", ARGS("id"), Handle, const Vector4i &);
    BIND_METHOD_OL_C(_get_event_description, "get_event_description_by_path", ARGS("path"), Handle, const String &);
    ClassDB ::bind_method(D_METHOD("get_bus", "guid"), &self_type ::_get_bus);
    ClassDB ::bind_method(D_METHOD("get_vca", "guid"), &self_type ::_get_vca);

    ClassDB ::bind_method(D_METHOD("get_bus_by_path", "path"), &self_type ::_get_bus_by_path);
    ClassDB ::bind_method(D_METHOD("get_vca_by_path", "path"), &self_type ::_get_vca_by_path);

    BIND_METHOD(path_to_guid, "path");
    BIND_METHOD(has_bank_loaded, "bank_name");
    BIND_METHOD(pause_all_events, "pause");

    BIND_METHOD_OL_WITH_DEF(set_listener_location, "set_listener_location_3d",
                            ARGS("node", "listener_index", "attenuation_object"), void, ARGS(Node3D *, int, Node3D *),
                            DEFVAL(0), DEFVAL(nullptr));
    BIND_METHOD_OL_WITH_DEF(set_listener_location, "set_listener_location_2d",
                            ARGS("node", "listener_index", "attenuation_object"), void, ARGS(Node2D *, int, Node2D *),
                            DEFVAL(0), DEFVAL(nullptr));
    BIND_METHOD_OL_WITH_DEF(set_listener_location, "set_listener_location_rigid_body2d",
                            ARGS("node", "listener_index", "attenuation_object"), void,
                            ARGS(RigidBody2D *, int, Node2D *), DEFVAL(0), DEFVAL(nullptr));
    BIND_METHOD_OL_WITH_DEF(set_listener_location, "set_listener_location_rigid_body3d",
                            ARGS("node", "listener_index", "attenuation_object"), void,
                            ARGS(RigidBody3D *, int, Node3D *), DEFVAL(0), DEFVAL(nullptr));
    BIND_METHOD(is_live_update_connected);
    BIND_METHOD(wait_for_all_sample_loading);
    ADD_SIGNAL(MethodInfo("preupdate"));
    ADD_SIGNAL(MethodInfo("postupdate"));
    // ADD_SIGNAL(MethodInfo("bank_unloaded"));
    ADD_SIGNAL(MethodInfo("live_update_connected"));
    ADD_SIGNAL(MethodInfo("live_update_disconnected"));
}

#pragma endregion

FmodAudioServer *FmodAudioServer::get_singleton()
{
    return singleton;
}
void FmodAudioServer::_load_all_banks_in(const String &p_directory)
{
    auto dir = DirAccess::open(p_directory);
    if (DirAccess::get_open_error() != godot::OK)
    {
        print_error(vformat("Failed To Open Bank Directory: '%s' with error: %s", p_directory,
                            UtilityFunctions::error_string(DirAccess::get_open_error())));
        return;
    }
    for (auto file : dir->get_files())
    {
        if (!file.ends_with(".bank"))
        {
            continue;
        }
        Ref<FmodBank> loaded_bank = ResourceLoader::get_singleton()->load(p_directory.path_join(file), "FmodBank",
                                                                          godot::ResourceLoader::CACHE_MODE_REUSE);
        if (loaded_bank.is_valid())
        {
            registered_banks.push_back(loaded_bank);
        }
    }
}
void FmodAudioServer::_load_start_up_banks()
{
    if (!start_up_banks_loaded)
    {
        print_verbose("Loading Start Up Banks");
        start_up_banks_loaded = true;
        int what = ProjectSettings::get_singleton()->get_setting_with_override(LOAD_BANKS);
        switch (what)
        {
        case 0: // none
        {
            return;
        }
        case 1: {
            Array arr = GLOBAL_GET(SPECIFIED_BANKS);
            for (auto bank : arr)
            {
                Ref<FmodBank> loaded_bank =
                    ResourceLoader::get_singleton()->load(bank, "FmodBank", ResourceLoader::CACHE_MODE_REUSE);
                if (loaded_bank.is_valid())
                {
                    registered_banks.push_back(loaded_bank);
                }
            }
        }
        break;
        case 2: // all
            _load_all_banks_in(GLOBAL_GET(BANK_DIRECTORY));
            break;
        default:
            break;
        }
    }
}
void FmodAudioServer::_reload_start_up_banks()
{
    for (auto bank : registered_banks)
    {
        bank->reload();
    }
}
void FmodAudioServer::_unload_start_up_banks()
{
    lock();
    if (start_up_banks_loaded)
    {
        print_verbose("Unloading Start Up Banks");
        registered_banks.clear();
        start_up_banks_loaded = false;
    }
    unlock();
}

void FmodAudioServer::unload_banks()
{
    lock();
    if (!FMOD_Studio_System_IsValid(studio_system))
    {
        unlock();
        return;
    }
    FMOD_Studio_System_UnloadAll(studio_system);
    registered_banks.clear();
    start_up_banks_loaded = false;
    print_verbose("Unloading all banks");
    unlock();
}

bool FmodAudioServer::is_live_update_connected() const
{
    return live_update_connected;
}
FMOD_SYSTEM *FmodAudioServer::get_core()
{
    return core_system;
}
const FMOD_SYSTEM *FmodAudioServer::get_core() const
{
    return core_system;
}
FMOD_STUDIO_SYSTEM *FmodAudioServer::get_studio()
{
    return studio_system;
}
const FMOD_STUDIO_SYSTEM *FmodAudioServer::get_studio() const
{
    return studio_system;
}
FMOD_STUDIO_EVENTINSTANCE *FmodAudioServer::create_instance(const Vector4i &p_guid) const
{
    FMOD_GUID guid = cast_to_fmod_guid(p_guid);
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_System_GetEventByID(studio_system, &guid, &description);
    FMOD_STUDIO_EVENTINSTANCE *event;
    FMOD_Studio_EventDescription_CreateInstance(description, &event);
    return event;
}

void FmodAudioServer::play_one_shot(const Vector4i &p_guid, const Vector3 &p_position)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);
    FMOD_3D_ATTRIBUTES attr;
    attr.forward = {0, 0, -1};
    attr.up = {0, 1, 0};
    attr.position = to_fmod_vector(p_position);
    attr.velocity = {0, 0, 0};
    FMOD_Studio_EventInstance_Set3DAttributes(event, &attr);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}

void FmodAudioServer::play_one_shot(const String &p_path, const Vector3 &p_position)
{
    play_one_shot(path_to_guid(p_path), p_position);
}
void FmodAudioServer::play_one_shot_attached(const Vector4i &p_guid, Node3D *p_node, bool p_non_rigid_body_velocity)
{
    _play_one_shot_attached(p_guid, p_node, p_non_rigid_body_velocity);
}
void FmodAudioServer::play_one_shot_attached(const String &p_path, Node3D *p_node, bool p_non_rigid_body_velocity)
{
    play_one_shot_attached(path_to_guid(p_path), p_node, p_non_rigid_body_velocity);
}
void FmodAudioServer::play_one_shot_attached(const Vector4i &p_guid, Node2D *p_node, bool p_non_rigid_body_velocity)
{
    _play_one_shot_attached(p_guid, p_node, p_non_rigid_body_velocity);
}
void FmodAudioServer::play_one_shot_attached(const String &p_path, Node2D *p_node, bool p_non_rigid_body_velocity)
{
    play_one_shot_attached(path_to_guid(p_path), p_node, p_non_rigid_body_velocity);
}

void FmodAudioServer::play_one_shot_attached(const Vector4i &p_guid, RigidBody3D *p_rigid_body3d)
{
    _play_one_shot_attached(p_guid, p_rigid_body3d);
}
void FmodAudioServer::play_one_shot_attached(const String &p_path, RigidBody3D *p_rigid_body3d)
{
    play_one_shot_attached(path_to_guid(p_path), p_rigid_body3d);
}
void FmodAudioServer::play_one_shot_attached(const Vector4i &p_guid, RigidBody2D *p_rigid_body2d)
{
    _play_one_shot_attached(p_guid, p_rigid_body2d);
}
void FmodAudioServer::play_one_shot_attached(const String &p_path, RigidBody2D *p_rigid_body2d)
{
    play_one_shot_attached(path_to_guid(p_path), p_rigid_body2d);
}
bool FmodAudioServer::any_sample_data_loading() const
{
    int count;
    FMOD_Studio_System_GetBankCount(studio_system, &count);
    int retrieved;
    FMOD_STUDIO_BANK **banks = memnew_arr(FMOD_STUDIO_BANK *, count);

    FMOD_Studio_System_GetBankList(studio_system, banks, count, &retrieved);
    bool loading = false;
    for (int i = 0; i < retrieved; i++)
    {
        FMOD_STUDIO_LOADING_STATE state;
        FMOD_Studio_Bank_GetSampleLoadingState(banks[i], &state);
        loading |= (state == FMOD_STUDIO_LOADING_STATE_LOADING);
    }
    memdelete_arr(banks);
    return loading;
}
void FmodAudioServer::attach_instance(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                      bool p_non_rigid_body_velocity)
{
    _attach_instance(p_node, p_event, p_non_rigid_body_velocity);
}
void FmodAudioServer::attach_instance(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    _attach_instance(p_node, p_event);
}
void FmodAudioServer::attach_instance(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                      bool p_non_rigid_body_velocity)
{
    _attach_instance(p_node, p_event, p_non_rigid_body_velocity);
}
void FmodAudioServer::attach_instance(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    _attach_instance(p_node, p_event);
}

void FmodAudioServer::detach_instance(FMOD_STUDIO_EVENTINSTANCE *p_instance)
{
    lock();
    int i = _find_instance(p_instance);
    if (i != -1)
    {
        instances[i] = instances[instances.size() - 1];
        instances.remove_at(instances.size() - 1);
        return;
    }
    unlock();
}
void FmodAudioServer::wait_for_all_sample_loading()
{
    FMOD_Studio_System_FlushSampleLoading(studio_system);
}

Vector4i FmodAudioServer::path_to_guid(const String &p_path) const
{
    FMOD_GUID guid;
    FMOD_Studio_System_LookupID(studio_system, p_path.utf8().ptr(), &guid);
    return cast_to_vector4i(guid);
}
FMOD_STUDIO_EVENTDESCRIPTION *FmodAudioServer::get_event_description(const String &p_path) const
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_System_GetEvent(studio_system, p_path.utf8().ptr(), &description);
    return description;
}
FMOD_STUDIO_EVENTDESCRIPTION *FmodAudioServer::get_event_description(const Vector4i &p_guid) const
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_System_GetEventByID(studio_system, reinterpret_cast<const FMOD_GUID *>(&p_guid), &description);
    return description;
}
void FmodAudioServer::pause_all_events(bool p_pause)
{
    FMOD_STUDIO_BUS *bus;
    if (FMOD_Studio_System_GetBus(studio_system, "bus:/", &bus) == FMOD_RESULT::FMOD_OK)
    {
        FMOD_Studio_Bus_SetPaused(bus, p_pause);
    }
}
bool FmodAudioServer::is_muted() const
{
    return muted;
}
void FmodAudioServer::set_muted(bool p_muted)
{
    FMOD_STUDIO_BUS *bus;
    muted = p_muted;
    if (FMOD_Studio_System_GetBus(studio_system, "bus:/", &bus) == FMOD_RESULT::FMOD_OK)
    {
        FMOD_Studio_Bus_SetMute(bus, p_muted);
    }
}
FMOD_STUDIO_BUS *FmodAudioServer::get_bus(const String &p_path) const
{
    FMOD_STUDIO_BUS *bus;
    FMOD_Studio_System_GetBus(studio_system, p_path.utf8().ptr(), &bus);
    return bus;
}
FMOD_STUDIO_VCA *FmodAudioServer::get_vca(const String &p_path) const
{
    FMOD_STUDIO_VCA *vca;
    FMOD_Studio_System_GetVCA(studio_system, p_path.utf8().ptr(), &vca);
    return vca;
}
FMOD_STUDIO_BUS *FmodAudioServer::get_bus(const Vector4i &p_guid) const
{
    FMOD_STUDIO_BUS *bus;
    FMOD_Studio_System_GetBusByID(studio_system, reinterpret_cast<const FMOD_GUID *>(&p_guid), &bus);
    return bus;
}
FMOD_STUDIO_VCA *FmodAudioServer::get_vca(const Vector4i &p_guid) const
{
    FMOD_STUDIO_VCA *vca;
    FMOD_Studio_System_GetVCAByID(studio_system, reinterpret_cast<const FMOD_GUID *>(&p_guid), &vca);
    return vca;
}

bool FmodAudioServer::has_bank_loaded(const String &p_bank_path_or_guid) const
{
    FMOD_STUDIO_BANK *bank;
    // TODO Only works synchronously
    return FMOD_Studio_System_GetBank(studio_system, p_bank_path_or_guid.utf8().ptr(), &bank) == FMOD_OK;
}

void FmodAudioServer::set_listener_location(Node2D *p_node, int p_listener_index, Node2D *p_attenuation_object)
{
    _set_listener_location(p_node, p_listener_index, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(RigidBody2D *p_rigid_body2d, int p_listener_index,
                                            Node2D *p_attenuation_object)
{
    _set_listener_location(p_rigid_body2d, p_listener_index, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(Node3D *p_node, int p_listener_index, Node3D *p_attenuation_object)
{
    _set_listener_location(p_node, p_listener_index, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(RigidBody3D *p_rigid_body3d, int p_listener_index,
                                            Node3D *p_attenuation_object)
{
    _set_listener_location(p_rigid_body3d, p_listener_index, p_attenuation_object);
}

extern "C"
{
    GDE_EXPORT int get_fmod_core(FMOD_SYSTEM **p_core)
    {
        *p_core = FmodAudioServer::get_singleton()->get_core();
        if (p_core == nullptr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    GDE_EXPORT int get_fmod_studio(FMOD_STUDIO_SYSTEM **p_studio)
    {
        *p_studio = FmodAudioServer::get_singleton()->get_studio();
        if (p_studio == nullptr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

#define FS FmodAudioServer::get_singleton()
    FMOD_STUDIO_EVENTINSTANCE *create_instance(const Vector4i p_guid)
    {
        return FS->create_instance(p_guid);
    }

    GDE_EXPORT void play_one_shot_by_id(const Vector4i &p_guid, const godot::Vector3 &p_position = Vector3())
    {
        FS->play_one_shot(p_guid, p_position);
    }
    GDE_EXPORT void play_one_shot(const char *p_path, const Vector3 p_position = Vector3())
    {
        FS->play_one_shot(p_path, p_position);
    }
    GDE_EXPORT void play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node,
                                                    bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_attached(p_guid, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_3d_attached(const char *p_path, Node3D *p_node,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_attached(p_path, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node,
                                                    bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_attached(p_guid, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_2d_attached(const char *p_path, Node2D *p_node,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_attached(p_path, p_node, p_non_rigid_body_velocity);
    }

    GDE_EXPORT void play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d)
    {
        FS->play_one_shot_attached(p_guid, p_rigid_body3d);
    }
    GDE_EXPORT void play_one_shot_rigid_body3d_attached(const char *p_path, RigidBody3D *p_rigid_body3d)
    {
        FS->play_one_shot_attached(p_path, p_rigid_body3d);
    }
    GDE_EXPORT void play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d)
    {
        FS->play_one_shot_attached(p_guid, p_rigid_body2d);
    }
    GDE_EXPORT void play_one_shot_rigid_body2d_attached(const char *p_path, RigidBody2D *p_rigid_body2d)
    {
        FS->play_one_shot_attached(p_path, p_rigid_body2d);
    }
    GDE_EXPORT bool any_sample_data_loading()
    {
        return FS->any_sample_data_loading();
    }
    GDE_EXPORT void attach_instance_to_node3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->attach_instance(p_node, p_event, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void attach_instance_to_rigid_body3d(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        FS->attach_instance(p_node, p_event);
    }
    GDE_EXPORT void attach_instance_to_node2D(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->attach_instance(p_node, p_event, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void attach_instance_to_rigid_body2d(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        FS->attach_instance(p_node, p_event);
    }
    GDE_EXPORT void detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        FS->detach_instance(p_event);
    }

    GDE_EXPORT FMOD_GUID path_to_guid(const char *p_path)
    {
        return cast_to_fmod_guid(FmodAudioServer::get_singleton()->path_to_guid(p_path));
    }
    GDE_EXPORT FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(const char *p_path)
    {
        return FS->get_event_description(p_path);
    }

    GDE_EXPORT FMOD_STUDIO_EVENTDESCRIPTION *get_event_description_by_id(Vector4i p_guid)
    {
        return FS->get_event_description(p_guid);
    }
    GDE_EXPORT void pause_all_events(bool p_pause)
    {
        FS->pause_all_events(p_pause);
    }

    GDE_EXPORT bool is_muted()
    {
        return FS->is_muted();
    }
    GDE_EXPORT void set_muted(bool p_muted)
    {
        FS->set_muted(p_muted);
    }

    GDE_EXPORT FMOD_STUDIO_BUS *get_bus(const char *p_path)
    {
        return FS->get_bus(p_path);
    }
    GDE_EXPORT FMOD_STUDIO_VCA *get_vca(const char *p_path)
    {
        return FS->get_vca(p_path);
    }
    GDE_EXPORT void unload_banks()
    {
        FS->unload_banks();
    }
    GDE_EXPORT bool has_bank_loaded(const char *&p_bank_name)
    {
        return FS->has_bank_loaded(p_bank_name);
    }

    GDE_EXPORT void set_listener_2d_rigidbody_location(int p_listener_index, void *p_rigid_body2d,
                                                       void *p_attenuation_object = nullptr)
    {
        FS->set_listener_location(static_cast<RigidBody2D *>(internal::get_object_instance_binding(p_rigid_body2d)),
                                  p_listener_index,
                                  static_cast<Node2D *>(internal::get_object_instance_binding(p_attenuation_object)));
    }
    GDE_EXPORT void set_listener_2d_location(void *p_node, int p_listener_index, void *p_attenuation_object = nullptr)
    {
        FS->set_listener_location(static_cast<Node2D *>(internal::get_object_instance_binding(p_node)),
                                  p_listener_index,
                                  static_cast<Node2D *>(internal::get_object_instance_binding(p_attenuation_object)));
    }

    GDE_EXPORT void set_listener_3d_rigidbody_location(int p_listener_index, void *p_rigid_body,
                                                       void *p_attenuation_object = nullptr)
    {
        FS->set_listener_location(static_cast<RigidBody3D *>(internal::get_object_instance_binding(p_rigid_body)),
                                  p_listener_index,
                                  static_cast<Node3D *>(internal::get_object_instance_binding(p_attenuation_object)));
    }
    GDE_EXPORT void set_listener_3d_location(int p_listener_index, void *p_node, void *p_attenuation_object = nullptr)
    {
        FS->set_listener_location(static_cast<RigidBody3D *>(internal::get_object_instance_binding(p_node)),
                                  p_listener_index,
                                  static_cast<Node3D *>(internal::get_object_instance_binding(p_attenuation_object)));
    }
}
} // namespace FmodGodot
#undef SERVER
