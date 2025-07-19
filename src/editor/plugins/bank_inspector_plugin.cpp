#ifdef TOOLS_ENABLED
#include "bank_inspector_plugin.h"
#include "fmod_object_details.h"
#include "fmod_bank.h"
void FmodGodot::BankInspectorPlugin::_bind_methods() {}
void FmodGodot::BankInspectorPlugin::_parse_begin(Object *p_object)
{
    Ref<FmodBank> obj = cast_to<FmodBank>(p_object);
    FmodObjectDetails *details = memnew(FmodObjectDetails);
    details->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    details->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
    details->display_fmod_object("bank:/" + obj->get_path().get_basename().get_file());
    add_custom_control(details);
    // add_custom_control(memnew(BankInspector(cast_to<FmodBank>(p_object))));
}

bool FmodGodot::BankInspectorPlugin::_can_handle(Object *p_object) const
{
    return cast_to<FmodBank>(p_object) != nullptr;
}
#endif