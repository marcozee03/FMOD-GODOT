#pragma once
#include "classes/rigid_body3d.hpp"
#include "fmod_event_emitter.h"
#include <classes/node3d.hpp>
#include <fmod_studio.h>
#include <variant/vector2i.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEventEmitter3D : public FmodEventEmitter<FmodEventEmitter3D, Node3D, RigidBody3D>
{
    friend class FmodEventEmitter<FmodEventEmitter3D, Node3D, RigidBody3D>;
    GDCLASS(FmodEventEmitter3D, Node3D);

  protected:
    static void _bind_methods();

  public:
    FmodEventEmitter3D() = default;
    ~FmodEventEmitter3D() override = default;
    void attach_instance_to_rigid_body(RigidBody3D *body, FMOD_STUDIO_EVENTINSTANCE *event);
    void attach_instance_to_node(Node3D *node, FMOD_STUDIO_EVENTINSTANCE *event, bool p_non_rigid_body_velocity);
    PackedStringArray _get_configuration_warnings() const override;
};

} // namespace FmodGodot
