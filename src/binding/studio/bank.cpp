#include "bank.h"
#include "fmod_defs.h"
#include "fmod_enums.h"
using namespace FmodGodot;
using namespace Studio;
bool FmodGodot::Studio::Bank::is_valid(Handle p_handle)
{
    return FMOD_Studio_Bank_IsValid(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle));
}
godot::Vector4i FmodGodot::Studio::Bank::get_id(Handle p_handle)
{
    FMOD_GUID guid;
    FMOD_Studio_Bank_GetID(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &guid);
    return cast_to_vector4i(guid);
}
godot::String FmodGodot::Studio::Bank::get_path(Handle p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_Bank_GetPath, std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), path)
    return path;
}
FMOD_RESULT FmodGodot::Studio::Bank::unload(Handle p_handle)
{
    return FMOD_Studio_Bank_Unload(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle));
}
FMOD_RESULT FmodGodot::Studio::Bank::load_sample_data(Handle p_handle)
{
    return FMOD_Studio_Bank_LoadSampleData(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle));
}
FMOD_RESULT FmodGodot::Studio::Bank::unload_sample_data(Handle p_handle)
{

    return FMOD_Studio_Bank_UnloadSampleData(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle));
}
FMOD_STUDIO_LOADING_STATE FmodGodot::Studio::Bank::get_loading_state(Handle p_handle)
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetLoadingState(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &state);
    return state;
}
FMOD_STUDIO_LOADING_STATE FmodGodot::Studio::Bank::get_sample_loading_state(Handle p_handle)
{
    FMOD_STUDIO_LOADING_STATE state;
    FMOD_Studio_Bank_GetSampleLoadingState(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &state);
    return state;
}
int FmodGodot::Studio::Bank::get_string_count(Handle p_handle)
{
    int count;
    FMOD_Studio_Bank_GetStringCount(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &count);
    return count;
}
godot::String FmodGodot::Studio::Bank::get_string_info(Handle p_handle, int p_index, const Vector4i &p_id)
{
    FMOD_LOOKUP_STRING(FMOD_Studio_Bank_GetStringInfo, std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), str, p_index,
                       std::bit_cast<FMOD_GUID *>(&p_id));
    return str;
}
int FmodGodot::Studio::Bank::get_event_count(Handle p_handle)
{
    int count;
    FMOD_Studio_Bank_GetEventCount(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &count);
    return count;
}
godot::PackedInt64Array FmodGodot::Studio::Bank::get_event_list(Handle p_handle)
{
    PackedInt64Array ret;
    ret.resize(get_event_count(p_handle));
    if (ret.size() == 0)
    {
        return ret;
    }
    FMOD_Studio_Bank_GetEventList(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle),
                                  std::bit_cast<FMOD_STUDIO_EVENTDESCRIPTION **>(ret.ptrw()), ret.size(), nullptr);
    return ret;
}
int FmodGodot::Studio::Bank::get_bus_count(Handle p_handle)
{
    int count;
    FMOD_Studio_Bank_GetBusCount(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &count);
    return count;
}
godot::PackedInt64Array FmodGodot::Studio::Bank::get_bus_list(Handle p_handle)
{
    PackedInt64Array ret;
    ret.resize(get_bus_count(p_handle));
    if (ret.size() == 0)
    {
        return ret;
    }
    FMOD_Studio_Bank_GetBusList(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle),
                                std::bit_cast<FMOD_STUDIO_BUS **>(ret.ptrw()), ret.size(), nullptr);
    return ret;
}
int FmodGodot::Studio::Bank::get_vca_count(Handle p_handle)
{
    int count;
    FMOD_Studio_Bank_GetVCACount(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &count);
    return count;
}
godot::PackedInt64Array FmodGodot::Studio::Bank::get_vca_list(Handle p_handle)
{
    PackedInt64Array ret;
    ret.resize(get_vca_count(p_handle));
    if (ret.size() == 0)
    {
        return ret;
    }
    FMOD_Studio_Bank_GetVCAList(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle),
                                std::bit_cast<FMOD_STUDIO_VCA **>(ret.ptrw()), ret.size(), nullptr);
    return ret;
}
void *FmodGodot::Studio::Bank::get_user_data(Handle p_handle)
{
    void *userdata;
    FMOD_Studio_Bank_GetUserData(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), &userdata);
    return userdata;
}
FMOD_RESULT FmodGodot::Studio::Bank::set_user_data(Handle p_handle, void *p_userdata)
{
    return FMOD_Studio_Bank_SetUserData(std::bit_cast<FMOD_STUDIO_BANK *>(p_handle), p_userdata);
}
