#include "fmod_selector_inspector_plugin.h"
#include "fmod_event_guid_selector_property.h"
#include "fmod_event_path_selector_property.h"
#include "fmod_object_tree.h"
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/core/memory.hpp>
using namespace godot;
namespace FmodGodot
{
bool FmodSelectorInspector::_parse_property(Object *p_object, Variant::Type p_type, const String &p_name,
                                            PropertyHint p_hint_type, const String &p_hint_string,
                                            BitField<PropertyUsageFlags> p_usage_flags, bool p_wide)
{
    String substr = p_hint_string.substr(4);
    if (p_hint_string.to_lower().begins_with("fmod"))
    {
        String substr = p_hint_string.substr(4);
        FmodObjectTree::DisplayFlags flags;
        if (substr.nocasecmp_to("event") == 0)
        {
            flags = FmodObjectTree::FMOD_DISPLAY_EVENTS;
        }
        else if (substr.nocasecmp_to("vca") == 0)
        {
            flags = FmodObjectTree::FMOD_DISPLAY_VCAS;
        }
        else if (substr.nocasecmp_to("param") == 0)
        {
            flags = FmodObjectTree::FMOD_DISPLAY_GLOBAL_PARAMETERS;
        }
        else
        {
            return EditorInspectorPlugin::_parse_property(p_object, p_type, p_name, p_hint_type, p_hint_string,
                                                          p_usage_flags, p_wide);
        }
        switch (p_type)
        {
        case Variant::Type::STRING:
            add_property_editor(p_name, memnew(FmodPathSelectorProperty(flags)), false, p_name);
            return true;
        case Variant::Type::VECTOR4I:
            add_property_editor(p_name, memnew(FmodGUIDSelectorProperty(flags)), false, p_name);
            return true;
        default:
            break;
        }
    }
    return EditorInspectorPlugin::_parse_property(p_object, p_type, p_name, p_hint_type, p_hint_string, p_usage_flags,
                                                  p_wide);
}
bool FmodSelectorInspector::_can_handle(Object *p_object) const
{
    return true;
}
void FmodSelectorInspector::_bind_methods()
{
}
} // namespace FmodGodot
