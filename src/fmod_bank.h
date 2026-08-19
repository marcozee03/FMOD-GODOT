#pragma once
#include "binding/conversions.h"
#include "fmod_globals.h"
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
    struct Cache
    {
        Cache()
        {
        }
        Cache(FMOD_STUDIO_BANK *p_bank)
        {
            FMOD_GET_OUT_STRING(FMOD_Studio_Bank_GetPath, p_bank, full_path);
            FMOD_GUID fguid;
            FMOD_Studio_Bank_GetID(p_bank, &fguid);
            guid = cast_to_vector4i(fguid);
            {
                int event_count;
                FMOD_Studio_Bank_GetEventCount(p_bank, &event_count);
                if (event_count > 0)
                {
                    FMOD_STUDIO_EVENTDESCRIPTION **descriptions =
                        memnew_arr(FMOD_STUDIO_EVENTDESCRIPTION *, event_count);
                    FMOD_Studio_Bank_GetEventList(p_bank, descriptions, event_count, &event_count);
                    for (int j = 0; j < event_count; j++)
                    {
                        FMOD_GET_STRING(FMOD_Studio_EventDescription_GetPath, descriptions[j], event_path);
                        children.push_back(event_path);
                    }
                    memdelete_arr(descriptions);
                }
            }
            {
                int vca_count;
                FMOD_Studio_Bank_GetVCACount(p_bank, &vca_count);
                if (vca_count > 0)
                {
                    FMOD_STUDIO_VCA **vcas = memnew_arr(FMOD_STUDIO_VCA *, vca_count);
                    FMOD_Studio_Bank_GetVCAList(p_bank, vcas, vca_count, &vca_count);
                    for (int j = 0; j < vca_count; j++)
                    {
                        FMOD_GET_STRING(FMOD_Studio_VCA_GetPath, vcas[j], vca_path);
                        children.push_back(vca_path);
                    }
                    memdelete_arr(vcas);
                }
            }
        }
        String full_path;
        Vector4i guid;
        Vector<String> children;
    };
    FmodBank();
    ~FmodBank();

    bool is_bank_valid() const;

    size_t get_bank() const;
    void set_bank(size_t p_bank_ptr);
    Vector4i get_id() const;
    // string getPath(char *path, int size, int *retrieved) const;
    // Loading control
    int unload();
    int reload();
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
