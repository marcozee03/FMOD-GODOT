#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_inspector_plugin.hpp>
#include <godot_cpp/core/memory.hpp>
using namespace godot;
namespace FmodGodot
{
    class BankInspectorPlugin : public EditorInspectorPlugin
    {
        GDCLASS(BankInspectorPlugin, EditorInspectorPlugin)
    protected:
        static void _bind_methods();
    public:
        void _parse_begin(Object *p_object) override;
        bool _can_handle(Object *p_object) const override;
    };
}
#endif