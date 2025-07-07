#include "fmod_audio_server_proxy.h"
#include "fmod_audio_server.h"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <fmod_errors.h>
#include "fmod_globals.h"
#include "globals.h"
#include <godot_cpp/classes/os.hpp>
using namespace std;
using namespace godot;
namespace FmodGodot
{
#pragma region Server Functionality

    FmodAudioServerProxy::FmodAudioServerProxy()
    {
    }
    FmodAudioServerProxy::~FmodAudioServerProxy()
    {
    }

#pragma endregion
    void FmodAudioServerProxy::_bind_methods()
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
        ClassDB::bind_method(D_METHOD("unload_banks"), &FmodAudioServerProxy::unload_banks);

        BIND_BOOL_PROPERTY(muted, FmodAudioServerProxy);
        ClassDB::bind_method(D_METHOD("play_one_shot_by_id", "guid", "position"), &FmodAudioServerProxy::play_one_shot_by_id);
        ClassDB::bind_method(D_METHOD("play_one_shot_by_path", "path", "position"), &FmodAudioServerProxy::play_one_shot_by_path);
        ClassDB::bind_method(D_METHOD("play_one_shot_3d_attached_by_id", "guid", "node", "non_rigidbody_velocity"), &FmodAudioServerProxy::play_one_shot_3d_attached_by_id);
        // void play_one_shot_3d_attached_by_path(const String p_path, Node3D *p_node, bool nonRigidBodyVelocity = false);
        // void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool nonRigidBodyVelocity = false);
        // void play_one_shot_2d_attached_by_path(const String p_path, Node2D *p_node, bool nonRigidBodyVelocity = false);
        // bool any_sample_data_loading();

        // void attach_instance_to_node3D(Node3D * p_node, Studio::EventInstance * p_instance, bool nonRigidBodyVelocity = false);
        // void attach_instance_to_rigid_body_3D(RigidBody3D * p_node, Studio::EventInstance * p_instance);
        // void attach_instance_to_node2D(Node2D * p_node, Studio::EventInstance * p_instance, bool nonRigidBodyVelocity = false);
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

    void FmodAudioServerProxy::LoadMasterBanks()
    {
    }
#pragma region server api
    FMOD_RESULT FmodAudioServerProxy::load_bank(String path, bool loadSamples)
    {
       return FmodAudioServer::get_singleton()->load_bank(path, loadSamples);
    }
    void FmodAudioServerProxy::unload_bank(String path)
    {
        FmodAudioServer::get_singleton()->unload_bank(path);
    }
    void FmodAudioServerProxy::unload_banks()
    {
       return FmodAudioServer::get_singleton()->unload_banks();
    }
    Studio::EventInstance *FmodAudioServerProxy::create_instance(const Vector4i p_guid)
    {
       return FmodAudioServer::get_singleton()->create_instance(p_guid);
    }
    void FmodAudioServerProxy::play_one_shot_by_id(const Vector4i p_guid, const Vector3 p_position)
    {
        FmodAudioServer::get_singleton()->play_one_shot_by_id(p_guid, p_position);
    }
    void FmodAudioServerProxy::play_one_shot_by_path(String p_path, const Vector3 p_position)
    {
        play_one_shot_by_id(path_to_guid(p_path), p_position);
    }
    void FmodAudioServerProxy::play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool nonRigidBodyVelocity)
    {
        FmodAudioServer::get_singleton()->play_one_shot_3d_attached_by_id(p_guid, p_node, nonRigidBodyVelocity);
    }
    void FmodAudioServerProxy::play_one_shot_3d_attached_by_path(String p_path, Node3D *p_node, bool nonRigidBodyVelocity)
    {
        play_one_shot_3d_attached_by_id(path_to_guid(p_path), p_node);
    }
    void FmodAudioServerProxy::play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool nonRigidBodyVelocity)
    {
        FmodAudioServer::get_singleton()->play_one_shot_2d_attached_by_id(p_guid, p_node, nonRigidBodyVelocity);
    }
    void FmodAudioServerProxy::play_one_shot_2d_attached_by_path(String p_path, Node2D *p_node, bool nonRigidBodyVelocity)
    {
        play_one_shot_2d_attached_by_id(path_to_guid(p_path), p_node);
    }
    bool FmodAudioServerProxy::any_sample_data_loading()
    {
       return FmodAudioServer::get_singleton()->any_sample_data_loading();
    }

    void FmodAudioServerProxy::attach_instance_to_node3D(Node3D *p_node, Studio::EventInstance *p_instance, bool nonRigidBodyVelocity)
    {
        // int instance_index = -1;
        // for (int i = 0; i < instances.size(); i++)
        // {
        //     if (instances[i].instance == p_instance)
        //     {
        //         instance_index = i;
        //     }
        // }
        // if (instance_index == -1)
        // {
        //     AttachedInstance instance;
        //     instance.node3D = p_node;
        //     instance.nonRigidbodyVelocity = nonRigidBodyVelocity;
        //     instance.attachment = Attachment::NODE3D;
        //     instance.lastFramePosition = p_node->get_position() * Vector3(0, -1, 0);
        //     instance.instance = p_instance;
        //     instances.push_back(instance);
        // }
        // else
        // {
        //     instances.write[instance_index].node3D = p_node;
        //     instances.write[instance_index].nonRigidbodyVelocity = nonRigidBodyVelocity;
        //     instances.write[instance_index].attachment = Attachment::NODE3D;
        //     instances.write[instance_index].lastFramePosition = p_node->get_position() * Vector3(0, -1, 0);
        //     instances.write[instance_index].instance = p_instance;
        // }
    }
    void FmodAudioServerProxy::attach_instance_to_rigid_body_3D(RigidBody3D *p_node, Studio::EventInstance *p_instance)
    {
    }
    void FmodAudioServerProxy::attach_instance_to_node2D(Node2D *p_node, Studio::EventInstance *p_instance, bool nonRigidBodyVelocity)
    {
    }
    void FmodAudioServerProxy::attach_instance_to_rigid_body_2D(RigidBody2D *p_node, Studio::EventInstance *p_instance)
    {
    }

    void FmodAudioServerProxy::detach_instance_from_node(Studio::EventInstance *p_instance)
    {
        // for (int i = 0; i < instances.size(); i++)
        // {
        //     if (instances[i].instance == p_instance)
        //     {
        //         instances.write[i] = instances[instances.size() - 1];
        //         instances.remove_at(instances.size() - 1);
        //         return;
        //     }
        // }
    }

    Vector4i FmodAudioServerProxy::path_to_guid(String p_path)
    {
        return FmodAudioServer::get_singleton()->path_to_guid(p_path);
    }
    Studio::EventDescription *FmodAudioServerProxy::get_event_description(String p_path)
    {
        return FmodAudioServer::get_singleton()->get_event_description(p_path);
    }
    Studio::EventDescription *FmodAudioServerProxy::get_event_description(Vector4i p_guid)
    {
        return FmodAudioServer::get_singleton()->get_event_description(p_guid);
    }
    void FmodAudioServerProxy::pause_all_events(bool p_pause)
    {
        FmodAudioServer::get_singleton()->pause_all_events(p_pause);
    }
    bool FmodAudioServerProxy::is_muted()
    {
        return FmodAudioServer::get_singleton()->is_muted();
    }
    void FmodAudioServerProxy::set_muted(bool p_muted)
    {
        FmodAudioServer::get_singleton()->set_muted(p_muted);
    }
    Studio::Bus *FmodAudioServerProxy::get_bus(String p_path)
    {
        return nullptr;
    }
    Studio::VCA *FmodAudioServerProxy::get_vca(String p_path)
    {
        return nullptr;
    }
    bool FmodAudioServerProxy::has_bank_loaded(String p_bankName)
    {
        return true;
    }

    bool FmodAudioServerProxy::have_all_banks_loaded()
    {
        return true;
    }
    bool FmodAudioServerProxy::have_all_master_banks_loaded()
    {
        return has_bank_loaded(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Bank_Path", "").stringify()) && has_bank_loaded(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Strings_Bank_Path", "").stringify());
    }

    // void FmodAudioServerProxy::set_listener_location(Node2D *p_node, Node2D *attenuationObject = nullptr)
    // {
    //     set_listener_location(0, p_node, attenuationObject);
    // }
    // void FmodAudioServerProxy::set_listener_location(Node2D *p_node, RigidBody2D *p_rigidBody2D, Node2D *attenuationObject = nullptr)
    // {
    //     set_listener_location(0, p_node, p_rigidBody2D, attenuationObject);
    // }
    // void FmodAudioServerProxy::set_listener_location(int listenerIndex, Node2D *p_node, RigidBody2D *rigidBody, Node2D *attenuationObject = nullptr)
    // {
    //     if (attenuationObject)
    //     {
    //         // studio_system->setListenerAttributes(listenerIndex, {}, )
    //     }
    // }
    // void FmodAudioServerProxy::set_listener_location(int listenerIndex, Node2D *p_node, Node2D *attenuationObject = nullptr)
    // {
    // }
#pragma endregion
}
