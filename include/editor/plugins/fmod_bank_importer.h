#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/variant/string.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodBankImporter : public EditorImportPlugin
    {
        GDCLASS(FmodBankImporter, EditorImportPlugin);

    protected:
        static void _bind_methods();

    public:
        FmodBankImporter(/* args */);
        ~FmodBankImporter();
        float _get_priority() const override;
        String _get_importer_name() const override;
        String _get_visible_name() const override;
        PackedStringArray _get_recognized_extensions() const override;
        String _get_save_extension() const override;
        String _get_resource_type() const override;
        int32_t _get_preset_count() const override;
        int32_t _get_import_order() const override;
        TypedArray<Dictionary> _get_import_options(const String &p_path, int32_t p_preset_index) const override;
        Error _import(const String &p_source_file, const String &p_save_path, const Dictionary &p_options, const TypedArray<String> &p_platform_variants, const TypedArray<String> &p_gen_files) const override;
    };
}
#endif