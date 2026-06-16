#pragma once
#include "fmod_console.h"
#ifdef TOOLS_ENABLED
#include "fmod_editor_cache.h"
#include "fmod_script_client.h"
#include "fmod_theme.h"
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
    FmodTheme theme;
    static FmodEditorInterface *singleton;
    bool live_update_enabled = false;
    FmodScriptClient *script;
    FmodConsole *console;

  protected:
    static void _bind_methods();

  public:
    FmodEditorInterface();
    ~FmodEditorInterface();
    void restart_server();
    void build_banks();
    void refresh();
    const FmodEditorCache *get_cache() const;
    FmodScriptClient *get_script_client() const;
    void set_console(FmodConsole *p_console);
    void print(const String &message);
    void print_warning(const String &message);
    void print_error(const String &message);
    void print_rich(const String &message);
    const FmodTheme *get_theme() const;
    static FmodEditorInterface *get_singleton();

    void show_event_in_fmod_studio(Vector4i guid);
};
} // namespace FmodGodot
#endif
