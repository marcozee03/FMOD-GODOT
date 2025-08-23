#pragma once
#include "classes/global_constants.hpp"
#include "classes/node.hpp"
#include "classes/rigid_body2d.hpp"
#include "core/math.hpp"
#include "core/print_string.hpp"
#include "fmod_audio_server.h"
#include "fmod_common.h"
#include "fmod_studio.h"
#include "globals.h"
#include "variant/string_name.hpp"
#include "variant/variant.hpp"
#include <classes/node.hpp>
using namespace godot;
namespace FmodGodot
{
template <class Derived, class NodeType, class RigidBody> class FmodListener : public NodeType
{
  private:
    int listener_index = 0;
    int weight = 1;
    enum ParentType
    {
        NODE,
        RIGIDBODY,
        NONE,
    };
    union {
        NodeType *parent_node;
        RigidBody *parent_rigidbody;
        Node *parent_other;
    };
    ParentType _parent_type;
    bool attached_to_parent = false;

    void unparent();

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    FmodListener();
    virtual ~FmodListener() = default;
    int get_listener_index() const;
    void set_listener_index(int p_listener_index);
    float get_listener_weight() const;
    void set_listener_weight(float weight);
    bool is_attached_to_parent() const;
    void set_attached_to_parent(bool p_attached_to_parent);
    static const StringName &get_class_static();
};

template <class Derived, class NodeType, class RigidBody> FmodListener<Derived, NodeType, RigidBody>::FmodListener()
{
    this->set_physics_process(true);
}
template <class Derived, class NodeType, class RigidBody>
int FmodListener<Derived, NodeType, RigidBody>::get_listener_index() const
{
    return listener_index;
}

template <class Derived, class NodeType, class RigidBody>
void FmodListener<Derived, NodeType, RigidBody>::set_listener_index(int p_listener_index)
{
    int count;
    FMOD_Studio_System_GetNumListeners(FmodAudioServer::get_singleton()->get_studio(), &count);
    listener_index = Math::clamp(p_listener_index, 0, FMOD_MAX_LISTENERS);
    if (count < listener_index + +1)
    {
        FMOD_Studio_System_SetNumListeners(FmodAudioServer::get_singleton()->get_studio(), listener_index + 1);
    }
}

template <class Derived, class NodeType, class RigidBody> void FmodListener<Derived, NodeType, RigidBody>::unparent()
{
    parent_other = nullptr;
    _parent_type = NONE;
}

template <class Derived, class NodeType, class RigidBody>
bool FmodListener<Derived, NodeType, RigidBody>::is_attached_to_parent() const
{
    return attached_to_parent;
}

template <class Derived, class NodeType, class RigidBody>
void FmodListener<Derived, NodeType, RigidBody>::set_attached_to_parent(bool p_attached_to_parent)
{
    attached_to_parent = p_attached_to_parent;
}

template <class Derived, class NodeType, class RigidBody>
void FmodListener<Derived, NodeType, RigidBody>::set_listener_weight(float p_listener_weight)
{
    FMOD_Studio_System_SetListenerWeight(FmodAudioServer::get_singleton()->get_studio(), listener_index,
                                         p_listener_weight);
}

template <class Derived, class NodeType, class RigidBody>
float FmodListener<Derived, NodeType, RigidBody>::get_listener_weight() const
{
    float weight;
    FMOD_Studio_System_GetListenerWeight(FmodAudioServer::get_singleton()->get_studio(), listener_index, &weight);
    return weight;
}

template <class Derived, class NodeType, class RigidBody>
void FmodListener<Derived, NodeType, RigidBody>::_notification(int p_what)
{
    switch (p_what)
    {
    case Node::NOTIFICATION_PARENTED: {
        RigidBody *rigidbody_parent = Object::cast_to<RigidBody>(this->get_parent());
        NodeType *node2D_parent = Object::cast_to<NodeType>(this->get_parent());
        if (rigidbody_parent)
        {
            _parent_type = RIGIDBODY;
            parent_rigidbody = rigidbody_parent;
        }
        else if (node2D_parent)
        {
            _parent_type = NODE;
            parent_node = node2D_parent;
        }
        else
        {
            unparent();
        }
        break;
    }
    case Node::NOTIFICATION_UNPARENTED: {
        unparent();
    }
    case Node::NOTIFICATION_PHYSICS_PROCESS:
        if (is_attached_to_parent())
        {
            switch (_parent_type)
            {
            case RIGIDBODY: {
                FmodAudioServer::get_singleton()->set_listener_location(listener_index, (RigidBody *)parent_rigidbody);
                return;
            }
            case NODE: {
                FmodAudioServer::get_singleton()->set_listener_location(listener_index, (NodeType *)parent_node);
                return;
            }
            default:
                break;
            }
        }
        FmodAudioServer::get_singleton()->set_listener_location(listener_index, (NodeType *)this);
    default:
        break;
    }
}

template <class Derived, class NodeType, class RigidBody>
void FmodListener<Derived, NodeType, RigidBody>::_bind_methods()
{
    typedef typename Derived::self_type self_type;
    BIND_PROPERTY_WITH_HINT(listener_index, Variant::Type::INT, godot::PROPERTY_HINT_RANGE,
                            "0," + itos(FMOD_MAX_LISTENERS));
    BIND_BOOL_PROPERTY(attached_to_parent)
    BIND_PROPERTY(listener_weight, Variant::Type::FLOAT);
}
template <class Derived, class NodeType, class RigidBody>
const StringName &FmodListener<Derived, NodeType, RigidBody>::get_class_static()
{
    return Derived::get_class_static();
}
} // namespace FmodGodot
