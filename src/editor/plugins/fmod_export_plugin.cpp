#include "fmod_export_plugin.h"
#include "classes/dir_access.hpp"
#include "classes/file_access.hpp"
#include "classes/os.hpp"
#include "classes/project_settings.hpp"
#include "fmod_string_names.h"
#include "variant/dictionary.hpp"
#include "variant/utility_functions.hpp"
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
bool FmodGodot::FmodExportPlugin::is_feature_subset(const PackedStringArray &p_subset,
                                                    const PackedStringArray &p_features)
{
    for (const String &str : p_subset)
    {
        if (!p_features.has(str))
        {
            return false;
        }
    }
    return true;
}

void FmodGodot::FmodExportPlugin::_export_file(const String &p_path, const String &p_type,
                                               const PackedStringArray &p_features)
{
    if (p_type == "FmodBank")
    {
        int source_type = ProjectSettings::get_singleton()->get_setting_with_override(SOURCE_TYPE);
        String bank_directory = ProjectSettings::get_singleton()->get_setting_with_override(BANK_DIRECTORY);
        if (source_type == 0) // Single Platform Build
        {
            return;
        }
        else
        {
            if (p_path.get_base_dir().get_base_dir() != bank_directory)
            {
                UtilityFunctions::push_warning(vformat("Bank at \"%s\" is not inside Bank Directory: \"%s\" which is "
                                                       "necessary for a Multi Platform Build",
                                                       p_path, bank_directory));
            }
            String parent_directory = p_path.get_base_dir().get_file();
            if (is_feature_subset(parent_directory.split("."), p_features))
            {
                return;
            }
            // Check if is base platform
            for (int i = 0; i < parent_directory.get_slice_count("."); i++)
            {
                String slice = parent_directory.get_slice(".", i);
                if (!OS::get_singleton()->has_feature(slice))
                {
                    skip();
                    return;
                }
            }

            Ref<DirAccess> dir = DirAccess::open(bank_directory);
            for (const auto &entry : dir->get_directories())
            {
                if (entry == parent_directory)
                {
                    continue;
                }
                else
                {
                    if (is_feature_subset(entry.split("."), p_features))
                    {
                        for (const auto &file : DirAccess::get_files_at(dir->get_current_dir().path_join(entry)))
                        {
                            if (p_path.get_file() == file)
                            {
                                add_file(p_path,
                                         FileAccess::get_file_as_bytes(bank_directory.path_join(entry).path_join(file)),
                                         true);
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

godot::String FmodGodot::FmodExportPlugin::_get_name() const
{
    return "Fmod Export Plugin";
}
