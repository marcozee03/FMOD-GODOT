#pragma once
#include "classes/editor_debugger_plugin.hpp"
#include "classes/editor_debugger_session.hpp"
#include "classes/scene_tree.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodDebuggerPlugin : public EditorDebuggerPlugin
{
    GDCLASS(FmodDebuggerPlugin, EditorDebuggerPlugin)
  private:
    void _session_started();
    void _session_stopped();
    bool mute = false;

  protected:
    static void _bind_methods();

    int session_id;

    Ref<EditorDebuggerSession> session = nullptr;

  public:
    bool get_mute() const;
    void set_mute(bool p_muted);
    void _setup_session(int32_t p_session_id) override;
    bool _has_capture(const String &p_capture) const override;
    bool _capture(const String &p_message, const Array &p_data, int32_t p_session_id) override;
};
} // namespace FmodGodot
