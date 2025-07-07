#pragma once
#include <classes/tree.hpp>
using namespace godot;

namespace FmodGodot
{
    class EventTree : public Tree
    {
        GDCLASS(EventTree, Tree);

    private:
        TreeItem *root;

    protected:
        static void _bind_methods();

    public:
        EventTree();
        ~EventTree();
        bool LoadEvents();
        void _ready() override;
    };
}