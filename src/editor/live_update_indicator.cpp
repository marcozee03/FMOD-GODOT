#include "live_update_indicator.h"
#include "classes/control.hpp"
#include "fmod_audio_server.h"
#include "variant/callable.hpp"
#include "variant/callable_method_pointer.hpp"

namespace FmodGodot
{

void LiveUpdateIndicator::_update_theme()
{
    theme_cache.icon = get_editor_theme_icon("Output");
    icon->set_texture(theme_cache.icon);
    theme_cache.connected_color = get_theme_color("error_color", "Editor");
    theme_cache.disconnected_color = get_theme_color("dark_color_1", "Editor");
    if (FmodAudioServer::get_singleton()->is_live_update_connected())
    {
        _connected();
    }
    else
    {
        _disconnected();
    }
}
void LiveUpdateIndicator::_connected()
{
    icon->call_deferred("set_modulate", Color(1, 0, 0));
}
void LiveUpdateIndicator::_disconnected()
{
    icon->call_deferred("set_modulate", theme_cache.disconnected_color);
}
void LiveUpdateIndicator::_bind_methods()
{
}
void LiveUpdateIndicator::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_THEME_CHANGED: {
        _update_theme();
    }
    break;
    case NOTIFICATION_ENTER_TREE: {
        _update_theme();
        FmodAudioServer::get_singleton()->connect("live_update_connected",
                                                  callable_mp(this, &LiveUpdateIndicator::_connected));
        FmodAudioServer::get_singleton()->connect("live_update_disconnected",
                                                  callable_mp(this, &LiveUpdateIndicator::_disconnected));
    }
    break;
    case NOTIFICATION_EXIT_TREE: {
        FmodAudioServer::get_singleton()->disconnect("live_update_connected",
                                                     callable_mp(this, &LiveUpdateIndicator::_connected));
        FmodAudioServer::get_singleton()->disconnect("live_update_disconnected",
                                                     callable_mp(this, &LiveUpdateIndicator::_disconnected));
    }
    break;
    }
}
LiveUpdateIndicator::LiveUpdateIndicator()
{
    icon = memnew(TextureRect);
    icon->set_expand_mode(godot::TextureRect::EXPAND_KEEP_SIZE);
    icon->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
    icon->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(icon);
    label = memnew(Label);
    label->set_text("Live Update");
    add_child(label);
    add_theme_constant_override("separation", 0);
}
} // namespace FmodGodot
