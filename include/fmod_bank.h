#pragma once
#include "fmod_studio_common.h"
#include <fmod_studio.h>
#include <godot_cpp/classes/resource.hpp>
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

    bool is_bank_valid() const;

    size_t get_bank() const;
    void set_bank(size_t p_bank_ptr);
    Vector4i get_id() const;
    // string getPath(char *path, int size, int *retrieved) const;
    // Loading control
    int unload();
    int load_sample_data();
    int unload_sample_data();

    int get_loading_state() const;
    int get_sample_loading_state() const;

    // Enumeration
    // int getStringCount() const;
    // FMOD_RESULT getStringInfo(int index, FMOD_GUID *id, char *path, int size, int *retrieved) const;
    // int getEventCount() const;
    // FMOD_RESULT getEventList(FMOD_STUDIO_EVENTDESCRIPTION **array, int capacity, int *count) const;
    // int getBusCount() const;
    // FMOD_RESULT getBusList(FMOD_STUDIO_BUS **array, int capacity, int *count) const;
    // int getVCACount() const;
    // FMOD_RESULT getVCAList(FMOD_STUDIO_VCA **array, int capacity, int *count) const;

    // FMOD_RESULT getUserData(void **userdata) const;
    // FMOD_RESULT setUserData(void *userdata);
};
} // namespace FmodGodot
