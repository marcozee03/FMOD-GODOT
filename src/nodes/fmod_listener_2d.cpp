#include "fmod_listener_2d.h"
#include "fmod_audio_server.h"
#include "fmod_globals.h"
#include "globals.h"
#include <classes/rigid_body2d.hpp>
using namespace godot;
namespace FmodGodot
{
    void FmodListener2D::_physics_process(double delta)
    {
        if (is_attached_to_parent())
        {
            switch (_parent_type)
            {
            case RIGIDBODY2D:
            {
                FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(rigid_parent2D);
                FMOD_Studio_System_SetListenerAttributes(FmodAudioServer::get_singleton()->get_studio(), listener_index, &attributes, NULL);
                return;
            }
            case NODE2D:
            {
                FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(parent2D);
                FMOD_Studio_System_SetListenerAttributes(FmodAudioServer::get_singleton()->get_studio(), listener_index, &attributes, NULL);
                return;
            }
            default:
                break;
            }
        }
        FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(this);
        FMOD_Studio_System_SetListenerAttributes(FmodAudioServer::get_singleton()->get_studio(), listener_index, &attributes, NULL);
    }
    void FmodListener2D::_notification(int p_what)
    {
        switch (p_what)
        {
        case NOTIFICATION_PARENTED:
        {
            RigidBody2D *rigidbody_parent = cast_to<RigidBody2D>(get_parent());
            Node2D *node2D_parent = cast_to<Node2D>(get_parent());
            if (rigidbody_parent)
            {
                _parent_type = RIGIDBODY2D;
                rigid_parent2D = rigidbody_parent;
            }
            else if (node2D_parent)
            {
                _parent_type = NODE2D;
                parent2D = node2D_parent;
            }
            else
            {
                _parent_type = NONE;
                parent_other = nullptr;
            }
            break;
        }
        case NOTIFICATION_UNPARENTED:
        {
            parent_other = nullptr;
            _parent_type = NONE;
            break;
        }

        default:
            break;
        }
    }
    int FmodListener2D::get_listener_index()
    {
        return listener_index;
    }
    void FmodListener2D::set_listener_index(int index)
    {
        listener_index = Math::clamp(index, 0, FMOD_MAX_LISTENERS - 1);

        // FmodAudioServer::get_singleton()->
    }

    void FmodListener2D::_bind_methods()
    {
        BIND_PROPERTY(listener_index, Variant::Type::INT);
        BIND_BOOL_PROPERTY(attached_to_parent)
    }
    void FmodListener2D::set_attached_to_parent(bool p_attached_to_parent)
    {
        attached_to_parent = p_attached_to_parent;
    }
    bool FmodListener2D::is_attached_to_parent()
    {
        return attached_to_parent;
    }
}