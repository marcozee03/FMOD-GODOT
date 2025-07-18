
#pragma once
#include <classes/node.hpp>
#include <fmod.h>
#include <fmod_studio.h>
#include <fmod_studio_common.h>
#include <godot_cpp/templates/hash_map.hpp>
using namespace std;
using namespace godot;
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/classes/mutex.hpp>
#include "fmod_bank.h"

// #ifdef TOOLS_ENABLED
#include "fmod_editor_interface.h"
// #endif

namespace FmodGodot
{

    class FmodAudioServer : public Object
    {
        GDCLASS(FmodAudioServer, Object)
    private:
        enum Attachment
        {
            RIGIDBODY2D,
            RIGIDBODY3D,
            NODE2D,
            NODE3D
        };
        struct AttachedInstance
        {
            FMOD_STUDIO_EVENTINSTANCE *instance;
            Attachment attachment;
            union
            {
                Node *node;
                Node2D *node2D;
                Node3D *node3D;
                RigidBody2D *rigidBody2D;
                RigidBody3D *rigidBody3D;
            };
            godot::Vector3 lastFramePosition;
            bool nonRigidbodyVelocity;
        };

    public:
        enum LiveUpdate : unsigned int
        {
            DISABLED = 0,
            ENABLED = 1,
            DEV_ONLY = 2
        };
        struct InitSettings
        {
            FMOD_DEBUG_FLAGS logging_level = FMOD_DEBUG_LEVEL_WARNING;
            FMOD_DEBUG_FLAGS debug_type = 0;
            FMOD_DEBUG_FLAGS debug_display = 0;
            int sample_rate = 48000;
            int dspbuffer_length = 1024;
            int dspbuffer_count = 5;
            int software_channels = 64;
            int virtual_channels = 1024;
            LiveUpdate live_update = DISABLED;
            int live_update_port = 9264;
            String encryption_key = "";
            String bank_directory = "res://";
            float doppler_scale = 1;
            float distance_factor = 1;
            float rolloff_scale = 1;
        };

    private:
        FMOD_SYSTEM *core_system;
        FMOD_STUDIO_SYSTEM *studio_system;
        bool initialized;
        bool muted;
        // core api
    public:
        FMOD_STUDIO_EVENTINSTANCE *create_instance(const Vector4i p_guid) const;

        void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector3 p_position = godot::Vector3()) const;
        void play_one_shot_by_path(const String &p_path, const godot::Vector3 p_position = godot::Vector3()) const;
        void play_one_shot_3d_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_3d_attached_by_path(const String &p_path, Node3D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_2d_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool p_non_rigid_body_velocity = false);
        void play_one_shot_2d_attached_by_path(const String &p_path, Node2D *p_node, bool p_non_rigid_body_velocity = false);

        void play_one_shot_rigid_body3d_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d);
        void play_one_shot_rigid_body3d_attached_by_path(const String &p_path, RigidBody3D *p_rigid_body3d);
        void play_one_shot_rigid_body2d_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d);
        void play_one_shot_rigid_body2d_attached_by_path(const String &p_path, RigidBody2D *p_rigid_body2d);
        bool any_sample_data_loading();

        void attach_instance_to_node3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
        void attach_instance_to_rigid_body3d(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);
        void attach_instance_to_node2D(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
        void attach_instance_to_rigid_body2d(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);

        void detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE *p_event);

        Vector4i path_to_guid(const String &p_path) const;
        FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(const String &p_path) const;
        FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(Vector4i p_guid) const;
        void pause_all_events(bool p_pause);

        bool is_muted() const;
        void set_muted(bool p_muted);

        FMOD_STUDIO_BUS *get_bus(const String &p_path) const;
        FMOD_STUDIO_VCA *get_vca(const String &p_path) const;
        FMOD_RESULT load_bank(const String &p_bankName, bool loadSamples = false);
        FMOD_RESULT load_bank_by_file(const String &p_path, bool loadSamples = false);
        // FMOD_RESULT load_bank(FmodBank bank, bool loadSamples = false);
        bool has_bank_loaded(const String &p_bankName) const;

        bool have_all_banks_loaded() const;

        void set_listener_location(Node2D *p_node, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(int p_listenerIndex, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
        void set_listener_location(int p_istenerIndex, Node2D *p_node, Node2D *p_attenuationObject = nullptr);

        void set_listener_location(Node3D *p_node, Node3D *attenuationObject = nullptr);
        void set_listener_location(RigidBody3D *rigidBody2D, Node3D *attenuationObject = nullptr);
        void set_listener_location(int listenerIndex, RigidBody3D *rigidBody, Node3D *attenuationObject = nullptr);
        void set_listener_location(int listenerIndex, Node3D *p_node, Node3D *attenuationObject = nullptr);

    private:
        void load_start_up_banks();
        Vector<AttachedInstance> instances;
        void thread_func();
        int find_instance(FMOD_STUDIO_EVENTINSTANCE *p_event);
        template <typename Node2D>
        void _attach_instance_2d(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, Attachment p_attachment, bool p_non_rigid_body_velocity = false);
        template <typename Node3D>
        void _attach_instance_3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, Attachment p_attachment, bool p_non_rigid_body_velocity = false);

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
        static FMOD_SYSTEM *get_global_core();
        static FMOD_STUDIO_SYSTEM *get_global_studio();
        static void get_global_core_ref(FMOD_SYSTEM **core);
        static void get_global_studio_ref(FMOD_STUDIO_SYSTEM **studio);
        FMOD_SYSTEM *get_core();
        FMOD_STUDIO_SYSTEM *get_studio();
        void get_core_ref(FMOD_SYSTEM **core);
        void get_studio_ref(FMOD_STUDIO_SYSTEM **studio);
        void unload_banks();
    };

    template <typename Node2D>
    inline void FmodAudioServer::_attach_instance_2d(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, Attachment p_attachment, bool p_non_rigid_body_velocity)
    {
        lock();
        int instance_index = find_instance(p_event);
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.node = p_node;
            instance.nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instance.attachment = p_attachment;
            instance.lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            instance.instance = p_event;
            FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
            FMOD_Studio_EventInstance_Set3DAttributes(instance.instance, &attributes);
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].node = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instances.write[instance_index].attachment = p_attachment;
            instances.write[instance_index].lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
            FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
            FMOD_Studio_EventInstance_Set3DAttributes(instances[instance_index].instance, &attributes);
            instances.write[instance_index].instance = p_event;
        }
        unlock();
    }

    template <typename Node3D>
    inline void FmodAudioServer::_attach_instance_3d(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, Attachment p_attachment, bool p_non_rigid_body_velocity)
    {
        lock();
        int instance_index = find_instance(p_event);
        if (instance_index == -1)
        {
            AttachedInstance instance;
            instance.node = p_node;
            instance.nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instance.attachment = p_attachment;
            instance.lastFramePosition = p_node->get_position();
            instance.instance = p_event;
            FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
            FMOD_Studio_EventInstance_Set3DAttributes(instance.instance, &attributes);
            instances.push_back(instance);
        }
        else
        {
            instances.write[instance_index].node = p_node;
            instances.write[instance_index].nonRigidbodyVelocity = p_non_rigid_body_velocity;
            instances.write[instance_index].attachment = p_attachment;
            instances.write[instance_index].lastFramePosition = p_node->get_position();
            FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
            FMOD_Studio_EventInstance_Set3DAttributes(instances[instance_index].instance, &attributes);
            instances.write[instance_index].instance = p_event;
        }
        unlock();
    }
}
