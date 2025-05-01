#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/core/memory.hpp>
using namespace godot;
namespace FMODGodot{
    class BankLoaderInspector : public EditorInspectorPlugin
    {
        GDCLASS(BankLoaderInspector,EditorInspectorPlugin)
    public:
        virtual bool _parse_property(Object *p_object, Variant::Type p_type, const String &p_name, PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, bool p_wide) override;
        virtual bool _can_handle(Object *p_object) const override;
        static void _bind_methods();
    };
}
#endif