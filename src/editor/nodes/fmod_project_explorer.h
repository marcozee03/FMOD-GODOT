#pragma once
#include "classes/box_container.hpp"
#include "fmod_event_previewer.h"
#include "fmod_event_tree.h"
#include "fmod_object_details.h"
#include "variant/packed_string_array.hpp"
#include <classes/button.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodProjectExplorer : public BoxContainer
{
    GDCLASS(FmodProjectExplorer, BoxContainer)
  private:
    FmodEventPreviewer *previewer;
    EventTree *tree;
    FmodObjectDetails *details;
    void emit_object_selected(const String &p_name);
    void emit_object_activated(const String &p_name);
    void _update_theme();

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    FmodProjectExplorer(/* args */);
    ~FmodProjectExplorer() = default;
    void set_display_flags(EventTree::DisplayFlags p_flags);
    void refresh();
};
} // namespace FmodGodot
