#include "fmod_event_path_selector_property.h"
#include "fmod_event_selector.h"
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;
namespace FmodGodot
{
EventPathSelectorProperty::EventPathSelectorProperty()
{
    eventSelector = memnew(FmodEventSelector);
    add_child(eventSelector);
    eventSelector->connect("fmod_guid_and_path_selected",
                           callable_mp(this, &EventPathSelectorProperty::_fmod_guid_and_path_changed));
}
EventPathSelectorProperty::~EventPathSelectorProperty()
{
}
void EventPathSelectorProperty::_fmod_guid_and_path_changed(const Vector4i &p_guid, const String &p_path)
{
    if (currentValue != p_path)
    {
        currentValue = p_path;
        emit_changed(get_edited_property(), currentValue);
    }

    if (p_guid == Vector4i(0, 0, 0, 0))
    {
        _err_print_error(__FUNCTION__, __FILE__, __LINE__,
                         vformat("Could not find FMOD_GUID %v for '%s' setting will not persist only guid is stored",
                                 p_guid, p_path),
                         true, true);
    }
}
void EventPathSelectorProperty::_bind_methods()
{
}

void EventPathSelectorProperty::_update_property()
{
    auto newValue = static_cast<String>(get_edited_object()->get(get_edited_property()));
    if (newValue == currentValue)
    {
        return;
    }
    // Update the control with the new value.
    updating = true;
    currentValue = newValue;
    eventSelector->set_path(currentValue);
    updating = false;
}
} // namespace FmodGodot
