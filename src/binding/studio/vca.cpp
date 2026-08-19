#include "vca.h"
#include "binding/conversions.h"
#include "fmod_enums.h"
#include "fmod_globals.h"
#include "fmod_studio.h"
#include "globals.h"
#include "variant/variant.hpp"

using namespace FmodGodot::Studio;
VCA::VCA()
{
}
VCA::VCA(FMOD_STUDIO_VCA *p_handle) : handle(p_handle)
{
}

void VCA::_bind_methods()
{
    BIND_METHOD(is_valid);
    BIND_METHOD(get_id);
    BIND_METHOD(get_path);
    BIND_METHOD(get_volume);
    BIND_METHOD(get_final_volume);
    BIND_PROPERTY(volume, Variant::FLOAT);
}
bool VCA::is_valid() const
{
    return FMOD_Studio_VCA_IsValid(handle);
}
godot::Vector4i VCA::get_id() const
{
    FMOD_GUID id;
    FMOD_Studio_VCA_GetID(handle, &id);
    return FmodGodot::cast_to_vector4i(id);
}
godot::String VCA::get_path() const
{
    FMOD_GET_STRING(FMOD_Studio_VCA_GetPath, handle, path)
    return path;
}
float VCA::get_volume() const
{
    float volume;
    FMOD_Studio_VCA_GetVolume(handle, &volume, nullptr);
    return volume;
}
float VCA::get_final_volume() const
{
    float volume;
    FMOD_Studio_VCA_GetVolume(handle, nullptr, &volume);
    return volume;
}
FMOD_RESULT VCA::set_volume(float p_volume)
{
    return FMOD_Studio_VCA_SetVolume(handle, p_volume);
}
#ifdef TOOLS_ENABLED
FmodGodot::Studio::VCA::Cache::Cache(FMOD_STUDIO_VCA *p_vca)
{
    VCA vca(p_vca);
    full_path = vca.get_path();
    guid = vca.get_id();
}
#endif
