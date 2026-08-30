#include "mute_button.h"
#include "classes/editor_interface.hpp"
#include "classes/engine_debugger.hpp"
#include "fmod_editor_interface.h"
namespace FmodGodot
{

void MuteButton::_bind_methods()
{
}
void MuteButton::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_THEME_CHANGED:
    case NOTIFICATION_ENTER_TREE:
        if (is_pressed())
        {
            set_button_icon(get_theme_icon("AudioMute", "EditorIcons"));
        }
        else
        {
            set_button_icon(get_theme_icon("AudioStreamPlayer", "EditorIcons"));
        }
        break;
    }
}
MuteButton::MuteButton()
{
    set_toggle_mode(true);
    set_theme_type_variation("FlatButton");
}
void MuteButton::_toggled(bool p_toggled_on)
{
    FmodEditorInterface::get_singleton()->set_mute_remote(p_toggled_on);
    if (p_toggled_on)
    {
        set_button_icon(get_theme_icon("AudioMute", "EditorIcons"));
    }
    else
    {
        set_button_icon(get_theme_icon("AudioStreamPlayer", "EditorIcons"));
    }
}

} // namespace FmodGodot
