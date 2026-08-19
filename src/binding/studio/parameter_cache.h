
#pragma once
#include "binding/conversions.h"
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
    bool discrete;
    ParameterCache()
    {
    }
    ParameterCache(const FMOD_STUDIO_PARAMETER_DESCRIPTION &p_description)
    {
        full_path = String("param:/") + p_description.name;
        guid = cast_to_vector4i(p_description.guid);
        default_value = p_description.defaultvalue;
        discrete = p_description.flags & FMOD_STUDIO_PARAMETER_DISCRETE;
        min_value = p_description.minimum;
        max_value = p_description.maximum;
    }
};

} // namespace FmodGodot
