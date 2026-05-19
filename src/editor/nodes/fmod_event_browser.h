#pragma once
#include "fmod_project_explorer.h"
#ifdef TOOLS_ENABLED
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include <classes/button.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEventBrowser : public VBoxContainer
{
    GDCLASS(FmodEventBrowser, VBoxContainer)
  private:
    HBoxContainer *top_buttons;
    FmodProjectExplorer *explorer;
    void refresh();

  protected:
    static void _bind_methods();

  public:
    FmodEventBrowser(/* args */);
    ~FmodEventBrowser() = default;
};
} // namespace FmodGodot
#endif
