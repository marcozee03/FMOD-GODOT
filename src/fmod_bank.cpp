#include "fmod_bank.h"
#include "fmod_audio_server.h"
namespace FmodGodot
{
    FmodBank::FmodBank()
    {
    }

    FmodBank::~FmodBank()
    {
    }

    FMOD_RESULT FmodBank::unload()
    {
        return bank->unload();
    }
    FMOD_RESULT FmodBank::loadSampleData()
    {
        return bank->loadSampleData();
    }
    FMOD_RESULT FmodBank::unloadSampleData()
    {
        return bank->unloadSampleData();
    }
    FMOD_STUDIO_LOADING_STATE FmodBank::getLoadingState() const
    {
        return FMOD_STUDIO_LOADING_STATE();
    }
    FMOD_STUDIO_LOADING_STATE FmodBank::getSampleLoadingState() const
    {
        return FMOD_STUDIO_LOADING_STATE();
    }
    void FmodBank::_bind_methods() {}
}