// #pragma once
#ifndef TREE_GUARD
#define TREE_GUARD
#include <classes/tree.hpp>
using namespace godot;

namespace FMODGodot
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
#endif