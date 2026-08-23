#include "fmod_bank.h"
#include "bank.h"
#include "core/print_string.hpp"
#include "fmod_audio_server.h"
#include "fmod_common.h"
#include "fmod_enums.h"
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
bool FmodBank::is_valid() const
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
    if (get_loading_state() != FMOD_STUDIO_LOADING_STATE_UNLOADED ||
        get_loading_state() != FMOD_STUDIO_LOADING_STATE_UNLOADING)
    {
        print_verbose("Unloading Bank", get_path());
        return FMOD_Studio_Bank_Unload(bank);
    }
    return FMOD_ERR_STUDIO_NOT_LOADED;
}
int FmodBank::reload()
{
    if (get_loading_state() == FMOD_STUDIO_LOADING_STATE_LOADED ||
        get_loading_state() == FMOD_STUDIO_LOADING_STATE_LOADING)
    {
        unload();
    }
    return FMOD_Studio_System_LoadBankFile(FmodAudioServer::get_singleton()->get_studio(), get_path().utf8().ptr(),
                                           FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
}

int FmodBank::load_sample_data()
{
    return FMOD_Studio_Bank_LoadSampleData(bank);
}
int FmodBank::unload_sample_data()
{
    return FMOD_Studio_Bank_UnloadSampleData(bank);
}
FMOD_STUDIO_LOADING_STATE FmodBank::get_loading_state() const
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetLoadingState(bank, &state);
    return state;
}
FMOD_STUDIO_LOADING_STATE FmodBank::get_sample_loading_state() const
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetSampleLoadingState(bank, &state);
    return state;
}
int FmodBank::get_string_count() const
{
    return Studio::StudioBank::get_string_count(std::bit_cast<size_t>(bank));
}
String FmodBank::get_string_info(int p_index, const Vector4i &p_id) const
{
    return Studio::StudioBank::get_string_info(std::bit_cast<size_t>(bank), p_index, p_id);
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
    BIND_METHOD(is_valid);
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
extern "C"
{
#define BANK ((FmodGodot::FmodBank *)internal::get_object_instance_binding(handle))

    GDE_EXPORT bool fmod_bank_is_valid(void *handle)
    {
        return BANK->is_valid();
    }

    GDE_EXPORT FMOD_STUDIO_BANK *fmod_bank_get_bank(void *handle)
    {
        return reinterpret_cast<FMOD_STUDIO_BANK *>(BANK->get_bank());
    }
    GDE_EXPORT void fmod_bank_set_bank(void *handle, FMOD_STUDIO_BANK *bank)
    {
        BANK->set_bank(reinterpret_cast<size_t>(bank));
    }
    GDE_EXPORT FMOD_GUID fmod_bank_get_id(void *handle)
    {
        return FmodGodot::cast_to_fmod_guid(BANK->get_id());
    }
    // string getPath(char *path, int size, int *retrieved) const;
    // Loading control
    GDE_EXPORT int fmod_bank_unload(void *handle)
    {
        return BANK->unload();
    }
    GDE_EXPORT int fmod_bank_load_sample_data(void *handle)
    {
        return BANK->load_sample_data();
    }
    GDE_EXPORT int fmod_bank_unload_sample_data(void *handle)
    {
        return BANK->unload_sample_data();
    }

    GDE_EXPORT int fmod_bank_get_loading_state(void *handle)
    {
        return BANK->get_loading_state();
    }
    GDE_EXPORT int fmod_bank_get_sample_loading_state(void *handle)
    {
        return BANK->get_sample_loading_state();
    }
#undef BANK
}
} // namespace FmodGodot
