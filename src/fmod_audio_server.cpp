#include "fmod_audio_server.h"
#include "classes/global_constants.hpp"
#include "classes/os.hpp"
#include "classes/resource_loader.hpp"
#include "core/error_macros.hpp"
#include "core/print_string.hpp"
#include "fmod_common.h"
#include "fmod_globals.h"
#include "fmod_string_names.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include "variant/utility_functions.hpp"
#include <classes/dir_access.hpp>
#include <classes/os.hpp>
#include <fmod_errors.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#ifdef TOOLS_ENABLED
#include <classes/editor_interface.hpp>
#include <classes/editor_settings.hpp>
#endif
using namespace std;
using namespace godot;
namespace FmodGodot
{
namespace
{
void get_live_update_settings(FmodAudioServer::InitSettings &settings)
{

    auto pj = ProjectSettings::get_singleton();
    settings.live_update =
        static_cast<FmodAudioServer::LiveUpdate>(static_cast<int>(pj->get_setting_with_override(LIVE_UPDATE)));
    settings.live_update_port = pj->get_setting_with_override(LIVE_UPDATE_PORT);
}
void get_debug_settings(FmodAudioServer::InitSettings &settings)
{
    auto pj = ProjectSettings::get_singleton();
    settings.logging_level = pj->get_setting_with_override(LOGGING_LEVEL);
    settings.debug_type = pj->get_setting_with_override(DEBUG_TYPE);
    settings.debug_display = pj->get_setting_with_override(DEBUG_DISPLAY);
}

} // namespace
FmodAudioServer::InitSettings FmodAudioServer::get_fmod_settings()
{
    FmodAudioServer::InitSettings settings;
    auto pj = ProjectSettings::get_singleton();
    settings.sample_rate = pj->get_setting_with_override(SAMPLE_RATE);
    settings.dspbuffer_length = pj->get_setting_with_override(BUFFER_LENGTH);
    settings.dspbuffer_count = pj->get_setting_with_override(BUFFER_COUNT);
    settings.software_channels = pj->get_setting_with_override(REAL_COUNT);
    settings.virtual_channels = pj->get_setting_with_override(VIRTUAL_COUNT);
    settings.encryption_key = pj->get_setting_with_override(ENCRYPTION_KEY);
    settings.rolloff_scale = pj->get_setting_with_override(ROLLOFF_SCALE);
    settings.doppler_scale = pj->get_setting_with_override(DOPPLER_SCALE);
    settings.distance_factor = pj->get_setting_with_override(DISTANCE_FACTOR);
    get_live_update_settings(settings);
    get_debug_settings(settings);
    return settings;
}

FMOD_RESULT fmod_debug_callback(FMOD_DEBUG_FLAGS flags, const char *file, int line, const char *func, const char *message)
{
    if (flags & FMOD_DEBUG_LEVEL_ERROR)
    {
        _err_print_error(func, file, line, message, true, true);
    }
    else if (flags & FMOD_DEBUG_LEVEL_WARNING)
    {
        _err_print_error(func, file, line, message, true, true);
    }
    else
    {
        print_line(message);
    }
    return FMOD_OK;
}

FMOD_RESULT godot_file_error_to_fmod_file_error(Error err)
{
    switch (err)
    {
    case OK:
        return FMOD_OK;
    case ERR_FILE_NOT_FOUND:
        return FMOD_ERR_FILE_NOTFOUND;
    case ERR_FILE_BAD_PATH:
        return FMOD_ERR_FILE_BAD;
    case ERR_FILE_CANT_READ:
        return FMOD_ERR_FILE_COULDNOTSEEK;
    case ERR_FILE_EOF:
        return FMOD_ERR_FILE_EOF;
    default:
        return FMOD_ERR_FILE_BAD;
    }
}
FMOD_RESULT open_callback(const char *name, unsigned int *filesize, void **handle, void *userdata)
{
    auto file = FileAccess::open(String(name), FileAccess::ModeFlags::READ);
    if (FileAccess::get_open_error() != OK)
    {
        return godot_file_error_to_fmod_file_error(FileAccess::get_open_error());
    }
    file->reference();
    *handle = file.ptr();
    *filesize = file->get_length();
    return godot_file_error_to_fmod_file_error(file->get_error());
}
FMOD_RESULT close_callback(void *handle, void *userdata)
{
    ((FileAccess *)handle)->close();
    ((FileAccess *)handle)->unreference();
    FMOD_RESULT result = godot_file_error_to_fmod_file_error(((FileAccess *)handle)->get_error());
    memdelete((FileAccess *)handle);
    return result;
}
FMOD_RESULT seek_callback(void *handle, unsigned int pos, void *userdata)
{
    ((FileAccess *)handle)->seek(pos);
    return godot_file_error_to_fmod_file_error(((FileAccess *)handle)->get_error());
}
FMOD_RESULT read_callback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
    *bytesread = ((FileAccess *)handle)->get_buffer((uint8_t *)buffer, sizebytes);
    return godot_file_error_to_fmod_file_error(((FileAccess *)handle)->get_error());
}

#pragma region Server Functionality
FmodAudioServer *FmodAudioServer::singleton = nullptr;

FmodAudioServer::FmodAudioServer()
{
    studio_system = nullptr;
    core_system = nullptr;
}
FmodAudioServer::~FmodAudioServer()
{
    finish();
}

FMOD_RESULT FmodAudioServer::init_with_project_settings()
{
    return init(get_fmod_settings());
}

FMOD_RESULT FmodAudioServer::init(const InitSettings &p_settings)
{
    mutex.instantiate();
    thread.instantiate();
    exit_thread = false;
    thread_exited = false;
    FMOD_STUDIO_INITFLAGS studio_init = FMOD_STUDIO_INIT_NORMAL;
    switch (p_settings.live_update)
    {
    case DISABLED: // disabled
        studio_init = FMOD_STUDIO_INIT_NORMAL;
        break;
    case ENABLED:
        studio_init = FMOD_STUDIO_INIT_LIVEUPDATE;
        print_line("Live Update");
        print_line(p_settings.live_update_port);
    case DEV_ONLY: // dev
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
    FMOD_RESULT result;
    result = FMOD_Debug_Initialize(p_settings.logging_level | p_settings.debug_type | p_settings.debug_display,
                                   FMOD_DEBUG_MODE_CALLBACK, fmod_debug_callback, nullptr);

    result = FMOD_Studio_System_Create(&studio_system, FMOD_VERSION);
    result = FMOD_Studio_System_GetCoreSystem(studio_system, &core_system);

    // Core Settings
    FMOD_ADVANCEDSETTINGS fmod_settings;
    fmod_settings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
    FMOD_System_GetAdvancedSettings(core_system, &fmod_settings);
    fmod_settings.profilePort = p_settings.live_update_port;
    FMOD_System_SetAdvancedSettings(core_system, &fmod_settings);
    FMOD_System_SetSoftwareFormat(core_system, p_settings.sample_rate, FMOD_SPEAKERMODE_DEFAULT, 0);
    FMOD_System_SetDSPBufferSize(core_system, p_settings.dspbuffer_length, p_settings.dspbuffer_count);
    FMOD_System_SetSoftwareChannels(core_system, p_settings.software_channels);
    FMOD_System_Set3DSettings(core_system, p_settings.doppler_scale, p_settings.distance_factor,
                              p_settings.rolloff_scale);
    FMOD_System_SetFileSystem(core_system, open_callback, close_callback, read_callback, seek_callback, NULL, NULL,
                              2048);
    // studio settings
    FMOD_STUDIO_ADVANCEDSETTINGS studio_settings;
    studio_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);
    FMOD_Studio_System_GetAdvancedSettings(studio_system, &studio_settings);
    studio_settings.cbsize = sizeof(FMOD_STUDIO_ADVANCEDSETTINGS);
    studio_settings.encryptionkey = p_settings.encryption_key.utf8();
    FMOD_Studio_System_SetAdvancedSettings(studio_system, &studio_settings);

    result =
        FMOD_Studio_System_Initialize(studio_system, p_settings.virtual_channels, studio_init, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        UtilityFunctions::printerr("Init FmodAudioServer Error", FMOD_ErrorString(result));
    }
    initialized = true;
    thread->start(callable_mp(this, &FmodAudioServer::thread_func), Thread::Priority::PRIORITY_NORMAL);
    return result;
}

void FmodAudioServer::_physics_process()
{
    if (thread_exited)
    {
        return;
    }
    lock();
    for (int i = 0; i < instances.size(); i++)
    {
        if (!FMOD_Studio_EventInstance_IsValid(instances[i].instance))
        {
            instances.write[i] = instances[instances.size() - 1];
            instances.remove_at(instances.size() - 1);
            continue;
        }
        FMOD_3D_ATTRIBUTES attributes;
        Vector3 velocity;
        switch (instances[i].attachment)
        {
        case Attachment::NODE2D: {
            FMOD_Studio_EventInstance_Get3DAttributes(instances[i].instance, &attributes);
            Vector2 pos = instances[i].node2D->get_global_position();
            velocity = (Vector3(pos.x, pos.y, 0) - instances[i].lastFramePosition) /
                       instances[i].node2D->get_physics_process_delta_time();
            attributes.velocity = to_fmod_vector(velocity);
            instances.write[i].lastFramePosition = Vector3(pos.x, pos.y, 0);
            FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
            break;
        }
        case Attachment::NODE3D: {
            FMOD_Studio_EventInstance_Get3DAttributes(instances[i].instance, &attributes);
            velocity = (instances[i].node3D->get_global_position() - instances[i].lastFramePosition) /
                       instances[i].node3D->get_physics_process_delta_time();
            attributes.velocity = to_fmod_vector(velocity);
            instances.write[i].lastFramePosition = instances[i].node3D->get_global_position();
            FMOD_Studio_EventInstance_Set3DAttributes(instances[i].instance, &attributes);
            break;
        }
        default:
            break;
        }
    }
    unlock();
}
void FmodAudioServer::thread_func()
{
    thread->set_thread_safety_checks_enabled(false);
    SceneTree *tree = cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    while (!tree)
    {
        OS::get_singleton()->delay_usec(20);
        tree = cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
    }
    Callable physics_callback = callable_mp(this, &FmodAudioServer::_physics_process);
    tree->connect("physics_frame", physics_callback);
    while (!exit_thread)
    {
        FMOD_3D_ATTRIBUTES attributes;
        lock();
        for (int i = 0; i < instances.size(); i++)
        {
            if (!FMOD_Studio_EventInstance_IsValid(instances[i].instance))
            {
                instances.write[i] = instances[instances.size() - 1];
                instances.remove_at(instances.size() - 1);
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
    if (UtilityFunctions::is_instance_id_valid(tree->get_instance_id()))
    {
        tree->disconnect("physics_frame", physics_callback);
    }
    FMOD_Studio_System_Release(studio_system);
}

int FmodAudioServer::find_instance(FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    int instance_index = -1;
    for (int i = 0; i < instances.size(); i++)
    {
        if (instances[i].instance == p_event)
        {
            instance_index = i;
        }
    }
    return instance_index;
}

void FmodAudioServer::unlock()
{
    if (!thread.is_valid() || !mutex.is_valid())
    {
        return;
    }

    mutex->unlock();
}

void FmodAudioServer::lock()
{
    if (!thread.is_valid() || !mutex.is_valid())
    {
        return;
    }

    mutex->lock();
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
    mutex.unref();
}

#pragma endregion
void FmodAudioServer::_bind_methods()
{
    BIND_ENUM_CONSTANT(DISABLED);
    BIND_ENUM_CONSTANT(ENABLED);
    BIND_ENUM_CONSTANT(DEV_ONLY);

    ClassDB::bind_method(D_METHOD("unload_banks"), &FmodAudioServer::unload_banks);

    BIND_BOOL_PROPERTY(muted);
    ClassDB::bind_method(D_METHOD("play_one_shot_by_id", "guid", "position"), &FmodAudioServer::play_one_shot_by_id,
                         DEFVAL(Vector3(0, 0, 0)));
    ClassDB::bind_static_method("FmodAudioServer", D_METHOD("get_singleton"), &FmodAudioServer::get_singleton);
    ClassDB::bind_method(D_METHOD("play_one_shot_by_path", "path", "position"), &FmodAudioServer::play_one_shot_by_path,
                         DEFVAL(Vector3(0, 0, 0)));
    ClassDB::bind_method(D_METHOD("play_one_shot_3d_attached_by_id", "guid", "node", "non_rigid_body_velocity"),
                         &FmodAudioServer::play_one_shot_3d_attached_by_id, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("play_one_shot_3d_attached_by_path", "path", "node", "non_rigid_body_velocity"),
                         &FmodAudioServer::play_one_shot_3d_attached_by_path, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("play_one_shot_2d_attached_by_id", "guid", "node", "non_rigid_body_velocity"),
                         &FmodAudioServer::play_one_shot_2d_attached_by_id, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("play_one_shot_2d_attached_by_path", "path", "node", "non_rigid_body_velocity"),
                         &FmodAudioServer::play_one_shot_2d_attached_by_path, DEFVAL(false));

    ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body3d_attached_by_id", "guid", "rigid_body3d"),
                         &FmodAudioServer::play_one_shot_rigid_body3d_attached_by_id);
    ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body3d_attached_by_path", "path", "rigid_body3d"),
                         &FmodAudioServer::play_one_shot_rigid_body3d_attached_by_path);
    ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body2d_attached_by_id", "guid", "rigid_body2d"),
                         &FmodAudioServer::play_one_shot_rigid_body2d_attached_by_id);
    ClassDB::bind_method(D_METHOD("play_one_shot_rigid_2d_attached_by_path", "path", "rigid_body2d"),
                         &FmodAudioServer::play_one_shot_rigid_body2d_attached_by_path);
    ClassDB::bind_method(D_METHOD("any_sample_data_loading"), &FmodAudioServer::any_sample_data_loading);
    // ClassDB::bind_method(D_METHOD("attach_instance_to_node3d", "node",
    // "instance", "non_rigid_body_velocity"),
    // &FmodAudioServer::attach_instance_to_node3d); void
    // attach_instance_to_node3D(Node3D * p_node, FMOD_STUDIO_EVENTINSTANCE *
    // p_instance, bool p_non_rigid_body_velocity = false); void
    // attach_instance_to_rigid_body_3D(RigidBody3D * p_node,
    // FMOD_STUDIO_EVENTINSTANCE * p_instance); void
    // attach_instance_to_node2D(Node2D * p_node, FMOD_STUDIO_EVENTINSTANCE *
    // p_instance, bool p_non_rigid_body_velocity = false); void
    // attach_instance_to_rigid_body_2D(RigidBody2D * p_node,
    // FMOD_STUDIO_EVENTINSTANCE * p_instance);

    // void detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE * p_instance);

    ClassDB::bind_method(D_METHOD("path_to_guid", "p_path"), &FmodAudioServer::path_to_guid);
    // FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(String p_path);
    // FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(Vector4i p_guid);
    // void pause_all_events(bool p_pause);

    // Studio::Bus *get_bus(String p_path);
    // Studio::VCA *get_vca(String p_path);

    ClassDB::bind_method(D_METHOD("load_bank", "bank_name", "load_samples"), &FmodAudioServer::load_bank,
                         DEFVAL(false));
    BIND_METHOD(has_bank_loaded, "bank_name")

    // bool have_all_banks_loaded();
    BIND_METHOD(have_all_banks_loaded);
    // set_listener_location
    BIND_METHOD(set_listener_2d_location, "listener_index", "node", "attenuation_object");
    BIND_METHOD(set_listener_3d_location, "listener_index", "node", "attenuation_object");
    BIND_METHOD(set_listener_2d_rigidbody_location, "listener_index", "rigidbody", "attenuation_object");
    BIND_METHOD(set_listener_3d_rigidbody_location, "listener_index", "rigidbody", "attenuation_object");
}

FmodAudioServer *FmodAudioServer::get_singleton()
{
    return singleton;
}
void FmodAudioServer::load_start_up_banks()
{
    Array arr;
    int what = ProjectSettings::get_singleton()->get_setting_with_override(LOAD_BANKS);
    arr = ProjectSettings::get_singleton()->get_setting_with_override(SPECIFIED_BANKS);
    switch (what)
    {
    case 0: // none
    {
        return;
    }
    case 1: // specified should be loaded by project settings?
        break;
    case 2: // all
    {
        auto dir = DirAccess::open(ProjectSettings::get_singleton()->get_setting_with_override(BANK_DIRECTORY));
        if (DirAccess::get_open_error() != godot::OK)
        {
            print_error(DirAccess::get_open_error());
            return;
        }
        for (auto file : dir->get_files())
        {
            if (!file.ends_with(".bank"))
            {
                continue;
            }
            ProjectSettings *ps = ProjectSettings::get_singleton();
            start_up_banks.push_back(ResourceLoader::get_singleton()->load(
                ps->get_setting_with_override(BANK_DIRECTORY).stringify() + "/" + file, String(),
                godot::ResourceLoader::CACHE_MODE_REPLACE));
        }
        break;
    }
    default:
        break;
    }
}

#pragma region server api

int FmodAudioServer::load_bank_by_file(const String &path, bool loadSamples)
{
    FMOD_STUDIO_BANK *bank;
    FMOD_RESULT result =
        FMOD_Studio_System_LoadBankFile(studio_system, path.utf8().ptr(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    if (result == FMOD_OK)
    {
        if (loadSamples)
        {
            FMOD_Studio_Bank_LoadSampleData(bank);
        }
    }
    else if (result == FMOD_ERR_EVENT_ALREADY_LOADED && loadSamples)
    {
        FMOD_Studio_Bank_LoadSampleData(bank);
    }
    else
    {
        godot::_err_print_error("load_bank", __FILE__, __LINE__,
                                "Couldn't load bank:" + path + " error:" + FMOD_ErrorString(result), true, true);
    }

    return result;
}
int FmodAudioServer::load_bank(const String &p_name, bool loadSamples)
{

    if (p_name.begins_with("bank:/"))
    {

        return load_bank_by_file(
            ProjectSettings::get_singleton()->get_setting_with_override(BANK_DIRECTORY).stringify() + p_name.substr(6));
    }
    else if (p_name.ends_with(".bank"))
    {
        return load_bank_by_file(p_name, loadSamples);
    }
    return FMOD_ERR_FILE_NOTFOUND;
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
    unlock();
}
void FmodAudioServer::get_core_ref(FMOD_SYSTEM **core)
{
    *core = this->core_system;
}
void FmodAudioServer::get_studio_ref(FMOD_STUDIO_SYSTEM **studio)
{
    *studio = this->studio_system;
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
FMOD_STUDIO_EVENTINSTANCE *FmodAudioServer::create_instance(const Vector4i p_guid) const
{
    FMOD_GUID guid = cast_to_fmod_guid(p_guid);
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_System_GetEventByID(studio_system, &guid, &description);
    FMOD_STUDIO_EVENTINSTANCE *event;
    FMOD_Studio_EventDescription_CreateInstance(description, &event);
    return event;
}

void FmodAudioServer::play_one_shot_by_id(const Vector4i p_guid, const Vector3 p_position) const
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
void FmodAudioServer::play_one_shot_by_path(const String &p_path, const Vector3 p_position) const
{
    play_one_shot_by_id(path_to_guid(p_path), p_position);
}
void FmodAudioServer::play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node,
                                                      bool p_non_rigid_body_velocity)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);

    attach_instance_to_node3d(p_node, event);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}
void FmodAudioServer::play_one_shot_3d_attached_by_path(const String &p_path, Node3D *p_node,
                                                        bool p_non_rigid_body_velocity)
{
    play_one_shot_3d_attached_by_id(path_to_guid(p_path), p_node);
}
void FmodAudioServer::play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node,
                                                      bool p_non_rigid_body_velocity)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);

    attach_instance_to_node2D(p_node, event);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}
void FmodAudioServer::play_one_shot_2d_attached_by_path(const String &p_path, Node2D *p_node,
                                                        bool p_non_rigid_body_velocity)
{
    play_one_shot_2d_attached_by_id(path_to_guid(p_path), p_node);
}

void FmodAudioServer::play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);

    attach_instance_to_rigid_body3d(p_rigid_body3d, event);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}
void FmodAudioServer::play_one_shot_rigid_body3d_attached_by_path(const String &p_path, RigidBody3D *p_rigid_body3d)
{
    play_one_shot_rigid_body3d_attached_by_id(path_to_guid(p_path), p_rigid_body3d);
}
void FmodAudioServer::play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);

    attach_instance_to_rigid_body2d(p_rigid_body2d, event);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}
void FmodAudioServer::play_one_shot_rigid_body2d_attached_by_path(const String &p_path, RigidBody2D *p_rigid_body2d)
{
    play_one_shot_rigid_body2d_attached_by_id(path_to_guid(p_path), p_rigid_body2d);
}
bool FmodAudioServer::any_sample_data_loading()
{
    int count;
    FMOD_Studio_System_GetBankCount(studio_system, &count);
    int retrieved;
    FMOD_STUDIO_BANK **banks;

    FMOD_Studio_System_GetBankList(studio_system, banks, count, &retrieved);
    bool loading = false;
    for (int i = 0; i < retrieved; i++)
    {
        FMOD_STUDIO_LOADING_STATE state;
        FMOD_Studio_Bank_GetSampleLoadingState(banks[i], &state);
        loading |= (state == FMOD_STUDIO_LOADING_STATE_LOADING);
    }
    return loading;
}

void FmodAudioServer::attach_instance_to_node3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_instance,
                                                bool p_non_rigid_body_velocity)
{
    _attach_instance_3d(p_node, p_instance, NODE3D, false);
}
void FmodAudioServer::attach_instance_to_rigid_body3d(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_instance)
{
    _attach_instance_3d<RigidBody3D>(p_node, p_instance, RIGIDBODY3D, false);
}
void FmodAudioServer::attach_instance_to_node2D(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_instance,
                                                bool p_non_rigid_body_velocity)
{
    _attach_instance_2d<Node2D>(p_node, p_instance, NODE2D, p_non_rigid_body_velocity);
}
void FmodAudioServer::attach_instance_to_rigid_body2d(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_instance)
{
    _attach_instance_2d<RigidBody2D>(p_node, p_instance, RIGIDBODY2D, false);
}

void FmodAudioServer::detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE *p_instance)
{
    lock();
    int i = find_instance(p_instance);
    for (int i = 0; i < instances.size(); i++)
    {
        if (instances[i].instance == p_instance)
        {
            instances.write[i] = instances[instances.size() - 1];
            instances.remove_at(instances.size() - 1);
            return;
        }
    }
    unlock();
}

Vector4i FmodAudioServer::path_to_guid(const String &p_path) const
{
    FMOD_GUID guid;
    FMOD_Studio_System_LookupID(studio_system, p_path.utf8(), &guid);
    return cast_to_vector4i(guid);
}
FMOD_STUDIO_EVENTDESCRIPTION *FmodAudioServer::get_event_description(const String &p_path) const
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_Studio_System_GetEvent(studio_system, p_path.utf8(), &description);
    return description;
}
FMOD_STUDIO_EVENTDESCRIPTION *FmodAudioServer::get_event_description(Vector4i p_guid) const
{
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_GUID guid = cast_to_fmod_guid(p_guid);
    FMOD_Studio_System_GetEventByID(studio_system, &guid, &description);
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
    FMOD_Studio_System_GetBus(studio_system, p_path.utf8(), &bus);
    return bus;
}
FMOD_STUDIO_VCA *FmodAudioServer::get_vca(const String &p_path) const
{
    FMOD_STUDIO_VCA *vca;
    FMOD_Studio_System_GetVCA(studio_system, p_path.utf8(), &vca);
    return vca;
}
bool FmodAudioServer::has_bank_loaded(const String &p_bankName) const
{
    FMOD_STUDIO_BANK *bank;
    // TODO Only works synchronously
    return FMOD_Studio_System_GetBank(studio_system, p_bankName.utf8(), &bank) == FMOD_OK;
}

bool FmodAudioServer::have_all_banks_loaded() const
{
    // TODO works because banks are loaded syncronously. but should be more
    // robust.
    return true;
}

void FmodAudioServer::set_listener_location(Node2D *p_node, Node2D *p_attenuation_object)
{
    set_listener_2d_location(0, p_node, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(int p_listener_index, Node2D *p_node, Node2D *p_attenuation_object)
{
    set_listener_2d_location(p_listener_index, p_node, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(RigidBody2D *p_rigid_body2d, Node2D *p_attenuation_object)
{
    set_listener_2d_rigidbody_location(0, p_rigid_body2d, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(int p_listener_index, RigidBody2D *p_rigid_body2d,
                                            Node2D *p_attenuation_object)
{
    set_listener_2d_rigidbody_location(p_listener_index, p_rigid_body2d, p_attenuation_object);
}
void FmodAudioServer::set_listener_2d_rigidbody_location(int p_listener_index, RigidBody2D *p_rigid_body_2d,
                                                         Node2D *p_attenuation_object)
{
    FMOD_3D_ATTRIBUTES node_attr;
    FMOD_VECTOR attenuation_attr;
    node_attr = to_3d_attributes(p_rigid_body_2d);
    if (p_attenuation_object)
    {
        attenuation_attr = to_fmod_vector(p_attenuation_object->get_global_position());

        FMOD_Studio_System_SetListenerAttributes(studio_system, p_listener_index, &node_attr, &attenuation_attr);
    }
    else
    {
        FMOD_Studio_System_SetListenerAttributes(studio_system, p_listener_index, &node_attr, nullptr);
    }
}
void FmodAudioServer::set_listener_2d_location(int p_listener_index, Node2D *p_node, Node2D *p_attenuation_object)
{
    FMOD_3D_ATTRIBUTES node_attr;
    FMOD_VECTOR attenuation_attr;
    node_attr = to_3d_attributes(p_node);
    if (p_attenuation_object)
    {
        attenuation_attr = to_fmod_vector(p_attenuation_object->get_global_position());
        FMOD_Studio_System_SetListenerAttributes(studio_system, p_listener_index, &node_attr, &attenuation_attr);
    }
    else
    {
        FMOD_Studio_System_SetListenerAttributes(studio_system, p_listener_index, &node_attr, nullptr);
    }
}

void FmodAudioServer::set_listener_location(Node3D *p_node, Node3D *p_attenuation_object)
{
    set_listener_3d_location(0, p_node, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(int p_listener_index, Node3D *p_node, Node3D *p_attenuation_object)
{
    set_listener_3d_location(p_listener_index, p_node, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(RigidBody3D *p_rigid_body3d, Node3D *p_attenuation_object)
{
    set_listener_3d_rigidbody_location(0, p_rigid_body3d, p_attenuation_object);
}
void FmodAudioServer::set_listener_location(int p_listener_index, RigidBody3D *p_rigid_body3d,
                                            Node3D *p_attenuation_object)
{
    set_listener_3d_rigidbody_location(p_listener_index, p_rigid_body3d, p_attenuation_object);
}
void FmodAudioServer::set_listener_3d_rigidbody_location(int listenerIndex, RigidBody3D *p_rigid_body3d,
                                                         Node3D *attenuationObject)
{
    FMOD_3D_ATTRIBUTES node_attr;
    FMOD_VECTOR attenuation_attr;
    node_attr = to_3d_attributes(p_rigid_body3d);
    if (attenuationObject)
    {
        attenuation_attr = to_fmod_vector(attenuationObject->get_global_position());
        FMOD_Studio_System_SetListenerAttributes(studio_system, listenerIndex, &node_attr, &attenuation_attr);
    }
    else
    {
        FMOD_Studio_System_SetListenerAttributes(studio_system, listenerIndex, &node_attr, nullptr);
    }
}

void FmodAudioServer::set_listener_3d_location(int listenerIndex, Node3D *p_node, Node3D *attenuationObject)
{
    FMOD_3D_ATTRIBUTES node_attr;
    FMOD_VECTOR attenuation_attr;
    node_attr = to_3d_attributes(p_node);
    if (attenuationObject)
    {
        attenuation_attr = to_fmod_vector(attenuationObject->get_global_position());
        FMOD_Studio_System_SetListenerAttributes(studio_system, listenerIndex, &node_attr, &attenuation_attr);
    }
    else
    {
        FMOD_Studio_System_SetListenerAttributes(studio_system, listenerIndex, &node_attr, nullptr);
    }
}

#pragma endregion
extern "C"
{
    GDE_EXPORT int get_fmod_core(FMOD_SYSTEM **core)
    {
        FmodAudioServer::get_singleton()->get_core_ref(core);
        if (core == NULL)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    GDE_EXPORT int get_fmod_studio(FMOD_STUDIO_SYSTEM **studio)
    {
        FmodAudioServer::get_singleton()->get_studio_ref(studio);
        if (studio == NULL)
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

    GDE_EXPORT void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector3 p_position = godot::Vector3())
    {
        FS->play_one_shot_by_id(p_guid, p_position);
    }
    GDE_EXPORT void play_one_shot_by_path(const char *p_path, const godot::Vector3 p_position = godot::Vector3())
    {
        play_one_shot_by_path(p_path, p_position);
    }
    GDE_EXPORT void play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node,
                                                    bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_3d_attached_by_id(p_guid, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_3d_attached_by_path(const char *p_path, Node3D *p_node,
                                                      bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_3d_attached_by_path(p_path, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node,
                                                    bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_2d_attached_by_id(p_guid, p_node, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void play_one_shot_2d_attached_by_path(const char *p_path, Node2D *p_node,
                                                      bool p_non_rigid_body_velocity = false)
    {
        FS->play_one_shot_2d_attached_by_path(p_path, p_node, p_non_rigid_body_velocity);
    }

    GDE_EXPORT void play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d)
    {
        FS->play_one_shot_rigid_body3d_attached_by_id(p_guid, p_rigid_body3d);
    }
    GDE_EXPORT void play_one_shot_rigid_body3d_attached_by_path(const char *p_path, RigidBody3D *p_rigid_body3d)
    {
        FS->play_one_shot_rigid_body3d_attached_by_path(p_path, p_rigid_body3d);
    }
    GDE_EXPORT void play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d)
    {
        FS->play_one_shot_rigid_body2d_attached_by_id(p_guid, p_rigid_body2d);
    }
    GDE_EXPORT void play_one_shot_rigid_body2d_attached_by_path(const char *p_path, RigidBody2D *p_rigid_body2d)
    {
        FS->play_one_shot_rigid_body2d_attached_by_path(p_path, p_rigid_body2d);
    }
    GDE_EXPORT bool any_sample_data_loading()
    {
        return FS->any_sample_data_loading();
    }
    GDE_EXPORT void attach_instance_to_node3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->attach_instance_to_node3d(p_node, p_event, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void attach_instance_to_rigid_body3d(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        attach_instance_to_rigid_body3d(p_node, p_event);
    }
    GDE_EXPORT void attach_instance_to_node2D(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                              bool p_non_rigid_body_velocity = false)
    {
        FS->attach_instance_to_node2D(p_node, p_event, p_non_rigid_body_velocity);
    }
    GDE_EXPORT void attach_instance_to_rigid_body2d(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        attach_instance_to_rigid_body2d(p_node, p_event);
    }
    GDE_EXPORT void detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE *p_event)
    {
        detach_instance_from_node(p_event);
    }

    GDE_EXPORT Vector4i path_to_guid(const char *p_path)
    {
        return FmodAudioServer::get_singleton()->path_to_guid(p_path);
    }
    GDE_EXPORT FMOD_STUDIO_EVENTDESCRIPTION *get_event_description_by_path(const char *p_path)
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
    GDE_EXPORT FMOD_RESULT load_bank(const char *p_bankName, bool loadSamples = false)
    {
        return (FMOD_RESULT)FS->load_bank(p_bankName, loadSamples);
    }

    GDE_EXPORT FMOD_RESULT load_bank_by_file(const char *p_path, bool loadSamples = false)
    {
        return (FMOD_RESULT)FS->load_bank_by_file(p_path, loadSamples);
    }
    GDE_EXPORT void unload_banks()
    {
        FS->unload_banks();
    }
    GDE_EXPORT bool has_bank_loaded(const char *&p_bankName)
    {
        return FS->has_bank_loaded(p_bankName);
    }

    GDE_EXPORT bool have_all_banks_loaded()
    {
        return FS->have_all_banks_loaded();
    }

    GDE_EXPORT void set_listener_2d_rigidbody_location(int p_listenerIndex, void *p_rigidBody2D,
                                                       void *p_attenuationObject = nullptr)
    {
        FS->set_listener_2d_rigidbody_location(p_listenerIndex,
                                               (RigidBody2D *)internal::get_object_instance_binding(p_rigidBody2D),
                                               (Node2D *)internal::get_object_instance_binding(p_attenuationObject));
    }
    GDE_EXPORT void set_listener_2d_location(int p_listenerIndex, void *p_node, void *p_attenuationObject = nullptr)
    {
        FS->set_listener_2d_location(p_listenerIndex, (Node2D *)internal::get_object_instance_binding(p_node),
                                     (Node2D *)internal::get_object_instance_binding(p_attenuationObject));
    }

    GDE_EXPORT void set_listener_3d_rigidbody_location(int listenerIndex, void *rigidBody,
                                                       void *attenuationObject = nullptr)
    {
        FS->set_listener_3d_rigidbody_location(listenerIndex,
                                               (RigidBody3D *)internal::get_object_instance_binding(rigidBody),
                                               (Node3D *)internal::get_object_instance_binding(attenuationObject));
    }
    GDE_EXPORT void set_listener_3d_location(int listenerIndex, void *p_node, void *attenuationObject = nullptr)
    {
        FS->set_listener_3d_location(listenerIndex, (Node3D *)internal::get_object_instance_binding(p_node),
                                     (Node3D *)internal::get_object_instance_binding(attenuationObject));
    }
}
} // namespace FmodGodot
