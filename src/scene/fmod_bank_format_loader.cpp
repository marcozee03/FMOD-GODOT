#include "fmod_bank_format_loader.h"
#include "classes/global_constants.hpp"
#include "classes/resource_format_loader.hpp"
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
Variant FmodGodot::FmodBankFormatLoader::_load(const String &path, const String &original_path, bool use_sub_threads,
                                               int32_t cache_mode) const
{
    switch (cache_mode)
    {
    case CACHE_MODE_REUSE: {
    case CACHE_MODE_REPLACE: {
        Ref<FmodBank> bank = memnew(FmodBank);
        bank->set_path(path);
        FMOD_Studio_System_LoadBankFile(FmodAudioServer::get_singleton()->get_studio(), path.utf8(),
                                        FMOD_STUDIO_LOAD_BANK_NORMAL, &(bank->bank));
        return bank;
    }
    default:
        UtilityFunctions::push_warning(
            "Using a cache mode other then CACHE_MODE_REUSE or CACHE_MODE_REPLACE is not supported. may lead to "
            "unwanted behavior when one bank goes out of scope.");
        return ERR_INVALID_PARAMETER;
    }
        return ERR_BUG;
    }
}
void FmodGodot::FmodBankFormatLoader::_bind_methods()
{
}
