#pragma once

#include "classes/menu_button.hpp"
#include "classes/popup_menu.hpp"
#include "classes/wrapped.hpp"
namespace FmodGodot
{
using namespace godot;
class EventBrowserMenuButton : MenuButton
{
    GDCLASS(EventBrowserMenuButton, MenuButton)
  private:
    enum OPTION_IDS : int
    {
        BUILD_BANKS,
        RESTART_SERVER,
    };

  public:
    EventBrowserMenuButton()
    {
        get_popup()->add_item("Build Banks", BUILD_BANKS);
        get_popup()->set_item_tooltip(0, "Fmod Studio must be open and Project build output must be configured");
        get_popup()->add_item("Restart Server", RESTART_SERVER);
    }
};
} // namespace FmodGodot
