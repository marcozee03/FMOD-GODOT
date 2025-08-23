#include "fmod_listener_2d.h"
#include "fmod_audio_server.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "globals.h"
#include <classes/rigid_body2d.hpp>
using namespace godot;
namespace FmodGodot
{
FmodListener2D::FmodListener2D() : FmodListener()
{
}
void FmodListener2D::_bind_methods()
{
    FmodListener<FmodListener2D, Node2D, RigidBody2D>::_bind_methods();
}
} // namespace FmodGodot
