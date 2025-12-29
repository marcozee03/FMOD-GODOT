#include "fmod_bank_format_loader.h"
#include "classes/engine.hpp"
#include "classes/global_constants.hpp"
#include "classes/project_settings.hpp"
#include "classes/resource_format_loader.hpp"
#include "fmod_audio_server.h"
#include "fmod_bank.h"
#include "fmod_common.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "variant/utility_functions.hpp"
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
    return "";
}
String FmodGodot::FmodBankFormatLoader::_get_resource_type(const String &path) const
{
    return "FmodBank";
}
bool FmodGodot::FmodBankFormatLoader::_handles_type(const StringName &p_type) const
{
    return p_type.casecmp_to("FmodBank") == 0;
}

// bool FmodGodot::FmodBankFormatLoader::_recognize_path(const String &p_path, const StringName &p_type) const
// {
//     return ResourceFormatLoader::_recognize_path(p_path, p_type) &&
//            p_path.begins_with(ProjectSettings::get_singleton()->get_setting_with_override(BANK_DIRECTORY));
// }
Variant FmodGodot::FmodBankFormatLoader::_load(const String &path, const String &original_path, bool use_sub_threads,
                                               int32_t cache_mode) const
{

    Ref<FmodBank> bank = memnew(FmodBank);
    // bank->set_path(path);
    auto err = FMOD_Studio_System_LoadBankFile(FmodAudioServer::get_singleton()->get_studio(), path.utf8(),
                                               FMOD_STUDIO_LOAD_BANK_NORMAL, &(bank->bank));
#ifndef TOOLS_ENABLED
    switch (cache_mode)
    {
    case CACHE_MODE_IGNORE:
    case CACHE_MODE_IGNORE_DEEP:
        UtilityFunctions::push_warning(
            "Tried loading bank with CACHE_MODE_IGNORE, If the same bank attempts to loads in the future it may fail");
    }
#endif
    switch (err)
    {
    case FMOD_OK:
        return bank;
    case FMOD_ERR_EVENT_ALREADY_LOADED:
        // HACK: workaround to let a bank successfully load on import even if it's already loaded. result is not
        // typically used. unless a future addition use CACHE_MODE_IGNORE

        if (Engine::get_singleton()->is_editor_hint() && cache_mode == CACHE_MODE_IGNORE)
        {
            return bank;
        }
        print_error("CacheMode:", cache_mode, " Fmod Bank has already been loaded reload");
        return ERR_CANT_CREATE;
    default:
        return ERR_BUG;
    }
}
void FmodGodot::FmodBankFormatLoader::_bind_methods()
{
}
