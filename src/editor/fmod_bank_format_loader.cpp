#include "fmod_bank_format_loader.h"
#include "fmod_audio_server.h"
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
    if (cache_mode != CACHE_MODE_REUSE)
    {
        UtilityFunctions::push_warning("Using a cache mode other then CACHE_MODE_REUSE is not supported. may lead to unwanted behavior when one bank goes out of scope");
    }
    Ref<FmodBank> bank = memnew(FmodBank);
    bank->set_path(path);
    FMOD_Studio_System_LoadBankFile(FmodAudioServer::get_singleton()->get_studio(), path.utf8(), FMOD_STUDIO_LOAD_BANK_NORMAL, &(bank->bank));
    return bank;
}
void FmodGodot::FmodBankFormatLoader::_bind_methods()
{
}
