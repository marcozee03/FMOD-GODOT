#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_event_selector.h"
#include "fmod_event_path_selector_property.h"
#include <godot_cpp/core/class_db.hpp>
// #include <godot_cpp/classes/event_s
using namespace godot;
namespace FmodGodot
{
    EventPathSelectorProperty::EventPathSelectorProperty()
    {
        eventSelector = memnew(FmodEventSelector);
        add_child(eventSelector);
        eventSelector->get_line_edit()->connect("text_submitted", callable_mp(this, &EventPathSelectorProperty::on_text_changed));
        eventSelector->get_line_edit()->connect("editing_toggled", callable_mp(this, &EventPathSelectorProperty::on_editing_toggled));
    }
    EventPathSelectorProperty::~EventPathSelectorProperty()
    {
    }
	void EventPathSelectorProperty::on_editing_toggled(bool toggled_on){
		if(!toggled_on){
			on_text_changed(eventSelector->get_line_edit()->get_text());
		}
	}
    void EventPathSelectorProperty::on_text_changed(String newText)
    {
        if (currentValue == newText)
        {
            return;
        }
        currentValue = newText;
        emit_changed(get_edited_property(), currentValue);
    }

    void EventPathSelectorProperty::_bind_methods()
    {
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
