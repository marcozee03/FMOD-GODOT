#ifdef TOOLS_ENABLED
#include "banks_explorer_property.h"

using namespace godot;
namespace FMODGodot
{
    void BanksExplorerProperty::on_paths_changed()
    {
        if (updating)
        {
            return;
        }
        currentValue = banksExplorer->path_list();
        emit_changed(get_edited_property(), currentValue);
    }

    void BanksExplorerProperty::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("on_paths_changed"), &BanksExplorerProperty::on_paths_changed);
    }

    BanksExplorerProperty::BanksExplorerProperty()
    {
        banksExplorer = memnew(BanksExplorer);
        add_child(banksExplorer);
        banksExplorer->connect("paths_changed", Callable(this, "on_paths_changed"));
        updating = false;
    }

    BanksExplorerProperty::~BanksExplorerProperty()
    {
    }
    void BanksExplorerProperty::_update_property()
    {
        PackedStringArray newValue = get_edited_object()->get(get_edited_property());
        if (newValue == currentValue)
        {
            return;
        }

        // Update the control with the new value.
        updating = true;
        currentValue = newValue;
        banksExplorer->clear();
        banksExplorer->add_banks(get_edited_object()->get(get_edited_property()));
        updating = false;
    }
}
#endif