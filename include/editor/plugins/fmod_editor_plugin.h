#pragma once
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin.hpp>
#include "fmod_event_inspector_plugin.h"
#include "fmod_audio_server.h"
#include "fmod_bank_importer.h"
#include "bank_inspector_plugin.h"
#include "fmod_event_browser.h"
using namespace godot;
namespace FmodGodot
{

    class FmodEditorPlugin : public EditorPlugin
    {
        GDCLASS(FmodEditorPlugin, EditorPlugin)
    private:
        EventInspector *eventInspector;
        FmodBankImporter *bankImporter;
        BankInspectorPlugin *bankInspector;
        FmodEventBrowser *browser;

    protected:
        static void _bind_methods();

    public:
        FmodEditorPlugin();
        ~FmodEditorPlugin();
        void _enter_tree() override;
        void _exit_tree() override;
        String _get_plugin_name() const override;
        Ref<Texture2D> _get_plugin_icon() const override;
    };
}
#endif