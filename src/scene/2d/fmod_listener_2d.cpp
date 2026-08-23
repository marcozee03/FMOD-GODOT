#include "fmod_listener_2d.h"
#include <classes/rigid_body2d.hpp>
using namespace godot;
namespace FmodGodot
{
FmodListener2D::FmodListener2D()
{
}
void FmodListener2D::_bind_methods()
{
    FmodListener<FmodListener2D, Node2D, RigidBody2D>::_bind_methods();
}
} // namespace FmodGodot
extern "C"
{
#define Listener ((FmodGodot::FmodListener2D *)internal::get_object_instance_binding(handle))
    GDE_EXPORT int listener2d_get_listener_index(void *handle)
    {
        return Listener->get_listener_index();
    }
    GDE_EXPORT void listener2d_set_listener_index(void *handle, int p_listener_index)
    {
        Listener->set_listener_index(p_listener_index);
    }
    GDE_EXPORT float listener2d_get_listener_weight(void *handle)
    {
        return Listener->get_listener_weight();
    }
    GDE_EXPORT void listener2d_set_listener_weight(void *handle, float weight)
    {
        Listener->set_listener_weight(weight);
    }
    GDE_EXPORT bool listener2d_is_attached_to_parent(void *handle)
    {
        return Listener->is_attached_to_parent();
    }
    GDE_EXPORT void listener2d_set_attached_to_parent(void *handle, bool p_attached_to_parent)
    {
        return Listener->set_attached_to_parent(p_attached_to_parent);
    }
#undef Listener
}
