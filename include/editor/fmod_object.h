#pragma once
#include <variant/string.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <variant/vector4i.hpp>
#include <iostream>
#include <fmod_studio.h>
using namespace godot;
namespace FmodGodot
{

    struct Parameter
    {
        friend std::ostream &operator<<(std::ostream &stream, const Parameter object);
        String full_path;
        Vector4i guid;
        float min_value;
        float default_value;
        float max_value;
        bool discrete;
    };
    struct Event
    {
        friend std::ostream &operator<<(std::ostream &stream, const Event object);
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

        friend std::ostream &operator<<(std::ostream &stream, const Bank object);
        String full_path;
        Vector4i guid;
        Vector<String> events;
    };
    struct VCA
    {
        friend std::ostream &operator<<(std::ostream &stream, const VCA object);
        String full_path;
        Vector4i guid;
    };

}