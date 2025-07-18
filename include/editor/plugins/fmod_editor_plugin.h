#pragma once
// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <classes/editor_plugin.hpp>
#include "fmod_event_inspector_plugin.h"
#include "fmod_audio_server.h"
#include "fmod_bank_importer.h"
#include "bank_inspector_plugin.h"
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
    protected:
        static void _bind_methods();

    public:
        FmodEditorPlugin();
        ~FmodEditorPlugin();
    };
}
#endif