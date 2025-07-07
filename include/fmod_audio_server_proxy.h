
#pragma once
#include <classes/node.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot_cpp/templates/hash_map.hpp>
using namespace std;
using namespace godot;
using namespace FMOD;
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/mutex.hpp>
namespace FmodGodot
{

    class FmodAudioServerProxy : public Object
    {
        GDCLASS(FmodAudioServerProxy, Object)

    public:
        Studio::EventInstance *create_instance(const Vector4i p_guid);

        void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector3 p_position = godot::Vector3());
        void play_one_shot_by_path(const String p_path, const godot::Vector3 p_position = godot::Vector3());
        void play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool nonRigidBodyVelocity = false);
        void play_one_shot_3d_attached_by_path(const String p_path, Node3D *p_node, bool nonRigidBodyVelocity = false);
        void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool nonRigidBodyVelocity = false);
        void play_one_shot_2d_attached_by_path(const String p_path, Node2D *p_node, bool nonRigidBodyVelocity = false);
        bool any_sample_data_loading();

        void attach_instance_to_node3D(Node3D *p_node, Studio::EventInstance *p_instance, bool nonRigidBodyVelocity = false);
        void attach_instance_to_rigid_body_3D(RigidBody3D *p_node, Studio::EventInstance *p_instance);
        void attach_instance_to_node2D(Node2D *p_node, Studio::EventInstance *p_instance, bool nonRigidBodyVelocity = false);
        void attach_instance_to_rigid_body_2D(RigidBody2D *p_node, Studio::EventInstance *p_instance);

        void detach_instance_from_node(Studio::EventInstance *p_instance);

        Vector4i path_to_guid(String p_path);
        Studio::EventDescription *get_event_description(String p_path);
        Studio::EventDescription *get_event_description(Vector4i p_guid);
        void pause_all_events(bool p_pause);

        bool is_muted();
        void set_muted(bool p_muted);

        Studio::Bus *get_bus(String p_path);
        Studio::VCA *get_vca(String p_path);
        FMOD_RESULT load_bank(String p_bankName, bool loadSamples = false);
        void unload_bank(String p_bankName);
        bool has_bank_loaded(String p_bankName);

        bool have_all_banks_loaded();
        bool have_all_master_banks_loaded();

        // void set_listener_location(Node2D *p_node, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(Node2D *p_node, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(int p_listenerIndex, Node2D *p_node, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        // void set_listener_location(int p_istenerIndex, Node2D *p_node, Node2D *p_attenuationObject = nullptr);

        // void set_listener_location(Node3D *p_node, Node2D *attenuationObject = nullptr);
        // void set_listener_location(Node3D *p_node, RigidBody2D *rigidBody2D, Node3D *attenuationObject = nullptr);
        // void set_listener_location(int listenerIndex, Node3D *p_node, RigidBody2D *rigidBody, Node3D *attenuationObject = nullptr);
        // void set_listener_location(int listenerIndex, Node3D *p_node, Node3D *attenuationObject = nullptr);

    private:

        void LoadMasterBanks();

    private:


    public:


    protected:
        static void _bind_methods();
        // void _notification(int p_what);

    public:
        FmodAudioServerProxy();
        ~FmodAudioServerProxy();
        void unload_banks();
    };

}
