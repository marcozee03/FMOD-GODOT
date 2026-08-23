#include "fmod_event_emitter_3d.h"
#include "fmod_audio_server.h"
#include "variant/packed_string_array.hpp"
using namespace godot;
namespace FmodGodot
{
void FmodEventEmitter3D::attach_instance_to_rigid_body(RigidBody3D *p_body, FMOD_STUDIO_EVENTINSTANCE *p_event)
{
    FmodAudioServer::get_singleton()->attach_instance_to_rigid_body3d(p_body, p_event);
}
void FmodEventEmitter3D::attach_instance_to_node(Node3D *p_node, FMOD_STUDIO_EVENTINSTANCE *p_event,
                                                 bool p_non_rigid_body_velocity)
{
    FmodAudioServer::get_singleton()->attach_instance_to_node3d(p_node, p_event, p_non_rigid_body_velocity);
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
using namespace FmodGodot;
extern "C"
{
#define Emitter ((FmodGodot::FmodEventEmitter3D *)internal::get_object_instance_binding(handle))
    GDE_EXPORT void emitter3d_start(void *handle)
    {
        Emitter->start();
    }
    GDE_EXPORT void emitter3d_stop(void *handle)
    {
        Emitter->stop();
    }
    GDE_EXPORT bool emitter3d_is_attached_to_rigidbody(void *handle)
    {
        return Emitter->is_attached_to_rigidbody();
    }
    GDE_EXPORT void emitter3d_set_attached_to_rigidbody(void *handle, bool p_attached)
    {
        return Emitter->set_attached_to_rigidbody(p_attached);
    }
    GDE_EXPORT FMOD_GUID emitter3d_get_event(void *handle)
    {
        return FmodGodot::cast_to_fmod_guid(Emitter->get_event());
    }
    GDE_EXPORT void emitter3d_set_event(void *handle, Vector4i p_event)
    {
        Emitter->set_event(p_event);
    }

    GDE_EXPORT bool emitter3d_is_one_shot(void *handle)
    {
        return Emitter->is_one_shot();
    }
    GDE_EXPORT void emitter3d_set_one_shot(void *handle, bool p_one_shot)
    {
        Emitter->set_one_shot(p_one_shot);
    }

    GDE_EXPORT bool emitter3d_is_trigger_once(void *handle)
    {
        return Emitter->is_trigger_once();
    }
    GDE_EXPORT void emitter3d_set_trigger_once(void *handle, bool p_trigger_once)
    {
        Emitter->set_trigger_once(p_trigger_once);
    }

    GDE_EXPORT bool emitter3d_is_non_rigid_body_velocity(void *handle)
    {
        return Emitter->is_non_rigid_body_velocity();
    }
    GDE_EXPORT void emitter3d_set_non_rigid_body_velocity(void *handle, bool p_non_rigid_body_velocity)
    {
        Emitter->set_non_rigid_body_velocity(p_non_rigid_body_velocity);
    }

    GDE_EXPORT bool emitter3d_is_preload_sample_data(void *handle)
    {
        return Emitter->is_preload_sample_data();
    }
    GDE_EXPORT void emitter3d_set_preload_sample_data(void *handle, bool p_preload_sample_data)
    {
        Emitter->set_preload_sample_data(p_preload_sample_data);
    }

    GDE_EXPORT bool emitter3d_is_override_attenuation(void *handle)
    {
        return Emitter->is_override_attenuation();
    }
    GDE_EXPORT void emitter3d_set_override_attenuation(void *handle, bool p_override_attenuation)
    {
        Emitter->set_override_attenuation(p_override_attenuation);
    }

    GDE_EXPORT float emitter3d_get_volume(void *handle)
    {
        return Emitter->get_volume();
    }
    GDE_EXPORT void emitter3d_set_volume(void *handle, float p_volume)
    {
        return Emitter->set_volume(p_volume);
    }

    GDE_EXPORT float emitter3d_get_attenuation_min(void *handle)
    {
        return Emitter->get_attenuation_min();
    }
    GDE_EXPORT void emitter3d_set_attenuation_min(void *handle, float p_attenuation_min)
    {
        Emitter->set_attenuation_min(p_attenuation_min);
    }

    GDE_EXPORT float emitter3d_get_attenuation_max(void *handle)
    {
        return Emitter->get_attenuation_max();
    }
    GDE_EXPORT void emitter3d_set_attenuation_max(void *handle, float p_attenuation_max)
    {
        Emitter->set_attenuation_max(p_attenuation_max);
    }

    GDE_EXPORT bool emitter3d_is_allow_fadeout(void *handle)
    {
        return Emitter->is_allow_fadeout();
    }
    GDE_EXPORT void emitter3d_set_allow_fadeout(void *handle, bool p_allow_fadeout)
    {
        return Emitter->set_allow_fadeout(p_allow_fadeout);
    }

    GDE_EXPORT void emitter3d_set_parameter(void *handle, String &p_name, float p_value)
    {
        Emitter->set_parameter(p_name, p_value);
    }
    GDE_EXPORT void emitter3d_set_parameter_by_id(void *handle, GD_PARAMETER_ID &p_id, float p_value)
    {
        Emitter->set_parameter_by_id(p_id, p_value);
    }
    GDE_EXPORT float emitter3d_get_parameter(void *handle, String &p_name)
    {
        return Emitter->get_parameter(p_name);
    }
    GDE_EXPORT float emitter3d_get_parameter_by_id(void *handle, GD_PARAMETER_ID &p_id)
    {
        return Emitter->get_parameter_by_id(p_id);
    }

    GDE_EXPORT bool emitter3d_is_playing(void *handle)
    {
        return Emitter->is_playing();
    }
    GDE_EXPORT bool emitter3d_is_paused(void *handle)
    {
        return Emitter->is_paused();
    }
    GDE_EXPORT void emitter3d_set_paused(void *handle, bool p_is_paused)
    {
        return Emitter->set_paused(p_is_paused);
    }
#undef Emitter
}
