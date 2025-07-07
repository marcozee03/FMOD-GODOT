#pragma once
#include <godot_cpp/classes/node3d.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodListener : public Node3D
    {
        GDCLASS(FmodListener, Node3D);
        int listener_index;

    private:
        /* data */
    protected:
        static void _bind_methods();

    public:
        void _physics_process(double delta) override;
        FmodListener(/* args */);
        ~FmodListener();
        int get_listener_index();
        void set_listener_index(int index);
    };
}