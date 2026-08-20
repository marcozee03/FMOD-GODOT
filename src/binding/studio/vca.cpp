#include "vca.h"
#include "binding/conversions.h"
#include "core/class_db.hpp"
#include "fmod_enums.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include <cstddef>

using namespace FmodGodot::Studio;
VCA::VCA()
{
}

void VCA::_bind_methods()
{
    BIND_STATIC_METHOD(is_valid, "handle");
    BIND_STATIC_METHOD(get_id, "handle");
    BIND_STATIC_METHOD(get_path, "handle");
    BIND_STATIC_METHOD(get_final_volume, "handle");
    BIND_STATIC_METHOD(get_volume, "handle");
    BIND_STATIC_METHOD(set_volume, "handle", "volume");
}
bool VCA::is_valid(size_t p_handle)
{
    return FMOD_Studio_VCA_IsValid((FMOD_STUDIO_VCA *)p_handle);
}
godot::Vector4i VCA::get_id(size_t p_handle)
{
    FMOD_GUID id;
    FMOD_Studio_VCA_GetID((FMOD_STUDIO_VCA *)p_handle, &id);
    return FmodGodot::cast_to_vector4i(id);
}
godot::String VCA::get_path(size_t p_handle)
{
    FMOD_GET_STRING(FMOD_Studio_VCA_GetPath, (FMOD_STUDIO_VCA *)p_handle, path)
    return path;
}
float VCA::get_volume(size_t p_handle)
{
    float volume;
    FMOD_Studio_VCA_GetVolume((FMOD_STUDIO_VCA *)p_handle, &volume, nullptr);
    return volume;
}
float VCA::get_final_volume(size_t p_handle)
{
    float volume;
    FMOD_Studio_VCA_GetVolume((FMOD_STUDIO_VCA *)p_handle, nullptr, &volume);
    return volume;
}
FMOD_RESULT VCA::set_volume(size_t p_handle, float p_volume)
{
    return FMOD_Studio_VCA_SetVolume((FMOD_STUDIO_VCA *)p_handle, p_volume);
}
#ifdef TOOLS_ENABLED
FmodGodot::Studio::VCA::Cache::Cache(FMOD_STUDIO_VCA *p_vca)
{
    full_path = VCA::get_path((size_t)p_vca);
    guid = VCA::get_id((size_t)p_vca);
}
#endif
