
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
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/thread.hpp>

namespace FmodGodot
{

class FmodAudioServer2D : public Object
{
    // GDCLASS(FmodAudioServer2D, Object)
  private:
   
  public:

  private:

  public:
    FmodAudioServer2D();
    ~FmodAudioServer2D();
    void play_one_shot_by_id(const Vector4i p_guid, const godot::Vector2 p_position = godot::Vector2(), float depth = 0) const;
    void play_one_shot_by_path(const String &p_path, const godot::Vector2 p_position = godot::Vector2(), float depth = 0) const;
    void play_one_shot_attached_by_id(const Vector4i p_guid, Node2D *p_node, bool p_non_rigid_body_velocity = false);
    void play_one_shot_attached_by_path(const String &p_path, Node2D *p_node,
                                           bool p_non_rigid_body_velocity = false);

    void play_one_shot_rigid_body_attached_by_id(const Vector4i p_guid, RigidBody2D *p_rigid_body2d);
    void play_one_shot_rigid_body_attached_by_path(const String &p_path, RigidBody2D *p_rigid_body2d);

    void attach_instance_to_node(Node2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                   bool p_non_rigid_body_velocity = false);
    void attach_instance_to_rigid_body(RigidBody2D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event);

    void detach_instance_from_node(FMOD_STUDIO_EVENTINSTANCE *p_event);


    void set_listener_location(int p_listener_index, Node2D *p_node, Node2D *p_attenuationObject = nullptr);
    void set_listener_location(Node2D *p_node, Node2D *p_attenuationObject = nullptr);
    void set_listener_location(int p_listener_index, RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
    void set_listener_location(RigidBody2D *p_rigidBody2D, Node2D *p_attenuationObject = nullptr);
    void set_listener_rigidbody_location(int p_listenerIndex, RigidBody2D *p_rigidBody2D,
                                            Node2D *p_attenuationObject = nullptr);
    void set_listener_location(int p_istenerIndex, Node2D *p_node, Node2D *p_attenuationObject = nullptr);

  protected:
    static void _bind_methods();
};

} // namespace FmodGodot
VARIANT_ENUM_CAST(FmodGodot::FmodAudioServer2D::LiveUpdate);
