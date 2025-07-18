#pragma once
#ifdef TOOLS_ENABLED
#include "fmod_editor_cache.h"
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

    protected:
        static void _bind_methods();

    public:
        FmodEditorInterface();
        ~FmodEditorInterface();
        void restart_server();
        void restart_server_with_live_update(short port);
        void build_banks();
        void refresh();
        const FmodEditorCache *get_cache() const;
        const FmodTheme *get_theme() const;
        static FmodEditorInterface *get_singleton();
    };
}
#endif