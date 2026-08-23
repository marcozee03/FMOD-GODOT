#include "fmod_listener_3d.h"
#include "fmod_listener.h"
#include <classes/rigid_body3d.hpp>
using namespace godot;
namespace FmodGodot
{

FmodListener3D::FmodListener3D()
{
}
void FmodListener3D::_bind_methods()
{
    FmodListener<FmodListener3D, Node3D, RigidBody3D>::_bind_methods();
}
} // namespace FmodGodot
extern "C"
{
#define Listener ((FmodGodot::FmodListener3D *)internal::get_object_instance_binding(handle))

    GDE_EXPORT FmodGodot::FmodListener3D *listener3d_new()
    {
        return memnew(FmodGodot::FmodListener3D);
    }
    GDE_EXPORT int listener3d_get_listener_index(void *handle)
    {
        return Listener->get_listener_index();
    }
    GDE_EXPORT void listener3d_set_listener_index(void *handle, int p_listener_index)
    {
        Listener->set_listener_index(p_listener_index);
    }
    GDE_EXPORT float listener3d_get_listener_weight(void *handle)
    {
        return Listener->get_listener_weight();
    }
    GDE_EXPORT void listener3d_set_listener_weight(void *handle, float p_weight)
    {
        Listener->set_listener_weight(p_weight);
    }
    GDE_EXPORT bool listener3d_is_attached_to_parent(void *handle)
    {
        return Listener->is_attached_to_parent();
    }
    GDE_EXPORT void listener3d_set_attached_to_parent(void *handle, bool p_attached_to_parent)
    {
        return Listener->set_attached_to_parent(p_attached_to_parent);
    }
#undef Listener
}
