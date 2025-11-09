#include "fmod_bank_loader.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <globals.h>
using namespace godot;
using namespace FmodGodot;
void FmodBankLoader::_bind_methods()
{
    BIND_PROPERTY_WITH_HINT(banks, Variant::Type::ARRAY, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":FmodBank");
    BIND_BOOL_PROPERTY(preload_sample_data);
}

FmodBankLoader::FmodBankLoader()
{
    banks = TypedArray<Ref<FmodBank>>();
}
FmodBankLoader::~FmodBankLoader()
{
}
void FmodBankLoader::_enter_tree()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    if (preload_sample_data)
    {
        for (Ref<FmodBank> bank : banks)
        {
            FMOD_Studio_Bank_LoadSampleData(bank->bank);
        }
    }
}
TypedArray<Ref<FmodBank>> FmodBankLoader::get_banks()
{
    return banks;
}
void FmodGodot::FmodBankLoader::set_preload_sample_data(bool p_preload)
{
    preload_sample_data = p_preload;
}
bool FmodGodot::FmodBankLoader::is_preload_sample_data()
{
    return preload_sample_data;
}
void FmodBankLoader::set_banks(TypedArray<Ref<FmodBank>> banks)
{
    this->banks = banks;
}
