#include "fmod_listener_3d.h"
#include "fmod_listener.h"
#include <classes/rigid_body3d.hpp>
using namespace godot;
namespace FmodGodot
{

FmodListener3D::FmodListener3D() : FmodListener<FmodListener3D, Node3D, RigidBody3D>()
{
}
void FmodListener3D::_bind_methods()
{
    FmodListener<FmodListener3D, Node3D, RigidBody3D>::_bind_methods();
}
} // namespace FmodGodot
