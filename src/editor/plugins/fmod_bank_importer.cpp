#include "classes/global_constants.hpp"
#include "classes/project_settings.hpp"
#include "variant/packed_string_array.hpp"
#include "variant/typed_array.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_bank.h"
#include "fmod_bank_importer.h"
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
FmodGodot::FmodBankImporter::FmodBankImporter()
{
}

FmodGodot::FmodBankImporter::~FmodBankImporter()
{
}

float FmodGodot::FmodBankImporter::_get_priority() const
{
    return 1.0f;
}

String FmodGodot::FmodBankImporter::_get_importer_name() const
{
    return "godot.fmod.bank";
}

String FmodGodot::FmodBankImporter::_get_visible_name() const
{
    return "FMOD Bank";
}
void FmodGodot::FmodBankImporter::_bind_methods()
{
}

PackedStringArray FmodGodot::FmodBankImporter::_get_recognized_extensions() const
{
    return {"bank"};
}

String FmodGodot::FmodBankImporter::_get_save_extension() const
{
    return "bank";
}

String FmodGodot::FmodBankImporter::_get_resource_type() const
{
    return "FmodBank";
}

int32_t FmodGodot::FmodBankImporter::_get_preset_count() const
{
    return 0;
}

int32_t FmodGodot::FmodBankImporter::_get_import_order() const
{
    return 90;
}

TypedArray<Dictionary> FmodGodot::FmodBankImporter::_get_import_options(const String &p_path,
                                                                        int32_t p_preset_index) const
{
    return {};
}

Error FmodGodot::FmodBankImporter::_import(const String &p_source_file, const String &p_save_path,
                                           const Dictionary &p_options, const TypedArray<String> &p_platform_variants,
                                           const TypedArray<String> &p_gen_files) const
{

    // ProjectSettings *ps = ProjectSettings::get_singleton();
    // switch ((int)ps->get_setting(SOURCE_TYPE))
    // {
    // case 0: // SINGLE_PLATFORM_BUILD

    return DirAccess::copy_absolute(p_source_file, String(".").join({p_save_path, _get_save_extension()}));
    // case 1: // MULTIPLE_PLATFORM_BUILD
    //         // Ref<DirAccess> dir = DirAccess::open(ps->get_setting(BANK_DIRECTORY));
    //         // for (auto directory : dir->get_directories())
    //         // {
    //         //         p_platform_variants.find_custom
    //         //     ResourceSaver::get_singleton()->save(bank, String(".").join({p_save_path,
    //         _get_save_extension()}));
    //         // }
    //     break;
    // }
    return godot::ERR_CANT_CREATE;
}
#endif