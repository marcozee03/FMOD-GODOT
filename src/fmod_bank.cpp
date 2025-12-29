#include "fmod_bank.h"
#include "fmod_common.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
namespace FmodGodot
{
FmodBank::FmodBank()
{
}

FmodBank::~FmodBank()
{
    unload();
}
bool FmodBank::is_bank_valid() const
{
    return FMOD_Studio_Bank_IsValid(bank);
}
Vector4i FmodBank::get_id() const
{
    FMOD_GUID guid;
    FMOD_Studio_Bank_GetID(bank, &guid);
    return cast_to_vector4i(guid);
}

int FmodBank::unload()
{
    return FMOD_Studio_Bank_Unload(bank);
}
int FmodBank::load_sample_data()
{
    return FMOD_Studio_Bank_LoadSampleData(bank);
}
int FmodBank::unload_sample_data()
{
    return FMOD_Studio_Bank_Unload(bank);
}
int FmodBank::get_loading_state() const
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetLoadingState(bank, &state);
    return state;
}
int FmodBank::get_sample_loading_state() const
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetSampleLoadingState(bank, &state);
    return state;
}
size_t FmodBank::get_bank() const
{
    return (size_t)bank;
}
void FmodBank::set_bank(size_t p_bank_ptr)
{
    bank = (FMOD_STUDIO_BANK *)p_bank_ptr;
}
void FmodBank::_bind_methods()
{
    BIND_METHOD(get_sample_loading_state);
    BIND_METHOD(get_loading_state);
    BIND_METHOD(unload_sample_data);
    BIND_METHOD(unload);
    BIND_METHOD(is_bank_valid);
    BIND_METHOD(get_id);
    ClassDB ::bind_method(D_METHOD("get_bank"), &self_type::get_bank);
    ClassDB ::bind_method(D_METHOD("set_bank", "bank"), &self_type::set_bank);
    ClassDB ::add_property(get_class_static(),
                           PropertyInfo(Variant ::INT, "bank", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE),
                           "set_"
                           "bank",
                           "get_"
                           "bank");
}
} // namespace FmodGodot
