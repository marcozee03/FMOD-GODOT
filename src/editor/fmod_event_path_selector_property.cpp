#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include "fmod_event_path_selector_property.h"
#include <godot_cpp/core/class_db.hpp>
// #include <godot_cpp/classes/event_s
using namespace godot;
namespace FMODGodot
{
    EventPathSelectorProperty::EventPathSelectorProperty()
    {
        eventSelector = memnew(FmodEventPathSelector);
        add_child(eventSelector);
        eventSelector->get_line_edit()->connect("text_changed", Callable(this, "on_text_changed"));
    }
    EventPathSelectorProperty::~EventPathSelectorProperty()
    {
    }
    void EventPathSelectorProperty::on_text_changed(String newText)
    {
        if (updating)
        {
            return;
        }
        if (currentValue == newText)
        {
            return;
        }
        currentValue = newText;
        emit_changed(get_edited_property(), currentValue);
    }

    void EventPathSelectorProperty::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("on_text_changed", "new_text"), &EventPathSelectorProperty::on_text_changed);
    }

    void EventPathSelectorProperty::_update_property()
    {
        auto newValue = (String)get_edited_object()->get(get_edited_property());
        if (newValue == currentValue)
        {
            return;
        }
        // Update the control with the new value.
        updating = true;
        currentValue = newValue;
        eventSelector->get_line_edit()->set_text(currentValue);
        updating = false;
    }
}
#endif