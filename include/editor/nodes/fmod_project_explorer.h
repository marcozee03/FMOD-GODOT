#pragma once
#ifdef TOOLS_ENABLED
#include "classes/h_box_container.hpp"
#include "fmod_event_previewer.h"
#include "fmod_event_tree.h"
#include "fmod_object_details.h"
#include "variant/packed_string_array.hpp"
#include <classes/button.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodProjectExplorer : public HBoxContainer
{
    GDCLASS(FmodProjectExplorer, HBoxContainer)
  private:
    FmodEventPreviewer *previewer;
    EventTree *tree;
    FmodObjectDetails *details;
    void emit_object_selected(const String &name);
    void emit_object_activated(const String &name);

  protected:
    static void _bind_methods();

  public:
    FmodProjectExplorer(/* args */);
    ~FmodProjectExplorer() = default;
    void set_display_flags(EventTree::DisplayFlags flags);
    void refresh();
};
} // namespace FmodGodot
#endif
#
