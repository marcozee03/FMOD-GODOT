#include "fmod_event_browser.h"
#include <classes/button.hpp>
using namespace godot;
namespace FmodGodot
{
    FmodGodot::FmodEventBrowser::FmodEventBrowser()
    {
        top_buttons = memnew(HBoxContainer);
        Button *button = memnew(Button());
        button->set_text("Refresh");
        top_buttons->add_child(button);
        add_child(top_buttons, false, INTERNAL_MODE_FRONT);
        split = memnew(HBoxContainer());
        split->set_v_size_flags(SIZE_EXPAND_FILL);
        add_child(split);
        tree = memnew(EventTree());
        details = memnew(FmodObjectDetails());
        split->add_child(tree);
        tree->set_display_flags(EventTree::DisplayFlags::BANKS | EventTree::EVENTS | EventTree::VCAS | EventTree::GLOBAL_PARAMETERS);
        tree->set_h_size_flags(SIZE_EXPAND_FILL);
        split->add_child(details);
        tree->connect("fmod_object_selected", Callable(details, "display_fmod_object"), CONNECT_PERSIST);
    }
    void FmodGodot::FmodEventBrowser::_bind_methods()
    {
    }
}