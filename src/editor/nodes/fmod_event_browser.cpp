#include "classes/control.hpp"
#include "classes/editor_dock.hpp"
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include "core/print_string.hpp"
#include "fmod_project_explorer.h"
#include "variant/callable_method_pointer.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_editor_interface.h"
#include "fmod_event_browser.h"
#include "live_update_indicator.h"
#include <classes/button.hpp>
#include <classes/label.hpp>
#include <classes/project_settings.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
FmodGodot::FmodEventBrowser::FmodEventBrowser()
{
    set_name("FMOD Event Browser");
    set_title("FMOD Event Browser");
    // set_dock_shortcut()
    set_default_slot(EditorDock::DOCK_SLOT_LEFT_BR);
    set_available_layouts(EditorDock::DOCK_LAYOUT_ALL);
    set_closable(true);
    set_global(true);
    vbox = memnew(VBoxContainer);
    add_child(vbox);
    HBoxContainer *hbox = memnew(HBoxContainer);
    hbox_right = memnew(HBoxContainer);

    vbox->add_child(hbox);
    top_buttons = memnew(HBoxContainer);
    top_buttons->set_h_size_flags(SIZE_EXPAND_FILL);
    refresh_button = memnew(Button());
    refresh_button->connect("pressed", callable_mp(this, &FmodEventBrowser::refresh));
    refresh_button->set_tooltip_text("Refresh FMOD explorer.");
    refresh_button->set_theme_type_variation("FlatButton");

    hbox->add_child(top_buttons, false, INTERNAL_MODE_FRONT);
    hbox->add_child(hbox_right);

    Button *build = memnew(Button());
    build->set_text("Build Banks");
    build->set_tooltip_text("The FMOD Studio project build path must be set inside this godot project");
    top_buttons->add_child(build);
    build->connect("pressed", callable_mp(FmodEditorInterface::get_singleton(), &FmodEditorInterface::build_banks));
    // build->set_disabled(true);
    Button *restart_server = memnew(Button());
    restart_server->set_text("Restart Server");
    restart_server->set_tooltip_text("restarts the fmod audio server");
    top_buttons->add_child(restart_server);
    restart_server->connect("pressed",
                            callable_mp(FmodEditorInterface::get_singleton(), &FmodEditorInterface::restart_server));
    hbox_right->add_child(memnew(LiveUpdateIndicator));
    hbox_right->add_child(refresh_button);

    explorer = memnew(FmodProjectExplorer);
    explorer->set_v_size_flags(SIZE_EXPAND_FILL);
    vbox->add_child(explorer);
}
void FmodEventBrowser::_update_layout(int p_layout)
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

void FmodGodot::FmodEventBrowser::refresh()
{
    FmodEditorInterface::get_singleton()->refresh();
    explorer->refresh();
}
void FmodEventBrowser::_update_theme()
{
    refresh_button->add_theme_icon_override("icon", get_theme_icon("Reload", "EditorIcons"));
}

void FmodGodot::FmodEventBrowser::_bind_methods()
{
}
void FmodEventBrowser::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_ENTER_TREE:
    case NOTIFICATION_THEME_CHANGED:
        _update_theme();
        break;
    }
}

} // namespace FmodGodot
#endif
