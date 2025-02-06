#pragma once
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin.hpp>
#include "fmod_event_inspector_plugin.h"
#include "bank_loader_inspector_plugin.h"
#include "fmodengine.h"
using namespace godot;
namespace FMODGodot
{

    class FmodEditorPlugin : public EditorPlugin
    {
        GDCLASS(FmodEditorPlugin, EditorPlugin)
    private:
        EventInspector *eventInspector;
        BankLoaderInspector *bankLoaderInspector;
        FMODEngine *fmodEngine;

    protected:
        static void _bind_methods();

    public:
        FmodEditorPlugin();
        ~FmodEditorPlugin() = default;
        void _enter_tree() override;
        void _exit_tree() override;
    };
}
#endif