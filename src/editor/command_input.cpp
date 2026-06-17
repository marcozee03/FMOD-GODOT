#ifdef TOOLS_ENABLED
#include "classes/input_event_key.hpp"
#include "command_input.h"
#include "core/math.hpp"
#include "core/object.hpp"
#include "core/property_info.hpp"
#include "variant/variant.hpp"
#include <classes/editor_interface.hpp>
#include <classes/global_constants.hpp>
#include <classes/input_event.hpp>
using namespace FmodGodot;
using namespace godot;
#define EDSCALE EditorInterface::get_singleton()->get_editor_scale()
void CommandInput::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("command_submitted", PropertyInfo(Variant::STRING, "command")));
}
CommandInput::CommandInput()
{
    set_fit_content_height_enabled(true);
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
void FmodGodot::CommandInput::_enter_tree()
{
    set_line_wrapping_mode(TextEdit::LINE_WRAPPING_BOUNDARY);
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
            set_text(history[history.size() - history_index - 1]);
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
            set_text(history[history.size() - history_index - 1]);
        }
        accept_event();
    }
    Ref<InputEventKey> key = event;
    if (key.is_valid() && key->is_pressed())
    {
        if (key->get_keycode_with_modifiers() == (Key)(KEY_ENTER | KEY_MASK_SHIFT))
        {
            insert_text_at_caret("\n");
            accept_event();
        }
        else if (key->get_keycode_with_modifiers() == KEY_ENTER)
        {
            if (get_text().strip_edges().is_empty())
            {
                return;
            }
            emit_signal("command_submitted", get_text());
            if (history_index != -1 && get_text() == history[history.size() - history_index - 1])
            {
                history.remove_at(history.size() - history_index - 1);
            }
            history_push(get_text());
            history_index = -1;
            clear();
            accept_event();
        }
    }
}
godot::PackedStringArray FmodGodot::CommandInput::get_history() const
{
    PackedStringArray arr;
    arr.resize(history.size());
    for (int i = 0; i < history.size(); i++)
    {
        arr[i] = history[i];
    }
    return arr;
}
void FmodGodot::CommandInput::load_history(const PackedStringArray &arr)
{
    history.clear();
    history.reserve(history_length);
    for (int i = 0; i < arr.size(); i++)
    {
        history.push_back(arr[i]);
    }
};
#endif
