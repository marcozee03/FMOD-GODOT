#pragma once
#ifdef TOOLS_ENABLED
#include <classes/editor_property.hpp>
#include "fmod_banks_explorer.h"
using namespace godot;
namespace FMODGodot
{
    class BanksExplorerProperty : public EditorProperty
    {
        GDCLASS(BanksExplorerProperty, EditorProperty);

    private:
        bool updating;
        BanksExplorer *banksExplorer;
        PackedStringArray currentValue;
        void on_paths_changed();

    protected:
        static void _bind_methods();

    public:
        BanksExplorerProperty();
        ~BanksExplorerProperty();
        virtual void _update_property() override;
    };
}
#endif