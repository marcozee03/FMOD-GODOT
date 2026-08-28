#include "fmod_debugger_plugin.h"
#include "classes/label.hpp"
#include "core/print_string.hpp"
#include "fmod.h"
#include "variant/variant.hpp"
namespace FmodGodot
{
void FmodDebuggerPlugin::_session_started()
{
    SceneTree::get_singleton()->call_group(LIVE_UPDATE_GROUP, "_start_session");
}
void FmodDebuggerPlugin::_session_stopped()
{
    SceneTree::get_singleton()->call_group(LIVE_UPDATE_GROUP, "_stop_session");
}
void FmodDebuggerPlugin::_bind_methods() {};
void FmodDebuggerPlugin::_setup_session(int32_t p_session_id)
{
    Ref<EditorDebuggerSession> session = get_session(p_session_id);
    session->connect("started", callable_mp(this, &FmodDebuggerPlugin::_session_started));
    session->connect("stopped", callable_mp(this, &FmodDebuggerPlugin::_session_stopped));
}
bool FmodDebuggerPlugin::_has_capture(const String &p_capture) const
{
    return p_capture == "fmod";
}
bool FmodDebuggerPlugin::_capture(const String &p_message, const Array &p_data, int32_t p_session_id)
{
    if (p_message == "fmod:live_update")
    {
        SceneTree::get_singleton()->call_group(LIVE_UPDATE_GROUP, "connected", static_cast<bool>(p_data[0]));
        return true;
    }
    return false;
}
} // namespace FmodGodot
