#pragma once

#include "classes/h_box_container.hpp"
#include "classes/label.hpp"
#include "classes/texture_rect.hpp"
#include "classes/wrapped.hpp"
using namespace godot;
namespace FmodGodot
{
#define get_editor_theme_icon(icon) get_theme_icon(icon, "EditorIcons")
class LiveUpdateIndicator : public HBoxContainer
{
    GDCLASS(LiveUpdateIndicator, HBoxContainer)
  private:
    friend class FmodDebuggerPlugin;
    TextureRect *icon;
    Label *label;
    void _update_theme();
    void _connected();
    void _disconnected();
    bool game_running = false;
    struct
    {
        Color disconnected_color;
        Color connected_color;
        Ref<Texture> icon;
    } theme_cache;
    void _start_session();
    void _stop_session();

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    LiveUpdateIndicator();
    void connected(bool p_connected);
};
} // namespace FmodGodot
