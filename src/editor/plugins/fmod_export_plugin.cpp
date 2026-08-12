#include "fmod_export_plugin.h"
#include "classes/project_settings.hpp"
#include "fmod_string_names.h"
#include "variant/dictionary.hpp"
using namespace godot;
using namespace FmodGodot;
void FmodGodot::FmodExportPlugin::_export_begin(const PackedStringArray &p_features, bool p_is_debug,
                                                const String &p_path, uint32_t p_flags)
{
    String plugin_path =
        ProjectSettings::get_singleton()->get_setting_with_override_and_custom_features(PLUGIN_PATH, p_features);
    Dictionary plugins =
        ProjectSettings::get_singleton()->get_setting_with_override_and_custom_features(PLUGINS, p_features);
    for (String plugin : plugins.keys())
    {
        add_shared_object(plugin, {}, plugin_path);
    }
}
godot::String FmodGodot::FmodExportPlugin::_get_name() const
{
    return "Fmod Export Plugin";
}
