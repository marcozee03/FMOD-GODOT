#pragma once
#include <classes/rigid_body3d.hpp>
#include <classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodListener3D : public Node3D
    {
        GDCLASS(FmodListener3D, Node3D);
        int listener_index;

    private:
        enum ParentType3D
        {
            NODE3D,
            RIGIDBODY3D,
            NONE
        };
        ParentType3D _parent_type = NONE;
        union
        {
            Node3D *parent3D;
            RigidBody3D *rigid_parent3D;
            Node *parent_other;
        };
        /* data */
        bool attached_to_parent;

    protected:
        static void _bind_methods();
        void _notification(int p_what);

    public:
        void _physics_process(double delta) override;
        FmodListener3D(/* args */);
        ~FmodListener3D();
        int get_listener_index();
        void set_listener_index(int index);

        bool is_attached_to_parent();
        void set_attached_to_parent(bool p_attached_to_parent);
    };
}