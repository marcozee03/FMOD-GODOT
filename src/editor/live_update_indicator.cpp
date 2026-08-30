#include "live_update_indicator.h"
#include "classes/control.hpp"
#include "fmod_audio_server.h"
#include "fmod_string_names.h"
#include "globals.h"
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
    if (!game_running)
    {
        icon->call_deferred("set_modulate", Color(1, 0, 0));
    }
}
void LiveUpdateIndicator::_disconnected()
{
    if (!game_running)
    {
        icon->call_deferred("set_modulate", theme_cache.disconnected_color);
    }
}
void LiveUpdateIndicator::_start_session()
{
    game_running = true;
}
void LiveUpdateIndicator::_stop_session()
{
    game_running = false;
    _update_theme();
}

void LiveUpdateIndicator::_bind_methods()
{
    BIND_METHOD(_start_session);
    BIND_METHOD(_stop_session);
    BIND_METHOD(connected, "connected");
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
        add_to_group(LIVE_UPDATE_GROUP);
        FmodAudioServer::get_singleton()->connect("live_update_connected",
                                                  callable_mp(this, &LiveUpdateIndicator::_connected));
        FmodAudioServer::get_singleton()->connect("live_update_disconnected",
                                                  callable_mp(this, &LiveUpdateIndicator::_disconnected));
    }
    break;
    case NOTIFICATION_EXIT_TREE: {
        remove_from_group(LIVE_UPDATE_GROUP);
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
    label = memnew(Label);
    label->set_text("Live Update");
    add_child(label);
    add_child(icon);
    add_theme_constant_override("separation", 0);
}
void LiveUpdateIndicator::connected(bool p_connected)
{
    if (p_connected)
    {
        icon->call_deferred("set_modulate", Color(1, 0, 0));
    }
    else
    {
        icon->call_deferred("set_modulate", theme_cache.disconnected_color);
    }
}

} // namespace FmodGodot
