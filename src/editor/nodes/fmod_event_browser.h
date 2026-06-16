#pragma once
#include "classes/editor_dock.hpp"
#include "fmod_project_explorer.h"
#ifdef TOOLS_ENABLED
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include <classes/button.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEventBrowser : public EditorDock
{
    GDCLASS(FmodEventBrowser, EditorDock)
  private:
    VBoxContainer *vbox;
    HBoxContainer *top_buttons;
    HBoxContainer *hbox_right;
    FmodProjectExplorer *explorer;
    void refresh();

  protected:
    static void _bind_methods();

  public:
    FmodEventBrowser(/* args */);
    ~FmodEventBrowser() = default;
    void _update_layout(int p_layout) override
    {
        switch (p_layout)
        {

        case EditorDock::DOCK_LAYOUT_VERTICAL: {
            explorer->set_vertical(true);
        }
        break;
        case EditorDock::DOCK_LAYOUT_HORIZONTAL:
        case EditorDock::DOCK_LAYOUT_FLOATING: {
            explorer->set_vertical(false);
        }
        break;
        default:
            break;
        }
    }
};
} // namespace FmodGodot
#endif
