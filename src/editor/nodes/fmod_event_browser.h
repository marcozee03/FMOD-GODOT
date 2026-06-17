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
    Button *refresh_button;
    FmodProjectExplorer *explorer;
    void refresh();
    void _update_theme();

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    FmodEventBrowser(/* args */);
    ~FmodEventBrowser() = default;
    void _update_layout(int p_layout) override;
};
} // namespace FmodGodot
#endif
