#include "fmod_bank.h"
#include "fmod_audio_server.h"
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
void FmodBank::_bind_methods()
{
    BIND_METHOD(get_sample_loading_state);
    BIND_METHOD(get_loading_state);
    BIND_METHOD(unload_sample_data);
    BIND_METHOD(unload);
}
} // namespace FmodGodot
