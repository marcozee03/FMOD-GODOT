#include "fmod_bank.h"
#include "fmod_audio_server.h"
namespace FmodGodot
{
    FmodBank::FmodBank()
    {
    }

    FmodBank::~FmodBank()
    {
        unload();
    }

    FMOD_RESULT FmodBank::unload()
    {
        return FMOD_Studio_Bank_Unload(bank);
    }
    FMOD_RESULT FmodBank::loadSampleData()
    {
        return FMOD_Studio_Bank_LoadSampleData(bank);
    }
    FMOD_RESULT FmodBank::unloadSampleData()
    {
        return FMOD_Studio_Bank_Unload(bank);
    }
    FMOD_STUDIO_LOADING_STATE FmodBank::getLoadingState() const
    {
        FMOD_STUDIO_LOADING_STATE state;
        FMOD_Studio_Bank_GetLoadingState(bank, &state);
        return state;
    }
    FMOD_STUDIO_LOADING_STATE FmodBank::getSampleLoadingState() const
    {
        FMOD_STUDIO_LOADING_STATE state;
        FMOD_Studio_Bank_GetSampleLoadingState(bank, &state);
        return state;
    }
    void FmodBank::_bind_methods() {}
}