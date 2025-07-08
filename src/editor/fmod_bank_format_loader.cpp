#include "fmod_bank_format_loader.h"

FmodGodot::FmodBankFormatLoader::FmodBankFormatLoader()
{
}

FmodGodot::FmodBankFormatLoader::~FmodBankFormatLoader()
{
}
bool FmodGodot::FmodBankFormatLoader::_exists(const String &path) const
{
    return path.ends_with(".bank");
}
PackedStringArray FmodGodot::FmodBankFormatLoader::_get_recognized_extensions() const
{
    return {"bank"};
}
String FmodGodot::FmodBankFormatLoader::_get_resource_script_class(const String &path) const
{
    return "FmodBank";
}
String FmodGodot::FmodBankFormatLoader::_get_resource_type(const String &path) const
{
    return "FmodBank";
}
bool FmodGodot::FmodBankFormatLoader::_handles_type(const StringName &p_type) const
{
    return p_type.casecmp_to("FmodBank") == 0;
}
Variant FmodGodot::FmodBankFormatLoader::_load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode) const
{
    Ref<FmodBank> bank = memnew(FmodBank);
    bank->set_path(path);
    return bank;
}
void FmodGodot::FmodBankFormatLoader::_bind_methods()
{
}
