// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include "bank_inspector_plugin.h"
#include "bank_inspector.h"
#include "fmod_bank.h"
void FmodGodot::BankInspectorPlugin::_bind_methods() {}
void FmodGodot::BankInspectorPlugin::_parse_begin(Object *p_object)
{
    add_custom_control(memnew(BankInspector(cast_to<FmodBank>(p_object))));
}

bool FmodGodot::BankInspectorPlugin::_can_handle(Object *p_object) const
{
    return cast_to<FmodBank>(p_object) != nullptr;
}
#endif