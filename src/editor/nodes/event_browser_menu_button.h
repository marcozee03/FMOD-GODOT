#pragma once

#include "classes/menu_button.hpp"
#include "classes/wrapped.hpp"
namespace FmodGodot
{
using namespace godot;
class EventBrowserMenuButton : public MenuButton
{
    GDCLASS(EventBrowserMenuButton, MenuButton)
  private:
    enum OPTION_IDS : int
    {
        BUILD_BANKS,
        RESTART_SERVER,
    };

    void _id_pressed(int p_id);

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    EventBrowserMenuButton();
};
} // namespace FmodGodot
