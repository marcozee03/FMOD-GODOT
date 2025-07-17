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
        enum DisplayFlags
        {
            BANKS = 1,
            EVENTS = 2,
            VCAS = 4,
            GLOBAL_PARAMETERS = 8
        };
        DisplayFlags display_flags;

    protected:
        static void
        _bind_methods();

    public:
        EventTree();
        ~EventTree();
        void set_display_flags(int p_flags);
        int get_display_flags() const;
        bool LoadEvents();
        void _ready() override;
    };
}