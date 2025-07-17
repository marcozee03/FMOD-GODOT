// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/dir_access.hpp>
#include "fmod_bank_importer.h"
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include "fmod_bank.h"
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
    return "fmod bank";
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

TypedArray<Dictionary> FmodGodot::FmodBankImporter::_get_import_options(const String &p_path, int32_t p_preset_index) const
{
    return {};
}

Error FmodGodot::FmodBankImporter::_import(const String &p_source_file, const String &p_save_path, const Dictionary &p_options, const TypedArray<String> &p_platform_variants, const TypedArray<String> &p_gen_files) const
{
    Ref<FmodBank> bank = memnew(FmodBank);
    bank->set_path(p_source_file);
    return ResourceSaver::get_singleton()->save(bank, String(".").join({p_save_path, _get_save_extension()}));
}
#endif