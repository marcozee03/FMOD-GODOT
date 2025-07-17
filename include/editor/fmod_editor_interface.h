#include "fmod_editor_cache.h"
namespace FmodGodot
{
    class FmodEditorInterface
    {
    private:
        /* data */
        FmodEditorCache cache;
    public:
        FmodEditorInterface() = default;
        ~FmodEditorInterface() = default;
        void restart_server();
        void restart_server_with_live_update(short port);
        void build_banks();
        void refresh();
    };
}