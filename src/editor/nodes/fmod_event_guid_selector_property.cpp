#include "fmod_event_guid_selector_property.h"
#include "fmod_event_selector.h"
#include "variant/variant.hpp"
#include <classes/project_settings.hpp>
#include <fmod_defs.h>
#include <fmod_errors.h>
#include <fmod_studio.h>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;
namespace FmodGodot
{
EventGUIDSelectorProperty::EventGUIDSelectorProperty()
{
    eventSelector = memnew(FmodEventSelector);
    add_child(eventSelector);
    eventSelector->connect("fmod_guid_and_path_selected",
                           callable_mp(this, &EventGUIDSelectorProperty::_fmod_guid_and_path_changed));
}
EventGUIDSelectorProperty::~EventGUIDSelectorProperty()
{
}

void EventGUIDSelectorProperty::_fmod_guid_and_path_changed(const Vector4i &p_guid, const String &p_path)
{
    if (currentValue != p_guid)
    {
        currentValue = p_guid;
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

void EventGUIDSelectorProperty::_bind_methods()
{
}

void EventGUIDSelectorProperty::_update_property()
{
    auto newValue = static_cast<Vector4i>(get_edited_object()->get(get_edited_property()));
    if (newValue == currentValue)
    {
        return;
    }
    // Update the control with the new value.
    updating = true;
    currentValue = newValue;
    eventSelector->set_guid(currentValue);
    updating = false;
}
} // namespace FmodGodot
