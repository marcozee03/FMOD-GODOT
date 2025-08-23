#pragma once
#include "fmod_listener.h"
#include <classes/node.hpp>
#include <classes/rigid_body2d.hpp>
#include <godot_cpp/classes/node2d.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodListener2D : public FmodListener<FmodListener2D, Node2D, RigidBody2D>
{
    GDCLASS(FmodListener2D, Node2D);

  protected:
    static void _bind_methods();

  public:
    FmodListener2D();
    ~FmodListener2D() = default;
};
} // namespace FmodGodot
