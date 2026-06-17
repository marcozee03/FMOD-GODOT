#pragma once
#include "variant/vector2i.hpp"
#ifdef TOOLS_ENABLED
#include <fmod_studio.h>
#include <godot_cpp/templates/vector.hpp>
#include <variant/string.hpp>
#include <variant/vector4i.hpp>
using namespace godot;
namespace FmodGodot
{

struct Parameter
{
    String full_path;
    Vector4i guid;
    float min_value;
    float default_value;
    float max_value;
    bool discrete;
};
struct Event
{
    String full_path;
    Vector4i guid;
    bool is3d;
    bool doppler_enabled;
    bool one_shot;
    bool stream;
    float min;
    float max;
    int lengthMS;
    Vector<Parameter> parameters;
};
struct Bank
{

    String full_path;
    Vector4i guid;
    // event and vca paths
    Vector<String> children;
    // Vector<String> vca;
};
struct VCA
{
    String full_path;
    Vector4i guid;
};

} // namespace FmodGodot
#endif
