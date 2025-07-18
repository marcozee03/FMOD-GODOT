#pragma once
#include <classes/tree.hpp>
using namespace godot;

namespace FmodGodot
{
    class EventTree : public Tree
    {
        GDCLASS(EventTree, Tree);

    public:
        enum DisplayFlags
        {
            BANKS = 1,
            EVENTS = 2,
            VCAS = 4,
            GLOBAL_PARAMETERS = 8
        };

    private:
        DisplayFlags display_flags;
        void on_item_activated();

    protected:
        static void
        _bind_methods();

    public:
        EventTree();
        ~EventTree();
        void set_display_flags(int p_flags);
        int get_display_flags() const;
        void LoadEvents();
    };

}
VARIANT_BITFIELD_CAST(FmodGodot::EventTree::DisplayFlags)