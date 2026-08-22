#pragma once

#include "binding/conversions.h"
#include "fmod_studio_common.h"
#include "variant/vector2i.hpp"
#include <fmod_studio.h>
#include <godot_cpp/templates/vector.hpp>
#include <variant/string.hpp>
#include <variant/vector4i.hpp>

using namespace godot;
namespace FmodGodot
{

struct ParameterCache
{
    String full_path;
    Vector4i guid;
    float min_value;
    float default_value;
    float max_value;
    FMOD_STUDIO_PARAMETER_FLAGS flags;
    constexpr bool is_readonly()
    {
        return flags & FMOD_STUDIO_PARAMETER_READONLY;
    }
    constexpr bool is_automatic()
    {
        return flags & FMOD_STUDIO_PARAMETER_AUTOMATIC;
    }
    constexpr bool is_global()
    {
        return flags & FMOD_STUDIO_PARAMETER_GLOBAL;
    }
    constexpr bool is_discrete()
    {
        return flags & FMOD_STUDIO_PARAMETER_DISCRETE;
    }
    constexpr bool is_labeled()
    {
        return flags & (FMOD_STUDIO_PARAMETER_LABELED | FMOD_STUDIO_PARAMETER_DISCRETE);
    }
    ParameterCache()
    {
    }
    ParameterCache(const FMOD_STUDIO_PARAMETER_DESCRIPTION &p_description)
    {
        full_path = String("param:/") + p_description.name;
        guid = cast_to_vector4i(p_description.guid);
        default_value = p_description.defaultvalue;
        flags = p_description.flags;
        min_value = p_description.minimum;
        max_value = p_description.maximum;
    }
};

} // namespace FmodGodot
