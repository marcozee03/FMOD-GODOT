#include "fmod_debugger_plugin.h"
#include "fmod_string_names.h"
namespace FmodGodot
{
void FmodDebuggerPlugin::_session_started()
{
    SceneTree::get_singleton()->call_group(LIVE_UPDATE_GROUP, "_start_session");
    set_mute(mute);
}
void FmodDebuggerPlugin::_session_stopped()
{
    SceneTree::get_singleton()->call_group(LIVE_UPDATE_GROUP, "_stop_session");
}
void FmodDebuggerPlugin::_bind_methods() {};
bool FmodDebuggerPlugin::get_mute() const
{
    return mute;
}
void FmodDebuggerPlugin::set_mute(bool p_muted)
{
    mute = p_muted;
    if (session.is_valid() && session->is_active())
    {
        session->send_message("fmod:mute_audio", {p_muted});
    }
}
void FmodDebuggerPlugin::_setup_session(int32_t p_session_id)
{
    session = get_session(p_session_id);
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
