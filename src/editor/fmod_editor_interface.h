#pragma once
#include "fmod_console.h"
#include "fmod_debugger_plugin.h"
#include "fmod_editor_cache.h"
#include "fmod_script_client.h"
#include "variant/string.hpp"
#include <classes/object.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEditorInterface : public Object
{
    GDCLASS(FmodEditorInterface, Object)
  private:
    /* data */
    FmodEditorCache cache;
    static FmodEditorInterface *singleton;
    FmodScriptClient *script = nullptr;
    FmodConsole *console = nullptr;
    Ref<FmodDebuggerPlugin> debugger = nullptr;

  protected:
    static void _bind_methods();

  public:
    FmodEditorInterface();
    ~FmodEditorInterface();
    void restart_server();
    void build_banks();
    void refresh(bool p_load_start_up_banks = false);
    const FmodEditorCache *get_cache() const;
    FmodScriptClient *get_script_client() const;
    void register_console(FmodConsole *p_console);
    void register_debugger(Ref<FmodDebuggerPlugin> p_debugger);
    bool get_mute_remote(bool p_muted) const;
    void set_mute_remote(bool p_muted);
    void print(const String &p_message);
    void print_warning(const String &p_message);
    void print_error(const String &p_message);
    void print_rich(const String &p_message);
    static FmodEditorInterface *get_singleton();

    void show_event_in_fmod_studio(Vector4i p_guid);
};
} // namespace FmodGodot
