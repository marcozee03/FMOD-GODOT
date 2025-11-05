#pragma once
#include "fmod_event_previewer.h"
#ifdef TOOLS_ENABLED
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include "fmod_event_tree.h"
#include "fmod_object_details.h"
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
    HBoxContainer *split;
    FmodEventPreviewer *previewer;
    EventTree *tree;
    FmodObjectDetails *details;
    void refresh();

  protected:
    static void _bind_methods();

  public:
    FmodEventBrowser(/* args */);
    ~FmodEventBrowser() = default;
};
} // namespace FmodGodot
#endif
