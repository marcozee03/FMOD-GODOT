#pragma once
#ifdef TOOLS_ENABLED
#include "classes/texture2d.hpp"
using namespace godot;
namespace FmodGodot
{
    class FmodTheme
    {
    public:
        Ref<Texture2D> event_icon;
        Ref<Texture2D> c_parameter_icon;
        Ref<Texture2D> d_parameter_icon;
        Ref<Texture2D> bank_icon;

    public:
        FmodTheme(/* args */);
        ~FmodTheme();
    };
}
#endif