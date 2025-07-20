#pragma once
#include <fmod_studio.h>
#include <godot_cpp/classes/resource.hpp>
#include "fmod_globals.h"
using namespace godot;
namespace FmodGodot
{
    class FmodBankFormatLoader;
    class FmodBank : public Resource
    {
        friend class FmodBankFormatLoader;
        friend class FmodBankLoader;
        friend class FmodAudioServer;
        GDCLASS(FmodBank, Resource);

    private:
        FMOD_STUDIO_BANK *bank;

    protected:
        static void _bind_methods();

    public:
        FmodBank();
        ~FmodBank();

        bool isValid() const;

        // Property access
        Vector4i getID() const;
        // string getPath(char *path, int size, int *retrieved) const;
        // Loading control
        FMOD_RESULT unload();
        FMOD_RESULT loadSampleData();
        FMOD_RESULT unloadSampleData();

        FMOD_STUDIO_LOADING_STATE getLoadingState() const;
        FMOD_STUDIO_LOADING_STATE getSampleLoadingState() const;

        // Enumeration
        int getStringCount() const;
        FMOD_RESULT getStringInfo(int index, FMOD_GUID *id, char *path, int size, int *retrieved) const;
        int getEventCount() const;
        FMOD_RESULT getEventList(FMOD_STUDIO_EVENTDESCRIPTION **array, int capacity, int *count) const;
        int getBusCount() const;
        FMOD_RESULT getBusList(FMOD_STUDIO_BUS **array, int capacity, int *count) const;
        int getVCACount() const;
        FMOD_RESULT getVCAList(FMOD_STUDIO_VCA **array, int capacity, int *count) const;

        FMOD_RESULT getUserData(void **userdata) const;
        FMOD_RESULT setUserData(void *userdata);
    };
}