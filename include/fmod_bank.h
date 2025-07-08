#pragma once
#include <fmod_studio.hpp>
#include <godot_cpp/classes/resource.hpp>
#include "fmod_globals.h"
using namespace godot;
using namespace FMOD;
namespace FmodGodot
{
    class FmodBankFormatLoader;
    class FmodBank : public Resource
    {
        friend class FmodBankFormatLoader;
        GDCLASS(FmodBank, Resource);

    private:
        FMOD::Studio::Bank *bank;
        /* data */
    protected:
        static void _bind_methods();

    public:
        FmodBank(/* args */);
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
        FMOD_RESULT getEventList(Studio::EventDescription **array, int capacity, int *count) const;
        int getBusCount() const;
        FMOD_RESULT getBusList(Studio::Bus **array, int capacity, int *count) const;
        int getVCACount() const;
        FMOD_RESULT getVCAList(Studio::VCA **array, int capacity, int *count) const;

        FMOD_RESULT getUserData(void **userdata) const;
        FMOD_RESULT setUserData(void *userdata);
    };
}