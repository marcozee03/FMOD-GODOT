#pragma once
#include "fmod_listener.h"
#include <classes/node.hpp>
#include <classes/rigid_body3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodListener3D : public FmodListener<FmodListener3D, Node3D, RigidBody3D>
{
    GDCLASS(FmodListener3D, Node3D);

  protected:
    static void _bind_methods();

  public:
    FmodListener3D(/* args */);
    ~FmodListener3D() = default;
};
} // namespace FmodGodot
