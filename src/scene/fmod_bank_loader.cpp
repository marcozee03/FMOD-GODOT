#include "fmod_bank_loader.h"
#include "variant/typed_array.hpp"
#include <globals.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
using namespace godot;
using namespace FmodGodot;
void FmodBankLoader::_bind_methods()
{
    BIND_PROPERTY_WITH_HINT(banks, Variant::Type::ARRAY, PROPERTY_HINT_TYPE_STRING,
                            String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) +
                                ":FmodBank");
    BIND_BOOL_PROPERTY(preload_sample_data);
}

FmodBankLoader::FmodBankLoader()
{
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
TypedArray<FmodBank> FmodBankLoader::get_banks()
{
    TypedArray<FmodBank> ret;
    for (auto bank : banks)
    {
        ret.push_back(bank);
    }
    return ret;
}
void FmodGodot::FmodBankLoader::set_preload_sample_data(bool p_preload)
{
    preload_sample_data = p_preload;
}
bool FmodGodot::FmodBankLoader::is_preload_sample_data()
{
    return preload_sample_data;
}
void FmodBankLoader::set_banks(TypedArray<FmodBank> banks)
{
    this->banks.clear();
    for (auto var : banks)
    {
        this->banks.push_back(var);
    }
}
