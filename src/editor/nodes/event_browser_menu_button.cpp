#include "event_browser_menu_button.h"
#include "classes/popup_menu.hpp"
#include "fmod_editor_interface.h"
void FmodGodot::EventBrowserMenuButton::_id_pressed(int p_id)
{
    if (p_id == BUILD_BANKS)
    {
        FmodEditorInterface::get_singleton()->build_banks();
    }
    else if (p_id == RESTART_SERVER)
    {
        FmodEditorInterface::get_singleton()->restart_server();
    }
}
void FmodGodot::EventBrowserMenuButton::_bind_methods()
{
}
void FmodGodot::EventBrowserMenuButton::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_ENTER_TREE:
    case NOTIFICATION_THEME_CHANGED:
        get_popup()->set_item_icon(0, get_theme_icon("FmodBank", "EditorIcons"));
        set_button_icon(get_theme_icon("GuiTabMenuHl", "EditorIcons"));
        get_popup()->set_item_icon(1, get_theme_icon("Reload", "EditorIcons"));
    }
}
FmodGodot::EventBrowserMenuButton::EventBrowserMenuButton()
{
    get_popup()->add_item("Build Banks", BUILD_BANKS);
    get_popup()->set_item_tooltip(0, "Fmod Studio must be open and Project build output must be configured");
    get_popup()->add_item("Restart Server", RESTART_SERVER);
    get_popup()->set_item_tooltip(1, "Restarts the FMOD Audio Server");
    get_popup()->connect("id_pressed", callable_mp(this, &EventBrowserMenuButton::_id_pressed));
    set_theme_type_variation("FlatButton");
}
