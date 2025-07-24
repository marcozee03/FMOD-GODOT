#ifdef TOOLS_ENABLED
#include "fmod_theme.h"
#include <classes/resource_loader.hpp>
namespace FmodGodot
{
    FmodTheme::FmodTheme(/* args */)
    {
        bank_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_icon.svg");
        event_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
        c_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/c_parameter_icon.svg");
        d_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/d_parameter_icon.svg");
        bank_loader_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_loader_icon.svg");
        snapshot_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/snapshot_icon.svg");
        fmod_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/fmod_icon.svg");
        vca_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/vca_icon.svg");
    }

    FmodTheme::~FmodTheme()
    {


    }
}
#endif