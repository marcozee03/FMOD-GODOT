#pragma once
#include "core/print_string.hpp"
#include "fmod_globals.h"
#include "variant/string.hpp"
#include "variant/utility_functions.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_editor_cache.h"
#include "fmod_script_client.h"
#include "fmod_theme.h"
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

  protected:
    static void _bind_methods();

  public:
    FmodEditorInterface();
    ~FmodEditorInterface();
    void restart_server();
    void build_banks();
    void refresh();
    const FmodEditorCache *get_cache() const;
    const FmodTheme *get_theme() const;
    static FmodEditorInterface *get_singleton();

    void show_event_in_fmod_studio(Vector4i guid);

};
} // namespace FmodGodot
#endif
