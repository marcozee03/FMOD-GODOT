#pragma once
#include "classes/object.hpp"
#include "classes/wrapped.hpp"
#include "conversions.h"
#include "core/print_string.hpp"
#include "fmod_bank.h"
#include "fmod_init_settings.h"
#include "variant/variant.hpp"
#include <classes/node.hpp>
#include <fmod.h>
#include <fmod_studio.h>
#include <fmod_studio_common.h>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/thread.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/mutex.hpp>

using namespace std;
using namespace godot;
namespace FmodGodot
{

class FmodAudioServer : public Object
{
    GDCLASS(FmodAudioServer, Object)
  private:
    static String _get_version_number();
    Handle _get_core();
    Handle _get_studio();

    Handle _create_instance(const Vector4i &p_guid) const;
    Handle _create_instance_by_path(const String &p_path) const;

    void _attach_instance_to_node3d(Node3D *p_node, Handle p_event, bool p_non_rigid_body_velocity = false);
    void _attach_instance_to_rigid_body3d(RigidBody3D *p_node, Handle p_event);
    void _attach_instance_to_node2d(Node2D *p_node, Handle p_event, bool p_non_rigid_body_velocity = false);

    void _attach_instance_to_rigid_body2d(RigidBody2D *p_node, Handle p_event);

    void _detach_instance(Handle p_event);

    Handle _get_event_description(const String &p_path) const;
    Handle _get_event_description(const Vector4i &p_guid) const;

    Handle _get_bus(const Vector4i &p_guid) const;
    Handle _get_vca(const Vector4i &p_guid) const;

    Handle _get_bus_by_path(const String &p_path) const;
    Handle _get_vca_by_path(const String &p_path) const;

  private:
    enum Attachment
    {
        RIGIDBODY2D,
        RIGIDBODY3D,
        NODE2D,
        NODE3D
    };
    template <typename T> static Attachment get_attachment_type()
    {
        if constexpr (std::is_base_of_v<RigidBody2D, T>)
        {
            return RIGIDBODY2D;
        }
        else if constexpr (std::is_base_of_v<Node2D, T>)
        {
            return NODE2D;
        }
        else if constexpr (std::is_base_of_v<RigidBody3D, T>)
        {
            return RIGIDBODY3D;
        }
        else if constexpr (std::is_base_of_v<Node3D, T>)
        {
            return NODE3D;
        }
    }
    struct AttachedInstance
    {
        FMOD_STUDIO_EVENTINSTANCE *instance;
        Attachment attachment;
        union {
            Node *node;
            Node2D *node2D;
            Node3D *node3D;
            RigidBody2D *rigidBody2D;
            RigidBody3D *rigidBody3D;
        };
        Vector3 lastFramePosition;
        bool nonRigidbodyVelocity;
    };

  private:
    FMOD_SYSTEM *core_system;
    FMOD_STUDIO_SYSTEM *studio_system;
    CharString encryption_key;
    bool initialized;
    bool muted;
    LocalVector<Ref<FmodBank>> start_up_banks;
    bool live_update_connected;
    bool start_up_banks_loaded;

    // core api

  public:
    friend FMOD_RESULT F_CALL fmod_studio_system_callback(FMOD_STUDIO_SYSTEM *p_system,
                                                          FMOD_STUDIO_SYSTEM_CALLBACK_TYPE p_type, void *p_commanddata,
                                                          void *p_userdata);
    FmodAudioServer();
    ~FmodAudioServer();
    FMOD_SYSTEM *get_core();
    const FMOD_SYSTEM *get_core() const;
    FMOD_STUDIO_SYSTEM *get_studio();
    const FMOD_STUDIO_SYSTEM *get_studio() const;
    bool is_live_update_connected() const;

    FMOD_STUDIO_EVENTINSTANCE *create_instance(const Vector4i &p_guid) const;

    // =======Playing-Audio============
  private:
    template <typename T>
        requires((std::is_base_of_v<godot::Node2D, T> || std::is_base_of_v<godot::Node3D, T>) &&
                 !(std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
    void _play_one_shot_attached(const Vector4i &p_guid, T *p_node, bool p_non_rigid_body_velocity = false);
    template <typename T>
        requires((std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
    void _play_one_shot_attached(const Vector4i &p_guid, T *p_rigidbody);

  public:
    void play_one_shot(const Vector4i &p_guid, const Vector3 &p_position = Vector3());
    void play_one_shot(const String &p_path, const Vector3 &p_position = Vector3());

    void play_one_shot_attached(const Vector4i &p_guid, Node3D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached(const String &p_path, Node3D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached(const Vector4i &p_guid, RigidBody3D *p_rigid_body3d);
    void play_one_shot_attached(const String &p_path, RigidBody3D *p_rigid_body3d);

    void play_one_shot_attached(const Vector4i &p_guid, Node2D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached(const String &p_path, Node2D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached(const Vector4i &p_guid, RigidBody2D *p_rigid_body2d);
    void play_one_shot_attached(const String &p_path, RigidBody2D *p_rigid_body2d);
    bool any_sample_data_loading() const;

    void attach_instance(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
    void attach_instance(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);
    void attach_instance(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
    void attach_instance(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);

    void detach_instance(FMOD_STUDIO_EVENTINSTANCE *p_event);

    void wait_for_all_sample_loading();

    Vector4i path_to_guid(const String &p_path) const;
    FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(const String &p_path) const;
    FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(const Vector4i &p_guid) const;
    void pause_all_events(bool p_pause);

    bool is_muted() const;
    void set_muted(bool p_muted);

    FMOD_STUDIO_BUS *get_bus(const Vector4i &p_guid) const;
    FMOD_STUDIO_VCA *get_vca(const Vector4i &p_guid) const;

    FMOD_STUDIO_BUS *get_bus(const String &p_path) const;
    FMOD_STUDIO_VCA *get_vca(const String &p_path) const;

    void unload_banks();
    bool has_bank_loaded(const String &p_bank_path_or_guid) const;

    //=======Listeners===========
  private:
    template <typename T, typename A>
        requires((std::is_base_of_v<godot::Node2D, T> && std::is_base_of_v<godot::Node2D, A>) ||
                 (std::is_base_of_v<godot::Node3D, T> && std::is_base_of_v<godot::Node3D, A>))
    void _set_listener_location(T *p_node, int p_listener_index = 0, A *p_attenuation_object = nullptr);

  public:
    void set_listener_location(Node2D *p_node, int p_listener_index = 0, Node2D *p_attenuation_object = nullptr);
    void set_listener_location(RigidBody2D *p_rigid_body2d, int p_listener_index = 0,
                               Node2D *p_attenuation_object = nullptr);

    void set_listener_location(Node3D *p_node, int p_listener_index, Node3D *p_attenuation_object = nullptr);
    void set_listener_location(RigidBody3D *p_rigid_body3d, int p_listener_index,
                               Node3D *p_attenuation_object = nullptr);

  public: // Internal unexposed
    void _load_start_up_banks();
    void _reload_start_up_banks();
    void _unload_start_up_banks();

  private:
    template <typename T>
        requires((std::is_base_of_v<godot::Node2D, T> || std::is_base_of_v<godot::Node3D, T>) &&
                 !(std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
    void _attach_instance(T *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
    template <typename T>
        requires((std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
    void _attach_instance(T *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);
    LocalVector<AttachedInstance> instances;
    void _thread_func();
    int _find_instance(FMOD_STUDIO_EVENTINSTANCE *p_event);

    bool thread_exited;
    mutable bool exit_thread = false;
    Ref<Thread> thread;
    Mutex mutex;
    void _physics_process();

#ifdef DEBUG_ENABLED
  public:
    enum CPUUsage
    {
        USAGE_STUDIO_UPDATE,
        USAGE_DSP,
        USAGE_STREAM,
        USAGE_GEOMETRY,
        USAGE_UPDATE,
        USAGE_CONVULUTION1,
        USAGE_CONVULUTION2,
    };

  private:
    int track_memory();
    float track_cpu(CPUUsage p_usage);

  public:
    bool debugger_capture(const String &p_message, const Array &p_data);
#endif

  public:
    static FmodAudioServer *singleton;
    static FmodAudioServer *get_singleton();
    FMOD_RESULT init_with_project_settings();
    FMOD_RESULT init(const InitSettings &p_settings);

    void lock();
    void unlock();
    void finish();

  protected:
    static void _bind_methods();
};
template <typename T, typename A>
    requires((std::is_base_of_v<godot::Node2D, T> && std::is_base_of_v<godot::Node2D, A>) ||
             (std::is_base_of_v<godot::Node3D, T> && std::is_base_of_v<godot::Node3D, A>))
inline void FmodAudioServer::_set_listener_location(T *p_node, int p_listener_index, A *p_attenuation_object)
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

template <typename T>
    requires((std::is_base_of_v<godot::Node2D, T> || std::is_base_of_v<godot::Node3D, T>) &&
             !(std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
inline void FmodAudioServer::_play_one_shot_attached(const Vector4i &p_guid, T *p_node, bool p_non_rigid_body_velocity)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);
    attach_instance(p_node, event, p_non_rigid_body_velocity);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}

template <typename T>
    requires((std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
inline void FmodAudioServer::_play_one_shot_attached(const Vector4i &p_guid, T *p_rigidbody)
{
    FMOD_STUDIO_EVENTINSTANCE *event = create_instance(p_guid);
    attach_instance(p_rigidbody, event);
    FMOD_Studio_EventInstance_Start(event);
    FMOD_Studio_EventInstance_Release(event);
}

template <typename T>
    requires((std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
inline void FmodAudioServer::_attach_instance(T *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    lock();
    int instance_index = _find_instance(p_event);
    if (!p_node->is_inside_tree())
    {
        print_error(vformat("Cannot Attach Fmod Event to node '%s' not inside tree", p_node->get_name()));
    }
    if (instance_index == -1)
    {
        AttachedInstance instance;
        instance_index = instances.size();
        instances.push_back(instance);
    }
    else
    {
        instances[instance_index].node = p_node;
        instances[instance_index].nonRigidbodyVelocity = false;
        instances[instance_index].attachment = get_attachment_type<T>();
        if constexpr (std::is_base_of_v<Node2D, T>)
        {
            instances[instance_index].lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
        }
        else if constexpr (std::is_base_of_v<Node3D, T>)
        {
            instances[instance_index].lastFramePosition = p_node->get_position();
        }
        FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
        FMOD_Studio_EventInstance_Set3DAttributes(instances[instance_index].instance, &attributes);
        instances[instance_index].instance = p_event;
    }
    unlock();
}

template <typename T>
    requires((std::is_base_of_v<godot::Node2D, T> || std::is_base_of_v<godot::Node3D, T>) &&
             !(std::is_base_of_v<godot::RigidBody2D, T> || std::is_base_of_v<godot::RigidBody3D, T>))
inline void FmodAudioServer::_attach_instance(T *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                              bool p_non_rigid_body_velocity)
{
    lock();
    int instance_index = _find_instance(p_event);
    if (!p_node->is_inside_tree())
    {
        print_error(vformat("Cannot Attach Fmod Event to node '%s' not inside tree", p_node->get_name()));
    }
    if (instance_index == -1)
    {
        AttachedInstance instance;
        instance_index = instances.size();
        instances.push_back(instance);
    }
    else
    {
        instances[instance_index].node = p_node;
        instances[instance_index].nonRigidbodyVelocity = p_non_rigid_body_velocity;
        instances[instance_index].attachment = get_attachment_type<T>();
        if constexpr (std::is_base_of_v<Node2D, T>)
        {
            instances[instance_index].lastFramePosition = {p_node->get_position().x, p_node->get_position().y, 0};
        }
        else if constexpr (std::is_base_of_v<Node3D, T>)
        {
            instances[instance_index].lastFramePosition = p_node->get_position();
        }
        FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_node);
        FMOD_Studio_EventInstance_Set3DAttributes(instances[instance_index].instance, &attributes);
        instances[instance_index].instance = p_event;
    }
    unlock();
}
} // namespace FmodGodot
VARIANT_ENUM_CAST(FmodGodot::InitSettings::LiveUpdate);
#ifdef DEBUG_ENABLED
VARIANT_ENUM_CAST(FmodGodot::FmodAudioServer::CPUUsage);
#endif
