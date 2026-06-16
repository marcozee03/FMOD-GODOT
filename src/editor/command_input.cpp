#ifdef TOOLS_ENABLED
#include "command_input.h"
#include "core/math.hpp"
#include "core/object.hpp"
#include "core/property_info.hpp"
#include "variant/variant.hpp"
#include <classes/editor_interface.hpp>
#include <classes/global_constants.hpp>
#include <classes/input_event.hpp>
#include "classes/input_event_key.hpp"
using namespace FmodGodot;
using namespace godot;
#define EDSCALE EditorInterface::get_singleton()->get_editor_scale()
void CommandInput::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("command_submitted", PropertyInfo(Variant::STRING, "command")));
}
CommandInput::CommandInput()
{
    history.reserve(history_length);
}
void CommandInput::history_push(const String &command)
{
    if (history.size() == history.get_capacity())
    {
        history.remove_at(0);
    }
    history.push_back(command);
}
void CommandInput::update_height()
{
    set_custom_minimum_size(Vector2(EDSCALE * 150, get_line_height() * Math::min(1, get_line_count())));
}
void CommandInput::_gui_input(const Ref<InputEvent> &event)
{
    if (event->is_action_pressed("ui_up"))
    {
        history_index = Math::clamp(history_index + 1, -1, (int)history.size() - 1);
        if (history_index == -1)
        {
            clear();
        }
        else
        {
            set_text(history[history_index]);
        }
        accept_event();
    }
    else if (event->is_action_pressed("ui_down"))
    {
        history_index = Math::clamp(history_index - 1, -1, (int)history.size() - 1);
        if (history_index == -1)
        {
            clear();
        }
        else
        {
            set_text(history[history_index]);
        }
        accept_event();
    }
    Ref<InputEventKey> key = event;
    if (key.is_valid() && key->is_pressed())
    {
        if (key->get_keycode_with_modifiers() == (KEY_ENTER | KEY_SHIFT))
        {
            update_height();
        }
        else if (KEY_ENTER == key->get_keycode() || KEY_ENTER == key->get_physical_keycode())
        {
            if (get_text().strip_edges().is_empty())
            {
                return;
            }
            emit_signal("command_submitted", get_text());
            if (history_index != -1 && get_text() == history[history_index])
            {
                history.remove_at(history_index);
            }
            history_push(get_text());
            history_index = -1;
            clear();
            accept_event();
        }
    }
}
#endif
