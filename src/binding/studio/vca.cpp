#include "vca.h"
#include "binding/conversions.h"
#include "core/class_db.hpp"
#include "fmod_defs.h"
#include "fmod_enums.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include <bit>

using namespace FmodGodot::Studio;
StudioVCA::StudioVCA()
{
}

void StudioVCA::_bind_methods()
{
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_id, "handle");
    BIND_STATIC_METHOD(get_path, "handle");
    BIND_STATIC_METHOD(get_final_volume, "handle");
    BIND_STATIC_METHOD(get_volume, "handle");
    BIND_STATIC_METHOD(set_volume, "handle", "volume");
}
bool StudioVCA::is_valid(Handle p_handle)
{
    return FMOD_Studio_VCA_IsValid(std::bit_cast<FMOD_STUDIO_VCA *>(p_handle));
}
godot::Vector4i StudioVCA::get_id(Handle p_handle)
{
    FMOD_GUID id;
    FMOD_Studio_VCA_GetID(std::bit_cast<FMOD_STUDIO_VCA *>(p_handle), &id);
    return FmodGodot::cast_to_vector4i(id);
}
godot::String StudioVCA::get_path(Handle p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_VCA_GetPath, std::bit_cast<FMOD_STUDIO_VCA *>(p_handle), path)
    return path;
}
float StudioVCA::get_volume(Handle p_handle)
{
    float volume;
    FMOD_Studio_VCA_GetVolume(std::bit_cast<FMOD_STUDIO_VCA *>(p_handle), &volume, nullptr);
    return volume;
}
float StudioVCA::get_final_volume(Handle p_handle)
{
    float volume;
    FMOD_Studio_VCA_GetVolume(std::bit_cast<FMOD_STUDIO_VCA *>(p_handle), nullptr, &volume);
    return volume;
}
FMOD_RESULT StudioVCA::set_volume(Handle p_handle, float p_volume)
{
    return FMOD_Studio_VCA_SetVolume(std::bit_cast<FMOD_STUDIO_VCA *>(p_handle), p_volume);
}
#ifdef TOOLS_ENABLED
FmodGodot::Studio::StudioVCA::Cache::Cache(FMOD_STUDIO_VCA *p_vca)
{
    full_path = StudioVCA::get_path((Handle)p_vca);
    guid = StudioVCA::get_id((Handle)p_vca);
}
#endif
