#pragma once
#include <classes/node.hpp>
#include <fmod.h>
#include <fmod_studio.h>
#include <fmod_studio_common.h>
#include <godot_cpp/templates/hash_map.hpp>
using namespace std;
using namespace godot;
#include "fmod_bank.h"
#include <godot_cpp/classes/mutex.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/thread.hpp>

namespace FmodGodot
{

class FmodAudioServer3D : public Object
{
    GDCLASS(FmodAudioServer3D, Object)

  public:
    FmodAudioServer3D();
    ~FmodAudioServer3D();
    FMOD_SYSTEM *get_core();
    FMOD_STUDIO_SYSTEM *get_studio();
    FMOD_STUDIO_EVENTINSTANCE *create_instance(const Vector4i p_guid) const;

    void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector3 p_position = godot::Vector3()) const;
    void play_one_shot_by_path(const String &p_path, const godot::Vector3 p_position = godot::Vector3()) const;
    void play_one_shot_attached_by_id(const Vector4i p_guid, Node3D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached_by_path(const String &p_path, Node3D *p_node, bool p_non_rigid_body_velocity = false);

    void play_one_shot_rigid_body_attached_by_id(const Vector4i p_guid, RigidBody3D *p_rigid_body3d);
    void play_one_shot_rigid_body_attached_by_path(const String &p_path, RigidBody3D *p_rigid_body3d);
    bool any_sample_data_loading();

    void attach_instance(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event, bool p_non_rigid_body_velocity = false);
    void attach_instance_to_rigid_body(RigidBody3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);

    void detach_instance(FMOD_STUDIO_EVENTINSTANCE *p_event);

    Vector4i path_to_guid(const String &p_path) const;
    FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(const String &p_path) const;
    FMOD_STUDIO_EVENTDESCRIPTION *get_event_description(Vector4i p_guid) const;
    void pause_all_events(bool p_pause);

    bool is_muted() const;
    void set_muted(bool p_muted);

    FMOD_STUDIO_BUS *get_bus(const String &p_path) const;
    FMOD_STUDIO_VCA *get_vca(const String &p_path) const;
    int load_bank(const String &p_bankName, bool loadSamples = false);
    int load_bank_by_file(const String &p_path, bool loadSamples = false);
    void unload_banks();
    bool has_bank_loaded(const String &p_bankName) const;

    bool have_all_banks_loaded() const;

    void set_listener_location(int p_listener_index, Node3D *p_node, Node3D *p_attenuationObject = nullptr);
    void set_listener_location(Node3D *p_node, Node3D *p_attenuationObject = nullptr);
    void set_listener_location(int p_listener_index, RigidBody3D *p_rigidBody3D, Node3D *p_attenuationObject = nullptr);
    void set_listener_location(RigidBody3D *p_rigidBody3D, Node3D *p_attenuationObject = nullptr);
    void set_listener_rigidbody_location(int p_listenerIndex, RigidBody3D *p_rigidBody3D,
                                         Node3D *p_attenuationObject = nullptr);

  protected:
    static void _bind_methods();
};

} // namespace FmodGodot
