#pragma once

#include "classes/editor_export_plugin.hpp"
#include "variant/packed_string_array.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodExportPlugin : public EditorExportPlugin
{
  private:
  public:
    void _export_begin(const PackedStringArray &p_features, bool p_is_debug, const String &p_path,
                       uint32_t p_flags) override;
    bool is_feature_subset(const PackedStringArray &p_subset, const PackedStringArray &p_features);
    void _export_file(const String &p_path, const String &p_type, const PackedStringArray &p_features) override;
    String _get_name() const override;
    bool _supports_platform(const Ref<EditorExportPlatform> &p_platform) const override;
};
} // namespace FmodGodot
