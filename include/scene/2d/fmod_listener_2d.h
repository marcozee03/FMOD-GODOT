#pragma once
#include <classes/rigid_body2d.hpp>
#include <classes/node.hpp>
#include <godot_cpp/classes/node2d.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodListener2D : public Node2D
    {
        GDCLASS(FmodListener2D, Node2D);
        int listener_index;

    private:
        enum ParentType2D
        {
            NODE2D,
            RIGIDBODY2D,
            NONE
        };
        ParentType2D _parent_type = NONE;
        union
        {
            Node2D *parent2D;
            RigidBody2D *rigid_parent2D;
            Node *parent_other;
        };
        /* data */
        bool attached_to_parent;

    protected:
        static void _bind_methods();
        void _notification(int p_what);

    public:
        void _physics_process(double delta) override;
        FmodListener2D() = default;
        ~FmodListener2D() = default;
        int get_listener_index();
        void set_listener_index(int index);

        bool is_attached_to_parent();
        void set_attached_to_parent(bool p_attached_to_parent);
    };
}