#pragma once
#include "classes/editor_interface.hpp"
#ifdef TOOLS_ENABLED
#include "bank_inspector_plugin.h"
#include "fmod_bank_importer.h"
#include "fmod_console.h"
#include "fmod_event_browser.h"
#include "fmod_event_inspector_plugin.h"
#include "fmod_export_plugin.h"
#include "fmod_installer.h"
#include <classes/editor_plugin.hpp>
using namespace godot;
namespace FmodGodot
{

class FmodEditorPlugin : public EditorPlugin
{
    GDCLASS(FmodEditorPlugin, EditorPlugin)
  private:
    Ref<EventInspector> eventInspector = nullptr;
    Ref<FmodBankImporter> bankImporter = nullptr;
    Ref<BankInspectorPlugin> bankInspector = nullptr;
    FmodEventBrowser *browser = nullptr;
    FmodConsole *log = nullptr;
    FmodInstaller *installer = nullptr;
    Ref<FmodExportPlugin> export_plugin = nullptr;
    uint64_t installer_id;
    void _open_installer();

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
} // namespace FmodGodot
#endif