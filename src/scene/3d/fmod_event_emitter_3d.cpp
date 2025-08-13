#include "fmod_event_emitter_3d.h"
#include "fmod_audio_server.h"
#include "globals.h"
#include "variant/packed_string_array.hpp"
using namespace godot;
namespace FmodGodot
{
void FmodEventEmitter3D::attach_instance_to_rigid_body(RigidBody3D *body, FMOD_STUDIO_EVENTINSTANCE *event)
{
    FmodAudioServer::get_singleton()->attach_instance_to_rigid_body3d(body, event);
}
void FmodEventEmitter3D::attach_instance_to_node(Node3D *node, FMOD_STUDIO_EVENTINSTANCE *event,
                                                 bool p_non_rigid_body_velocity)
{
    FmodAudioServer::get_singleton()->attach_instance_to_node3d(node, event, p_non_rigid_body_velocity);
}
PackedStringArray FmodEventEmitter3D::_get_configuration_warnings() const
{
    if (is_attached_to_rigidbody() && !cast_to<RigidBody2D>(get_parent()))
    {
        return {"Parent must be a rigidbody2D when attached_to_rigidbody is true"};
    }
    return {};
}
void FmodEventEmitter3D::_bind_methods()
{
    FmodEventEmitter<FmodEventEmitter3D, Node3D, RigidBody3D>::_bind_methods();
}
} // namespace FmodGodot
