#pragma once
#include "fmod_event_emitter.h"
#include "variant/packed_string_array.hpp"
#include <classes/node2d.hpp>
#include <classes/rigid_body2d.hpp>
#include <fmod_studio.h>
#include <variant/vector2i.hpp>
namespace FmodGodot
{

class FmodEventEmitter2D : public FmodEventEmitter<FmodEventEmitter2D, godot::Node2D, godot::RigidBody2D>
{
    friend class FmodEventEmitter<FmodEventEmitter2D, Node2D, RigidBody2D>;
    GDCLASS(FmodEventEmitter2D, godot::Node2D);

  protected:
    static void _bind_methods();

  public:
    FmodEventEmitter2D() = default;
    ~FmodEventEmitter2D() override = default;
    void attach_instance_to_rigid_body(RigidBody2D *body, FMOD_STUDIO_EVENTINSTANCE *event) ;
    void attach_instance_to_node(Node2D *node, FMOD_STUDIO_EVENTINSTANCE *event, bool p_non_rigid_body_velocity) ;
    PackedStringArray _get_configuration_warnings() const override;
};
} // namespace FmodGodot
