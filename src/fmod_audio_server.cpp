#include "fmod_audio_server.h"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <fmod_errors.h>
#include "fmod_globals.h"
#include "globals.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
using namespace std;
using namespace godot;
namespace FmodGodot
{
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
        return godot_file_error_to_fmod_file_error(((FileAccess *)handle)->get_error());
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
        Master = nullptr;
        Masterstrings = nullptr;
    }
    FmodAudioServer::~FmodAudioServer()
    {
        // if (banks)
        // {
        //     delete (banks);
        // }
    }
    FMOD_RESULT FmodAudioServer::init()
    {
        mutex.instantiate();
        thread.instantiate();
        FMOD_RESULT result;
        result = FMOD::Studio::System::create(&studio_system);
        if (result != FMOD_OK)
        {
            std::cout << "\ncreate" << FMOD_ErrorString(result);
        }
        result = studio_system->initialize(ProjectSettings::get_singleton()->get_setting("Fmod/General/channel_count"), FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
        if (result != FMOD_OK)
        {
            std::cout << "\ninit" << FMOD_ErrorString(result);
        }
        result = studio_system->getCoreSystem(&core_system);
        if (result != FMOD_OK)
        {
            std::cout << "\nget core" << FMOD_ErrorString(result);
        }
        // result = core->setDSPBufferSize(ProjectSettings::get_singleton()->get_setting("Fmod/DSP/dsp_buffer_size", 512), ProjectSettings::get_singleton()->get_setting("Fmod/DSP/dsp_buffer_count", 4));
        if (result != FMOD_OK)
        {
            std::cout << "\ndsp" << FMOD_ErrorString(result);
        }
        initialized = true;
        thread->start(callable_mp(this, &FmodAudioServer::thread_func), Thread::Priority::PRIORITY_NORMAL);
        core_system->set3DSettings(1, 1, 1);
        core_system->setFileSystem(open_callback, close_callback, read_callback, seek_callback, NULL, NULL, 2048);
        LoadMasterBanks();

        return result;
    }
    void FmodAudioServer::_physics_process()
    {
        lock();
        for (int i = 0; i < instances.size(); i++)
        {
            if (!instances[i].instance->isValid())
            {
                instances.write[i] = instances[instances.size() - 1];
                instances.remove_at(instances.size() - 1);
                continue;
            }
            FMOD_3D_ATTRIBUTES attributes;
            Vector3 velocity;
            switch (instances[i].attachment)
            {
            case Attachment::NODE2D:
            {
                instances[i].instance->get3DAttributes(&attributes);
                Vector2 pos = instances[i].node2D->get_global_position();
                velocity = (Vector3(pos.x, pos.y, 0) - instances[i].lastFramePosition) / instances[i].node2D->get_physics_process_delta_time();
                attributes.velocity = to_fmod_vector(velocity);
                instances.write[i].lastFramePosition = Vector3(pos.x, pos.y, 0);
                break;
            }
            case Attachment::NODE3D:
            {
                instances[i].instance->get3DAttributes(&attributes);
                velocity = (instances[i].node3D->get_global_position() - instances[i].lastFramePosition) / instances[i].node3D->get_physics_process_delta_time();
                attributes.velocity = to_fmod_vector(velocity);
                instances.write[i].lastFramePosition = instances[i].node3D->get_global_position();
                instances[i].instance->set3DAttributes(&attributes);
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
        tree->connect("physics_frame", callable_mp(this, &FmodAudioServer::_physics_process));
        while (!exit_thread)
        {
            FMOD_3D_ATTRIBUTES attributes;
            lock();
            for (int i = 0; i < instances.size(); i++)
            {
                if (!instances[i].instance->isValid())
                {
                    instances.write[i] = instances[instances.size() - 1];
                    instances.remove_at(instances.size() - 1);
                    continue;
                }
                switch (instances[i].attachment)
                {
                case NODE2D:
                    attributes = to_3d_attributes(instances[i].node2D);
                    instances[i].instance->set3DAttributes(&attributes);
                    break;
                case NODE3D:
                    attributes = to_3d_attributes(instances[i].node3D);
                    instances[i].instance->set3DAttributes(&attributes);
                    break;
                case RIGIDBODY2D:
                    attributes = to_3d_attributes(instances[i].rigidBody2D);
                    instances[i].instance->set3DAttributes(&attributes);
                    break;
                case RIGIDBODY3D:
                    attributes = to_3d_attributes(instances[i].rigidBody3D);
                    instances[i].instance->set3DAttributes(&attributes);
                    break;

                default:
                    break;
                }
            }
            unlock();
            studio_system->update();
            OS::get_singleton()->delay_usec(20);
        }
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
        thread_exited = true;
        mutex.unref();
    }

#pragma endregion
    void FmodAudioServer::_bind_methods()
    {
        // BIND_ENUM_CONSTANT(FMOD_OK);
        // BIND_ENUM_CONSTANT(FMOD_ERR_BADCOMMAND);
        // BIND_ENUM_CONSTANT(FMOD_ERR_CHANNEL_ALLOC);
        // BIND_ENUM_CONSTANT(FMOD_ERR_CHANNEL_STOLEN);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DMA);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_CONNECTION);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_DONTPROCESS);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_FORMAT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_INUSE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_NOTFOUND);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_RESERVED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_SILENCE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_DSP_TYPE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_BAD);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_COULDNOTSEEK);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_DISKEJECTED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_EOF);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_ENDOFDATA);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FILE_NOTFOUND);
        // BIND_ENUM_CONSTANT(FMOD_ERR_FORMAT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HEADER_MISMATCH);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HTTP);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HTTP_ACCESS);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HTTP_PROXY_AUTH);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HTTP_SERVER_ERROR);
        // BIND_ENUM_CONSTANT(FMOD_ERR_HTTP_TIMEOUT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INITIALIZATION);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INITIALIZED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INTERNAL);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_FLOAT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_HANDLE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_PARAM);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_POSITION);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_SPEAKER);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_SYNCPOINT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_THREAD);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_VECTOR);
        // BIND_ENUM_CONSTANT(FMOD_ERR_MAXAUDIBLE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_MEMORY);
        // BIND_ENUM_CONSTANT(FMOD_ERR_MEMORY_CANTPOINT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NEEDS3D);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NEEDSHARDWARE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NET_CONNECT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NET_SOCKET_ERROR);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NET_URL);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NET_WOULD_BLOCK);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NOTREADY);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_ALLOCATED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_CREATEBUFFER);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_DRIVERCALL);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_FORMAT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_INIT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_OUTPUT_NODRIVERS);
        // BIND_ENUM_CONSTANT(FMOD_ERR_PLUGIN);
        // BIND_ENUM_CONSTANT(FMOD_ERR_PLUGIN_MISSING);
        // BIND_ENUM_CONSTANT(FMOD_ERR_PLUGIN_RESOURCE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_PLUGIN_VERSION);
        // BIND_ENUM_CONSTANT(FMOD_ERR_RECORD);
        // BIND_ENUM_CONSTANT(FMOD_ERR_REVERB_CHANNELGROUP);
        // BIND_ENUM_CONSTANT(FMOD_ERR_REVERB_INSTANCE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_SUBSOUNDS);
        // BIND_ENUM_CONSTANT(FMOD_ERR_SUBSOUND_ALLOCATED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_SUBSOUND_CANTMOVE);
        // BIND_ENUM_CONSTANT(FMOD_ERR_TAGNOTFOUND);
        // BIND_ENUM_CONSTANT(FMOD_ERR_TOOMANYCHANNELS);
        // BIND_ENUM_CONSTANT(FMOD_ERR_TRUNCATED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_UNIMPLEMENTED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_UNINITIALIZED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_UNSUPPORTED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_VERSION);
        // BIND_ENUM_CONSTANT(FMOD_ERR_EVENT_ALREADY_LOADED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_EVENT_LIVEUPDATE_BUSY);
        // BIND_ENUM_CONSTANT(FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH);
        // BIND_ENUM_CONSTANT(FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT);
        // BIND_ENUM_CONSTANT(FMOD_ERR_EVENT_NOTFOUND);
        // BIND_ENUM_CONSTANT(FMOD_ERR_STUDIO_UNINITIALIZED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_STUDIO_NOT_LOADED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_INVALID_STRING);
        // BIND_ENUM_CONSTANT(FMOD_ERR_ALREADY_LOCKED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_NOT_LOCKED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_RECORD_DISCONNECTED);
        // BIND_ENUM_CONSTANT(FMOD_ERR_TOOMANYSAMPLES);
        // BIND_ENUM_CONSTANT(FMOD_RESULT_FORCEINT);
        ClassDB::bind_method(D_METHOD("unload_banks"), &FmodAudioServer::unload_banks);

        BIND_BOOL_PROPERTY(muted, FmodAudioServer);
        ClassDB::bind_method(D_METHOD("play_one_shot_by_id", "guid", "position"), &FmodAudioServer::play_one_shot_by_id, DEFVAL(Vector3(0, 0, 0)));
        ClassDB::bind_static_method("FmodAudioServer", D_METHOD("get_singleton"), &FmodAudioServer::get_singleton);
        ClassDB::bind_method(D_METHOD("play_one_shot_by_path", "path", "position"), &FmodAudioServer::play_one_shot_by_path, DEFVAL(Vector3(0, 0, 0)));
        ClassDB::bind_method(D_METHOD("play_one_shot_3d_attached_by_id", "guid", "node", "non_rigid_body_velocity"), &FmodAudioServer::play_one_shot_3d_attached_by_id, DEFVAL(false));
        ClassDB::bind_method(D_METHOD("play_one_shot_3d_attached_by_path", "path", "node", "non_rigid_body_velocity"), &FmodAudioServer::play_one_shot_3d_attached_by_path, DEFVAL(false));
        ClassDB::bind_method(D_METHOD("play_one_shot_2d_attached_by_id", "guid", "node", "non_rigid_body_velocity"), &FmodAudioServer::play_one_shot_2d_attached_by_id, DEFVAL(false));
        ClassDB::bind_method(D_METHOD("play_one_shot_2d_attached_by_path", "path", "node", "non_rigid_body_velocity"), &FmodAudioServer::play_one_shot_2d_attached_by_path, DEFVAL(false));

        ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body3d_attached_by_id", "guid", "rigid_body3d"), &FmodAudioServer::play_one_shot_rigid_body3d_attached_by_id);
        ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body3d_attached_by_path", "path", "rigid_body3d"), &FmodAudioServer::play_one_shot_rigid_body3d_attached_by_path);
        ClassDB::bind_method(D_METHOD("play_one_shot_rigid_body2d_attached_by_id", "guid", "rigid_body2d"), &FmodAudioServer::play_one_shot_rigid_body2d_attached_by_id);
        ClassDB::bind_method(D_METHOD("play_one_shot_rigid_2d_attached_by_path", "path", "rigid_body2d"), &FmodAudioServer::play_one_shot_rigid_body2d_attached_by_path);
        // bool any_sample_data_loading();
        // ClassDB::bind_method(D_METHOD("attach_instance_to_node3d", "node", "instance", "non_rigid_body_velocity"), &FmodAudioServer::attach_instance_to_node3d);
        // void attach_instance_to_node3D(Node3D * p_node, Studio::EventInstance * p_instance, bool p_non_rigid_body_velocity = false);
        // void attach_instance_to_rigid_body_3D(RigidBody3D * p_node, Studio::EventInstance * p_instance);
        // void attach_instance_to_node2D(Node2D * p_node, Studio::EventInstance * p_instance, bool p_non_rigid_body_velocity = false);
        // void attach_instance_to_rigid_body_2D(RigidBody2D * p_node, Studio::EventInstance * p_instance);

        // void detach_instance_from_node(Studio::EventInstance * p_instance);

        // Vector4i path_to_guid(String p_path);
        // Studio::EventDescription *get_event_description(String p_path);
        // Studio::EventDescription *get_event_description(Vector4i p_guid);
        // void pause_all_events(bool p_pause);

        // bool is_muted();
        // void set_muted(bool p_muted);

        // Studio::Bus *get_bus(String p_path);
        // Studio::VCA *get_vca(String p_path);
        // FMOD_RESULT load_bank(String p_bankName, bool loadSamples = false);
        // void unload_bank(String p_bankName);
        // bool has_bank_loaded(String p_bankName);

        // bool have_all_banks_loaded();
        // bool have_all_master_banks_loaded();

        // void set_listener_location(Node2D * p_node, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(Node2D * p_node, RigidBody2D * p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(int p_listenerIndex, Node2D *p_node, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(int p_istenerIndex, Node2D *p_node, Node2D *p_attenuationObject = nullptr);
    }

    FmodAudioServer *FmodAudioServer::get_singleton()
    {
        return singleton;
    }
    void FmodAudioServer::LoadMasterBanks()
    {
        load_bank(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Bank_Path", "").stringify().utf8().ptr());
        load_bank(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Strings_Bank_Path", "").stringify().utf8().ptr());
    }
#pragma region server api
    FMOD_RESULT FmodAudioServer::load_bank(String path, bool loadSamples)
    {
        cout << "AHHHH" << path.utf8() << std::endl;
        Studio::Bank *bank;
        FMOD_RESULT result = studio_system->loadBankFile(path.utf8().ptr(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
        if (result == FMOD_OK)
        {
            banks.insert(path, 1);
            if (loadSamples)
            {
                bank->loadSampleData();
            }
        }
        else if (result == FMOD_ERR_EVENT_ALREADY_LOADED)
        {
            banks[path] = banks[path] + 1;
            if (loadSamples)
            {
                bank->loadSampleData();
            }
        }
        else
        {
            godot::_err_print_error("load_bank", "FmodAudioServer.cpp", 103, "Couldn't load bank:" + path + " error:" + result, true, true);
        }

        return result;
    }

    // FMOD_RESULT FmodAudioServer::load_bank(FmodBank p_bank, bool loadSamples)
    // {

    //     PackedByteArray BankMemory = p_bank.;
    //     Studio::Bank *bank;
    //     FMOD_RESULT result = studio_system->loadBankMemory((char *)BankMemory.ptr(), BankMemory.size(), FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    //     if (result == FMOD_OK)
    //     {
    //         banks.insert(p_bank.get_path(), 1);
    //         if (loadSamples)
    //         {
    //             bank->loadSampleData();
    //         }
    //     }
    //     else if (result == FMOD_ERR_EVENT_ALREADY_LOADED)
    //     {
    //         banks[p_bank.get_path()] = banks[p_bank.get_path()] + 1;
    //         if (loadSamples)
    //         {
    //             bank->loadSampleData();
    //         }
    //     }
    //     else
    //     {
    //         godot::_err_print_error("load_bank", "FmodAudioServer.cpp", 103, "Couldn't load bank:" + path + " error:" + result, true, true);
    //     }

    //     return result;
    // }

    void FmodAudioServer::unload_bank(String path)
    {
        lock();
        if (!studio_system->isValid())
        {
            return;
        }
        banks[path] = banks[path] - 1;
        if (banks[path] <= 0)
        {
            banks.erase(path);
            Studio::Bank *bank;
            studio_system->getBank(path.utf8().ptr(), &bank);
            bank->unload();
        }
        unlock();
    }
    void FmodAudioServer::unload_banks()
    {
        lock();
        if (!studio_system->isValid())
        {
            unlock();
            return;
        }
        studio_system->unloadAll();
        unlock();
    }
    void FmodAudioServer::get_core_ref(System **core)
    {
        *core = this->core_system;
    }
    void FmodAudioServer::get_studio_ref(Studio::System **studio)
    {
        *studio = this->studio_system;
    }
    System *FmodAudioServer::get_core()
    {
        return core_system;
    }
    Studio::System *FmodAudioServer::get_studio()
    {
        return studio_system;
    }

    System *FmodAudioServer::get_global_core()
    {
        return singleton->get_core();
    }
    Studio::System *FmodAudioServer::get_global_studio()
    {
        return singleton->get_studio();
    }
    void FmodAudioServer::get_global_core_ref(System **core)
    {
        singleton->get_core_ref(core);
    }
    void FmodAudioServer::get_global_studio_ref(Studio::System **studio)
    {
        singleton->get_studio_ref(studio);
    }
    FMOD_RESULT FmodAudioServer::load_global_bank(String path)
    {

        return singleton->load_bank(path);
    }
    void FmodAudioServer::unload_global_bank(String path)
    {
        singleton->unload_bank(path);
    }
    void FmodAudioServer::unload_global_banks()
    {
        singleton->unload_banks();
    }
    Studio::EventInstance *FmodAudioServer::create_instance(const Vector4i p_guid) const
    {
        FMOD_GUID guid = cast_to_FMOD_GUID(p_guid);
        Studio::EventDescription *description;
        studio_system->getEventByID(&guid, &description);
        Studio::EventInstance *event;
        description->createInstance(&event);
        return event;
    }
    void FmodAudioServer::play_one_shot_by_id(const Vector4i p_guid, const Vector3 p_position) const
    {
        Studio::EventInstance *event = create_instance(p_guid);
        FMOD_3D_ATTRIBUTES attr;
        attr.forward = {0, 0, -1};
        attr.up = {0, 1, 0};
        attr.position = {p_position.x, -p_position.y, p_position.z};
        attr.velocity = {0, 0, 0};
        event->start();
        event->release();
    }
    void FmodAudioServer::play_one_shot_by_path(String p_path, const Vector3 p_position) const
    {
        play_one_shot_by_id(path_to_guid(p_path), p_position);
    }
    void FmodAudioServer::play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool p_non_rigid_body_velocity)
    {
        Studio::EventInstance *event = create_instance(p_guid);

        attach_instance_to_node3d(p_node, event);
        event->start();
        event->release();
    }
    void FmodAudioServer::play_one_shot_3d_attached_by_path(String p_path, Node3D *p_node, bool p_non_rigid_body_velocity)
    {
        play_one_shot_3d_attached_by_id(path_to_guid(p_path), p_node);
    }
    void FmodAudioServer::play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool p_non_rigid_body_velocity)
    {
        Studio::EventInstance *event = create_instance(p_guid);

        attach_instance_to_node2D(p_node, event);
        event->start();
        event->release();
    }
    void FmodAudioServer::play_one_shot_2d_attached_by_path(String p_path, Node2D *p_node, bool p_non_rigid_body_velocity)
    {
        play_one_shot_2d_attached_by_id(path_to_guid(p_path), p_node);
    }

    void FmodAudioServer::play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d)
    {
        Studio::EventInstance *event = create_instance(p_guid);

        attach_instance_to_rigid_body3d(p_rigid_body3d, event);
        event->start();
        event->release();
    }
    void FmodAudioServer::play_one_shot_rigid_body3d_attached_by_path(const String p_path, RigidBody3D *p_rigid_body3d)
    {
        play_one_shot_rigid_body3d_attached_by_id(path_to_guid(p_path), p_rigid_body3d);
    }
    void FmodAudioServer::play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d)
    {
        Studio::EventInstance *event = create_instance(p_guid);

        attach_instance_to_rigid_body2d(p_rigid_body2d, event);
        event->start();
        event->release();
    }
    void FmodAudioServer::play_one_shot_rigid_body2d_attached_by_path(const String p_path, RigidBody2D *p_rigid_body2d)
    {
        play_one_shot_rigid_body2d_attached_by_id(path_to_guid(p_path), p_rigid_body2d);
    }
    bool FmodAudioServer::any_sample_data_loading()
    {
        int count;
        studio_system->getBankCount(&count);
        int retrieved;
        Studio::Bank **banks;

        studio_system->getBankList(banks, count, &retrieved);
        bool loading = false;
        for (int i = 0; i < retrieved; i++)
        {
            FMOD_STUDIO_LOADING_STATE state;
            banks[i]->getSampleLoadingState(&state);
            loading |= (state == FMOD_STUDIO_LOADING_STATE_LOADING);
        }
        return loading;
    }

    void FmodAudioServer::attach_instance_to_node3d(Node3D *p_node, Studio::EventInstance *p_instance, bool p_non_rigid_body_velocity)
    {
        lock();
        int instance_index = -1;
        for (int i = 0; i < instances.size(); i++)
        {
            if (instances[i].instance == p_instance)
            {
                instance_index = i;
            }
        }
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.node3D = p_node;
            instance.nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instance.attachment = Attachment::NODE3D;
            instance.lastFramePosition = p_node->get_position();
            instance.instance = p_instance;
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].node3D = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instances.write[instance_index].attachment = Attachment::NODE3D;
            instances.write[instance_index].lastFramePosition = p_node->get_position();
            instances.write[instance_index].instance = p_instance;
        }
        unlock();
    }
    void FmodAudioServer::attach_instance_to_rigid_body3d(RigidBody3D *p_node, Studio::EventInstance *p_instance)
    {
        lock();
        int instance_index = -1;
        for (int i = 0; i < instances.size(); i++)
        {
            if (instances[i].instance == p_instance)
            {
                instance_index = i;
            }
        }
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.rigidBody3D = p_node;
            instance.nonRigidbodyVelocity = false;
            instance.attachment = Attachment::RIGIDBODY3D;
            instance.lastFramePosition = p_node->get_position();
            instance.instance = p_instance;
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].rigidBody3D = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = false;
            instances.write[instance_index].attachment = Attachment::RIGIDBODY3D;
            instances.write[instance_index].lastFramePosition = p_node->get_position();
            instances.write[instance_index].instance = p_instance;
        }
        unlock();
    }
    void FmodAudioServer::attach_instance_to_node2D(Node2D *p_node, Studio::EventInstance *p_instance, bool p_non_rigid_body_velocity)
    {
        lock();
        int instance_index = -1;
        for (int i = 0; i < instances.size(); i++)
        {
            if (instances[i].instance == p_instance)
            {
                instance_index = i;
            }
        }
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.node2D = p_node;
            instance.nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instance.attachment = Attachment::NODE2D;
            instance.lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            instance.instance = p_instance;
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].node2D = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instances.write[instance_index].attachment = Attachment::NODE2D;
            instances.write[instance_index].lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            instances.write[instance_index].instance = p_instance;
        }
        unlock();
    }
    void FmodAudioServer::attach_instance_to_rigid_body2d(RigidBody2D *p_node, Studio::EventInstance *p_instance)
    {
        lock();
        int instance_index = -1;
        for (int i = 0; i < instances.size(); i++)
        {
            if (instances[i].instance == p_instance)
            {
                instance_index = i;
            }
        }
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.node2D = p_node;
            instance.nonRigidbodyVelocity = false;
            instance.attachment = Attachment::NODE2D;
            instance.lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            instance.instance = p_instance;
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].node2D = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = false;
            instances.write[instance_index].attachment = Attachment::NODE2D;
            instances.write[instance_index].lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            instances.write[instance_index].instance = p_instance;
        }
        unlock();
    }

    void FmodAudioServer::detach_instance_from_node(Studio::EventInstance *p_instance)
    {
        lock();
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

    Vector4i FmodAudioServer::path_to_guid(String p_path) const
    {
        FMOD_GUID guid;
        studio_system->lookupID(p_path.utf8(), &guid);
        return cast_to_Vector4i(guid);
    }
    Studio::EventDescription *FmodAudioServer::get_event_description(String p_path) const
    {
        Studio::EventDescription *description;
        studio_system->getEvent(p_path.utf8(), &description);
        return description;
    }
    Studio::EventDescription *FmodAudioServer::get_event_description(Vector4i p_guid) const
    {
        Studio::EventDescription *description;
        FMOD_GUID guid = cast_to_FMOD_GUID(p_guid);
        studio_system->getEventByID(&guid, &description);
        return description;
    }
    void FmodAudioServer::pause_all_events(bool p_pause)
    {
        Studio::Bus *bus;
        if (studio_system->getBus("bus:/", &bus) == FMOD_RESULT::FMOD_OK)
        {
            bus->setPaused(p_pause);
        }
    }
    bool FmodAudioServer::is_muted() const
    {
        return muted;
    }
    void FmodAudioServer::set_muted(bool p_muted)
    {
        Studio::Bus *bus;
        muted = p_muted;
        if (studio_system->getBus("bus:/", &bus) == FMOD_RESULT::FMOD_OK)
        {
            bus->setMute(p_muted);
        }
    }
    Studio::Bus *FmodAudioServer::get_bus(String p_path) const
    {
        Studio::Bus *bus;
        studio_system->getBus(p_path.utf8(), &bus);
        return bus;
    }
    Studio::VCA *FmodAudioServer::get_vca(String p_path) const
    {
        Studio::VCA *vca;
        studio_system->getVCA(p_path.utf8(), &vca);
        return vca;
    }
    bool FmodAudioServer::has_bank_loaded(String p_bankName) const
    {
        return banks.has(p_bankName);
    }

    bool FmodAudioServer::have_all_banks_loaded() const
    {
        // TODO
        return true;
    }
    bool FmodAudioServer::have_all_master_banks_loaded() const
    {
        return has_bank_loaded(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Bank_Path", "").stringify()) && has_bank_loaded(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Strings_Bank_Path", "").stringify());
    }

    void FmodAudioServer::set_listener_location(Node2D *p_node, Node2D *p_attenuation_object)
    {
        set_listener_location(0, p_node, p_attenuation_object);
    }
    void FmodAudioServer::set_listener_location(RigidBody2D *p_rigid_body2d, Node2D *p_attenuation_object)
    {
        set_listener_location(0, p_rigid_body2d, p_attenuation_object);
    }
    void FmodAudioServer::set_listener_location(int p_listener_index, RigidBody2D *p_rigid_body_2d, Node2D *p_attenuation_object)
    {
        FMOD_3D_ATTRIBUTES node_attr;
        FMOD_VECTOR attenuation_attr;
        node_attr = to_3d_attributes(p_rigid_body_2d);
        if (p_attenuation_object)
        {
            attenuation_attr = to_fmod_vector(p_attenuation_object->get_global_position());
            studio_system->setListenerAttributes(p_listener_index, &node_attr, &attenuation_attr);
        }
        else
        {
            studio_system->setListenerAttributes(p_listener_index, &node_attr, nullptr);
        }
    }
    void FmodAudioServer::set_listener_location(int p_listener_index, Node2D *p_node, Node2D *p_attenuation_object)
    {
        FMOD_3D_ATTRIBUTES node_attr;
        FMOD_VECTOR attenuation_attr;
        node_attr = to_3d_attributes(p_node);
        if (p_attenuation_object)
        {
            attenuation_attr = to_fmod_vector(p_attenuation_object->get_global_position());
            studio_system->setListenerAttributes(p_listener_index, &node_attr, &attenuation_attr);
        }
        else
        {
            studio_system->setListenerAttributes(p_listener_index, &node_attr, nullptr);
        }
    }

    void FmodAudioServer::set_listener_location(Node3D *p_node, Node3D *p_attenuation_object)
    {
        set_listener_location(0, p_node, p_attenuation_object);
    }
    void FmodAudioServer::set_listener_location(RigidBody3D *p_rigid_body3d, Node3D *p_attenuation_object)
    {
        set_listener_location(0, p_rigid_body3d, p_attenuation_object);
    }
    void FmodAudioServer::set_listener_location(int listenerIndex, RigidBody3D *p_rigid_body3d, Node3D *attenuationObject)
    {
        FMOD_3D_ATTRIBUTES node_attr;
        FMOD_VECTOR attenuation_attr;
        node_attr = to_3d_attributes(p_rigid_body3d);
        if (attenuationObject)
        {
            attenuation_attr = to_fmod_vector(attenuationObject->get_global_position());
            studio_system->setListenerAttributes(listenerIndex, &node_attr, &attenuation_attr);
        }
        else
        {
            studio_system->setListenerAttributes(listenerIndex, &node_attr, nullptr);
        }
    }
    void FmodAudioServer::set_listener_location(int listenerIndex, Node3D *p_node, Node3D *attenuationObject)
    {
        FMOD_3D_ATTRIBUTES node_attr;
        FMOD_VECTOR attenuation_attr;
        node_attr = to_3d_attributes(p_node);
        if (attenuationObject)
        {
            attenuation_attr = to_fmod_vector(attenuationObject->get_global_position());
            studio_system->setListenerAttributes(listenerIndex, &node_attr, &attenuation_attr);
        }
        else
        {
            studio_system->setListenerAttributes(listenerIndex, &node_attr, nullptr);
        }
    }
#pragma endregion
    extern "C"
    {
        GDE_EXPORT int get_fmod_core(System **core)
        {
            FmodAudioServer::get_global_core_ref(core);
            if (core == NULL)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        GDE_EXPORT int get_fmod_studio(Studio::System **studio)
        {
            FmodAudioServer::get_global_studio_ref(studio);
            if (studio == NULL)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
