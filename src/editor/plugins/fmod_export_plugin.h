#pragma once

#include "classes/editor_export_plugin.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodExportPlugin : public EditorExportPlugin
{
  private:
  public:
    void _export_begin(const PackedStringArray &p_features, bool p_is_debug, const String &p_path,
                       uint32_t p_flags) override;
    String _get_name() const override;
};
} // namespace FmodGodot
