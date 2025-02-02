#ifdef TOOLS_ENABLED
#include "bank_loader_inspector_plugin.h"
#include "bankloader.h"
#include "banks_explorer_property.h"
using namespace godot;
namespace FMODGodot
{
    bool BankLoaderInspector::_parse_property(Object *p_object, Variant::Type p_type, const String &p_name, PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, bool p_wide)
    {
        if ((p_hint_string == "FMODBanks") && p_type == Variant::Type::PACKED_STRING_ARRAY)
        {
            add_property_editor(p_name, memnew(BanksExplorerProperty), false, p_name);
            return true;
        }
        return EditorInspectorPlugin::_parse_property(p_object, p_type, p_name, p_hint_type, p_hint_string, p_usage_flags, p_wide);
    }
    bool BankLoaderInspector::_can_handle(Object *p_object) const
    {
        return dynamic_cast<BankLoader *>(p_object);
    }
    void BankLoaderInspector::_bind_methods()
    {
    }
}
#endif