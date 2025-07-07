
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
#include "fmod_bank.h"
namespace FmodGodot
{

    class FmodAudioServer : public Object
    {
        GDCLASS(FmodAudioServer, Object)

        enum Attachment
        {
            RIGIDBODY2D,
            RIGIDBODY3D,
            NODE2D,
            NODE3D
        };

        struct AttachedInstance
        {
            Studio::EventInstance *instance;
            Attachment attachment;
            union
            {
                Node2D *node2D;
                Node3D *node3D;
                RigidBody2D *rigidBody2D;
                RigidBody3D *rigidBody3D;
            };
            godot::Vector3 lastFramePosition;
            bool nonRigidbodyVelocity;
        };

    private:
        System *core_system;
        Studio::System *studio_system;
        bool initialized;
        bool muted;
        // core api
    public:
        Studio::EventInstance *create_instance(const Vector4i p_guid) const;

        void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector3 p_position = godot::Vector3()) const;
        void play_one_shot_by_path(const String p_path, const godot::Vector3 p_position = godot::Vector3()) const;
        void play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_3d_attached_by_path(const String p_path, Node3D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_2d_attached_by_path(const String p_path, Node2D *p_node, bool p_non_rigid_body_velocity = false);

        void play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d);
        void play_one_shot_rigid_body3d_attached_by_path(const String p_path, RigidBody3D *p_rigid_body3d);
        void play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d);
        void play_one_shot_rigid_body2d_attached_by_path(const String p_path, RigidBody2D *p_rigid_body2d);
        bool any_sample_data_loading();

        void attach_instance_to_node3d(Node3D *p_node, Studio::EventInstance *p_instance, bool p_non_rigid_body_velocity = false);
        void attach_instance_to_rigid_body3d(RigidBody3D *p_node, Studio::EventInstance *p_instance);
        void attach_instance_to_node2D(Node2D *p_node, Studio::EventInstance *p_instance, bool p_non_rigid_body_velocity = false);
        void attach_instance_to_rigid_body2d(RigidBody2D *p_node, Studio::EventInstance *p_instance);

        void detach_instance_from_node(Studio::EventInstance *p_instance);

        Vector4i path_to_guid(String p_path) const;
        Studio::EventDescription *get_event_description(String p_path) const;
        Studio::EventDescription *get_event_description(Vector4i p_guid) const;
        void pause_all_events(bool p_pause);

        bool is_muted() const;
        void set_muted(bool p_muted);

        Studio::Bus *get_bus(String p_path) const;
        Studio::VCA *get_vca(String p_path) const;
        FMOD_RESULT load_bank(String p_bankName, bool loadSamples = false);
        // FMOD_RESULT load_bank(FmodBank bank, bool loadSamples = false);
        void unload_bank(String p_bankName);
        bool has_bank_loaded(String p_bankName) const;

        bool have_all_banks_loaded() const;
        bool have_all_master_banks_loaded() const;

        void set_listener_location(Node2D *p_node, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(int p_listenerIndex, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(int p_istenerIndex, Node2D *p_node, Node2D *p_attenuationObject = nullptr);

        void set_listener_location(Node3D *p_node, Node3D *attenuationObject = nullptr);
        void set_listener_location(RigidBody3D *rigidBody2D, Node3D *attenuationObject = nullptr);
        void set_listener_location(int listenerIndex, RigidBody3D *rigidBody, Node3D *attenuationObject = nullptr);
        void set_listener_location(int listenerIndex, Node3D *p_node, Node3D *attenuationObject = nullptr);

    private:
        Studio::Bank *Master;
        Studio::Bank *Masterstrings;
        HashMap<String, size_t> banks;
        void LoadMasterBanks();
        Vector<AttachedInstance> instances;
        void thread_func();

    private:
        bool thread_exited;
        mutable bool exit_thread = false;
        Ref<Thread> thread;
        Ref<Mutex> mutex;
        void _physics_process();

    public:
        static FmodAudioServer *singleton;
        static FmodAudioServer *get_singleton();
        FMOD_RESULT init();
        void lock();
        void unlock();
        void finish();

    protected:
        static void _bind_methods();
        // void _notification(int p_what);

    public:
        FmodAudioServer();
        ~FmodAudioServer();
        static System *get_global_core();
        static Studio::System *get_global_studio();
        static void get_global_core_ref(System **core);
        static void get_global_studio_ref(Studio::System **studio);
        static FMOD_RESULT load_global_bank(String path);
        static void unload_global_bank(String path);
        static void unload_global_banks();
        System *get_core();
        Studio::System *get_studio();
        void get_core_ref(System **core);
        void get_studio_ref(Studio::System **studio);
        void unload_banks();
    };

}
