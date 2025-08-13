#include "fmod_event_emitter_2d.h"
#include "classes/rigid_body2d.hpp"
#include "core/class_db.hpp"
#include "fmod_audio_server.h"
#include "fmod_event_emitter.h"
#include "globals.h"
#include "variant/packed_string_array.hpp"
#include <classes/node2d.hpp>
using namespace godot;
namespace FmodGodot
{
void FmodEventEmitter2D::attach_instance_to_rigid_body(RigidBody2D *body, FMOD_STUDIO_EVENTINSTANCE *event)
{
    FmodAudioServer::get_singleton()->attach_instance_to_rigid_body2d(body, event);
}
void FmodEventEmitter2D::attach_instance_to_node(Node2D *node, FMOD_STUDIO_EVENTINSTANCE *event,
                                                 bool p_non_rigid_body_velocity)
{
    FmodAudioServer::get_singleton()->attach_instance_to_node2D(node, event, p_non_rigid_body_velocity);
}
PackedStringArray FmodEventEmitter2D::_get_configuration_warnings() const
{
    if (is_attached_to_rigidbody() && !cast_to<RigidBody2D>(get_parent()))
    {
        return {"Parent must be a rigidbody2D when attached_to_rigidbody is true"};
    }
    return {};
}

void FmodEventEmitter2D::_bind_methods()
{
    FmodEventEmitter<FmodEventEmitter2D, Node2D, RigidBody2D>::_bind_methods();
}

} // namespace FmodGodot
