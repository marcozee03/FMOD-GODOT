#ifdef TOOLS_ENABLED
#include "fmod_event_inspector_plugin.h"
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include "fmod_event_path_selector_property.h"
#include "fmod_event_guid_selector_property.h"
#include <godot_cpp/core/memory.hpp>
using namespace godot;
namespace FmodGodot
{
    bool EventInspector::_parse_property(Object *p_object, Variant::Type p_type, const String &p_name, PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, bool p_wide)
    {
        if (p_hint_string.nocasecmp_to("FMODEvent") == 0)
        {

            switch (p_type)
            {
            case Variant::Type::STRING:
                add_property_editor(p_name, memnew(EventPathSelectorProperty()), false, p_name);
                return true;
            case Variant::Type::VECTOR4I:
                add_property_editor(p_name, memnew(EventGUIDSelectorProperty()), false, p_name);

                return true;
            default:
                break;
            }
        }
        return EditorInspectorPlugin::_parse_property(p_object, p_type, p_name, p_hint_type, p_hint_string, p_usage_flags, p_wide);
    }
    bool EventInspector::_can_handle(Object *p_object) const
    {
        return true;
    }
    void EventInspector::_bind_methods()
    {
    }
}

#endif