#ifdef TOOLS_ENABLED
#include "fmod_console.h"
#include "classes/config_file.hpp"
#include "classes/h_flow_container.hpp"
#include "classes/line_edit.hpp"
#include "classes/script_editor.hpp"
#include "classes/v_box_container.hpp"
#include "fmod_script_client.h"
#include "variant/callable_method_pointer.hpp"
#include "variant/packed_string_array.hpp"
#include "variant/string.hpp"
#include <classes/script.hpp>

#include "classes/display_server.hpp"
#include "classes/editor_dock.hpp"
#include "classes/editor_interface.hpp"
#include "classes/editor_settings.hpp"
#include "classes/flow_container.hpp"
#include "classes/font.hpp"
#include "classes/h_box_container.hpp"
#include "classes/input_event.hpp"
#include "classes/os.hpp"
#include "classes/resource_loader.hpp"

using namespace FmodGodot;

#define EDSCALE EditorInterface::get_singleton()->get_editor_scale()
#define EDITOR_GET(setting) EditorInterface::get_singleton()->get_editor_settings()->get(setting)
#define ED_IS_SHORTCUT(name, event) EditorInterface::get_singleton()->get_editor_settings()->is_shortcut(name, event)
#define SNAME(str) str
#define EditorStringName(sn) #sn
#define SceneStringName(sn) #sn
#define get_editor_theme_icon(icon) get_theme_icon(icon, "EditorIcons")
void FmodConsole::_update_theme()
{
    const Ref<Font> normal_font = get_theme_font(SNAME("output_source"), EditorStringName(EditorFonts));
    if (normal_font.is_valid())
    {
        log->add_theme_font_override("normal_font", normal_font);
    }

    const Ref<Font> bold_font = get_theme_font(SNAME("output_source_bold"), EditorStringName(EditorFonts));
    if (bold_font.is_valid())
    {
        log->add_theme_font_override("bold_font", bold_font);
    }

    const Ref<Font> italics_font = get_theme_font(SNAME("output_source_italic"), EditorStringName(EditorFonts));
    if (italics_font.is_valid())
    {

        log->add_theme_font_override("italics_font", italics_font);
    }

    const Ref<Font> bold_italics_font =
        get_theme_font(SNAME("output_source_bold_italic"), EditorStringName(EditorFonts));
    if (bold_italics_font.is_valid())
    {
        log->add_theme_font_override("bold_italics_font", bold_italics_font);
    }

    const Ref<Font> mono_font = get_theme_font(SNAME("output_source_mono"), EditorStringName(EditorFonts));
    if (mono_font.is_valid())
    {
        log->add_theme_font_override("mono_font", mono_font);
    }

    // Disable padding for highlighted background/foreground to prevent highlights from overlapping on close lines.
    // This also better matches terminal output, which does not use any form of padding.
    log->add_theme_constant_override("text_highlight_h_padding", 0);
    log->add_theme_constant_override("text_highlight_v_padding", 0);

    const int font_size = get_theme_font_size(SNAME("output_source_size"), EditorStringName(EditorFonts));
    log->begin_bulk_theme_override();
    log->add_theme_font_size_override("normal_font_size", font_size);
    log->add_theme_font_size_override("bold_font_size", font_size);
    log->add_theme_font_size_override("italics_font_size", font_size);
    log->add_theme_font_size_override("mono_font_size", font_size);
    log->end_bulk_theme_override();

    const String wide_text = "MM";

    Button *button = type_filter_map[MSG_TYPE_STD]->toggle_button;
    button->set_button_icon(get_editor_theme_icon(SNAME("Popup")));
    button->set_custom_minimum_size(Vector2(button->get_minimum_size().x * EDSCALE, 0));
    button = type_filter_map[MSG_TYPE_ERROR]->toggle_button;
    button->set_button_icon(get_editor_theme_icon(SNAME("StatusError")));
    button->set_custom_minimum_size(Vector2(button->get_minimum_size().x * EDSCALE, 0));
    button = type_filter_map[MSG_TYPE_WARNING]->toggle_button;
    button->set_button_icon(get_editor_theme_icon(SNAME("StatusWarning")));
    button->set_custom_minimum_size(Vector2(button->get_minimum_size().x * EDSCALE, 0));
    button = type_filter_map[MSG_TYPE_INPUT]->toggle_button;
    button->set_button_icon(get_editor_theme_icon(SNAME("Edit")));
    button->set_custom_minimum_size(Vector2(button->get_minimum_size().x * EDSCALE, 0));

    clear_button->set_button_icon(get_editor_theme_icon(SNAME("Clear")));
    collapse_button->set_button_icon(get_editor_theme_icon(SNAME("CombineLines")));

    theme_cache.error_color = get_theme_color(SNAME("error_color"), EditorStringName(Editor));
    theme_cache.error_icon = get_editor_theme_icon(SNAME("Error"));
    theme_cache.disconnected_icon = get_editor_theme_icon(SNAME("StatusError"));
    theme_cache.warning_color = get_theme_color(SNAME("warning_color"), EditorStringName(Editor));
    theme_cache.warning_icon = get_editor_theme_icon(SNAME("Warning"));
    theme_cache.connected_icon = get_editor_theme_icon(SNAME("StatusSuccess"));
    theme_cache.message_color =
        get_theme_color(SceneStringName(font_color), EditorStringName(Editor)) * Color(1, 1, 1, 0.6);
}

void FmodConsole::_editor_settings_changed()
{
    int new_line_limit = int(EDITOR_GET("run/output/max_lines"));
    if (new_line_limit != line_limit)
    {
        line_limit = new_line_limit;
        _rebuild_log();
    }
}

void FmodConsole::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_ENTER_TREE: {
        _update_theme();
    }
    break;
    case NOTIFICATION_THEME_CHANGED: {
        _update_theme();
        _rebuild_log();
    }
    break;
    }
}
void FmodGodot::FmodConsole::_bind_methods()
{
}

void FmodConsole::_set_collapse(bool p_collapse)
{
    collapse = p_collapse;
    _rebuild_log();
}

void FmodConsole::_save_layout_to_config(const Ref<ConfigFile> &p_config, const String &p_section) const
{
    for (const KeyValue<MessageType, LogFilter *> &E : type_filter_map)
    {
        p_config->set_value(p_section, "_editor_log_filter_" + itos(E.key), E.value->is_active());
    }

    p_config->set_value(p_section, "_editor_log_collapse", collapse);
}

void FmodConsole::_load_layout_from_config(const Ref<ConfigFile> &p_config, const String &p_section)
{
    is_loading_state = true;

    for (const KeyValue<MessageType, LogFilter *> &E : type_filter_map)
    {
        E.value->set_active(p_config->get_value(p_section, "_editor_log_filter_" + itos(E.key), true));
    }
    collapse_button->set_pressed(p_config->get_value(p_section, "_editor_log_collapse", false));

    is_loading_state = false;
}
void FmodGodot::FmodConsole::_set_fmod_script_client(FmodScriptClient *p_client)
{
    client = p_client;
}

void FmodConsole::_meta_clicked(const String &p_meta)
{
    if (!p_meta.contains(":"))
    {
        return;
    }
    const PackedStringArray parts = p_meta.rsplit(":", true, 1);
    String path = parts[0];
    const int line = parts[1].to_int() - 1;

    if (path.begins_with("res://"))
    {
        if (ResourceLoader::get_singleton()->exists(path))
        {
            const Ref<Resource> res = ResourceLoader::get_singleton()->load(path);
            EditorInterface::get_singleton()->edit_script(res, line, 0);
            EditorInterface::get_singleton()->edit_resource(res);
        }
    }
    else
    {
        OS::get_singleton()->shell_open(p_meta);
    }
}

void FmodConsole::_clear_request()
{
    log->clear();
    messages.clear();
    _reset_message_counts();
    _set_dock_tab_icon(Ref<Texture2D>());
}

void FmodConsole::clear()
{
    _clear_request();
}

void FmodConsole::_process_message(const String &p_msg, MessageType p_type, bool p_clear)
{
    if (messages.size() > 0 && messages[messages.size() - 1].text == p_msg &&
        messages[messages.size() - 1].type == p_type)
    {
        // If previous message is the same as the new one, increase previous count rather than adding another
        // instance to the messages list.
        LogMessage &previous = messages.write[messages.size() - 1];
        previous.count++;

        _add_log_line(previous, collapse);
    }
    else
    {
        // Different message to the previous one received.
        LogMessage message(p_msg, p_type, p_clear);
        _add_log_line(message);
        messages.push_back(message);
    }

    type_filter_map[p_type]->set_message_count(type_filter_map[p_type]->get_message_count() + 1);
}

void FmodConsole::add_message(const String &p_msg, MessageType p_type)
{
    // Make text split by new lines their own message.
    // See #41321 for reasoning. At time of writing, multiple print()'s in running projects
    // get grouped together and sent to the editor log as one message. This can mess with the
    // search functionality (see the comments on the PR above for more details). This behavior
    // also matches that of other IDE's.
    PackedStringArray lines = p_msg.split("\n", true);
    int line_count = lines.size();

    for (int i = 0; i < line_count; i++)
    {
        _process_message(lines[i], p_type, i == line_count - 1);
    }
}

void FmodConsole::_set_dock_tab_icon(Ref<Texture2D> p_icon)
{
    set_dock_icon(p_icon);
    set_force_show_icon(p_icon.is_valid());
}
void FmodGodot::FmodConsole::_command_submitted(const String &command)
{
    add_message(command, MessageType::MSG_TYPE_INPUT);
    String text = client->get_script_output(command);
    if (text.begins_with("out():"))
    {
        add_message(text.substr(6), MessageType::MSG_TYPE_STD);
    }
    else if (text.begins_with("error():"))
    {
        add_message(text.substr(8), MessageType::MSG_TYPE_ERROR);
    }
    else if (text.begins_with("warning():"))
    {
        add_message(text.substr(10), MessageType::MSG_TYPE_WARNING);
    }
    else
    {
        add_message(text);
    }
}

void FmodConsole::_rebuild_log()
{
    if (messages.is_empty())
    {
        return;
    }

    log->clear();

    int line_count = 0;
    int start_message_index = 0;
    int initial_skip = 0;

    // Search backward for starting place.
    for (start_message_index = messages.size() - 1; start_message_index >= 0; start_message_index--)
    {
        LogMessage msg = messages[start_message_index];
        if (collapse)
        {
            if (_check_display_message(msg))
            {
                line_count++;
            }
        }
        else
        {
            // If not collapsing, log each instance on a line.
            for (int i = 0; i < msg.count; i++)
            {
                if (_check_display_message(msg))
                {
                    line_count++;
                }
            }
        }
        if (line_count >= line_limit)
        {
            initial_skip = line_count - line_limit;
            break;
        }
        if (start_message_index == 0)
        {
            break;
        }
    }

    for (int msg_idx = start_message_index; msg_idx < messages.size(); msg_idx++)
    {
        LogMessage msg = messages[msg_idx];

        if (collapse)
        {
            // If collapsing, only log one instance of the message.
            _add_log_line(msg);
        }
        else
        {
            // If not collapsing, log each instance on a line.
            for (int i = initial_skip; i < msg.count; i++)
            {
                initial_skip = 0;
                _add_log_line(msg);
            }
        }
    }
}

bool FmodConsole::_check_display_message(LogMessage &p_message)
{
    return type_filter_map[p_message.type]->is_active();
}

void FmodConsole::_add_log_line(LogMessage &p_message, bool p_replace_previous)
{
    if (!is_inside_tree())
    {
        // The log will be built all at once when it enters the tree and has its theme items.
        return;
    }

    // Only add the message to the log if it passes the filters.
    if (!_check_display_message(p_message))
    {
        return;
    }

    if (p_replace_previous)
    {
        // Remove last line if replacing, as it will be replace by the next added line.
        // Why "- 2"? RichTextLabel is weird. When you add a line with add_newline(), it also adds an element to the
        // list of lines which is null/blank, but it still counts as a line. So if you remove the last line (count - 1)
        // you are actually removing nothing...
        log->remove_paragraph(log->get_paragraph_count() - 2);
    }

    switch (p_message.type)
    {
    case MSG_TYPE_STD: {
    }
    break;
    case MSG_TYPE_STD_RICH: {
    }
    break;
    case MSG_TYPE_ERROR: {
        log->push_color(theme_cache.error_color);
        Ref<Texture2D> icon = theme_cache.error_icon;
        log->add_image(icon);
        log->push_bold();
        log->add_text(U" ERROR: ");
        log->pop(); // bold
    }
    break;
    case MSG_TYPE_WARNING: {
        log->push_color(theme_cache.warning_color);
        Ref<Texture2D> icon = theme_cache.warning_icon;
        log->add_image(icon);
        log->push_bold();
        log->add_text(U" WARNING: ");
        log->pop(); // bold
    }

    break;
    case MSG_TYPE_INPUT: {
        // Distinguish editor messages from messages printed by the project
        log->push_color(theme_cache.message_color);
    }
    break;
    }
    if (client->is_connected_to_studio())
    {
        _set_dock_tab_icon(theme_cache.connected_icon);
    }
    else
    {
        _set_dock_tab_icon(theme_cache.disconnected_icon);
    }
    // If collapsing, add the count of this message in bold at the start of the line.
    if (collapse && p_message.count > 1)
    {
        log->push_bold();
        log->add_text(vformat("(%s) ", itos(p_message.count)));
        log->pop();
    }

    // Note that errors and warnings only support BBCode in the file part of the message.
    if (p_message.type == MSG_TYPE_STD_RICH || p_message.type == MSG_TYPE_ERROR || p_message.type == MSG_TYPE_WARNING)
    {
        log->append_text(p_message.text);
    }
    else
    {
        log->add_text(p_message.text);
    }
    if (p_message.clear || p_message.type != MSG_TYPE_STD_RICH)
    {
        log->pop_all(); // Pop all unclosed tags.
    }
    log->newline();

    while (log->get_paragraph_count() > line_limit + 1)
    {
        log->remove_paragraph(0, true);
    }
}

void FmodConsole::_set_filter_active(bool p_active, MessageType p_message_type)
{
    type_filter_map[p_message_type]->set_active(p_active);
    _rebuild_log();
}

void FmodConsole::_reset_message_counts()
{
    for (const KeyValue<MessageType, LogFilter *> &E : type_filter_map)
    {
        E.value->set_message_count(0);
    }
}

FmodConsole::FmodConsole()
{
    set_name("FMOD Console");
    set_title("FMOD Console");
    // set_icon_name("Output");
    // set_dock_shortcut()
    set_default_slot(EditorDock::DOCK_SLOT_BOTTOM);
    set_available_layouts(EditorDock::DOCK_LAYOUT_HORIZONTAL | EditorDock::DOCK_LAYOUT_FLOATING);
    set_process_shortcut_input(true);
    set_shortcut_context(this);

    line_limit = int(EDITOR_GET("run/output/max_lines"));
    EditorInterface::get_singleton()->get_editor_settings()->connect(
        "settings_changed", callable_mp(this, &FmodConsole::_editor_settings_changed));

    HBoxContainer *hb = memnew(HBoxContainer);
    add_child(hb);

    VBoxContainer *vb_left = memnew(VBoxContainer);
    vb_left->set_custom_minimum_size(Size2(0, 90 * EDSCALE));
    vb_left->set_v_size_flags(SIZE_EXPAND_FILL);
    vb_left->set_h_size_flags(SIZE_EXPAND_FILL);
    hb->add_child(vb_left);

    // Log - Rich Text Label.
    log = memnew(RichTextLabel);
    log->set_threaded(true);
    log->set_use_bbcode(true);
    log->set_scroll_follow(true);
    log->set_selection_enabled(true);
    log->set_context_menu_enabled(true);
    log->set_focus_mode(FOCUS_CLICK);
    log->set_v_size_flags(SIZE_EXPAND_FILL);
    log->set_h_size_flags(SIZE_EXPAND_FILL);
    log->set_deselect_on_focus_loss_enabled(false);
    log->connect("meta_clicked", callable_mp(this, &FmodConsole::_meta_clicked));
    vb_left->add_child(log);

    HFlowContainer *bottom_hf = memnew(HFlowContainer);
    bottom_hf->set_alignment(FlowContainer::ALIGNMENT_END);
    vb_left->add_child(bottom_hf);

    // Clear.
    clear_button = memnew(Button);
    clear_button->set_accessibility_name(tr("Clear Log"));
    clear_button->set_theme_type_variation("BottomPanelButton");
    clear_button->set_focus_mode(FOCUS_ACCESSIBILITY);
    EditorInterface::get_singleton()->get_editor_settings()->get_shortcut("editor/clear_output");
    bottom_hf->add_child(clear_button);

    // Collapse.
    collapse_button = memnew(Button);
    collapse_button->set_theme_type_variation("BottomPanelButton");
    collapse_button->set_focus_mode(FOCUS_ACCESSIBILITY);
    collapse_button->set_tooltip_text(
        tr("Collapse duplicate messages into one log entry. Shows number of occurrences."));
    collapse_button->set_toggle_mode(true);
    collapse_button->set_pressed(false);
    collapse_button->connect(SceneStringName(toggled), callable_mp(this, &FmodConsole::_set_collapse));
    bottom_hf->add_child(collapse_button);

    // Search box
    input = memnew(CommandInput);
    input->set_h_size_flags(Control::SIZE_EXPAND_FILL);
    input->set_placeholder(tr("Enter Command"));
    input->set_accessibility_name(tr("Enter Command"));
    input->connect("command_submitted", callable_mp(this, &FmodConsole::_command_submitted));
    bottom_hf->add_child(input);

    HBoxContainer *hbox = memnew(HBoxContainer);
    bottom_hf->add_child(hbox);

    // Message Type Filters.
    LogFilter *std_filter = memnew(LogFilter(MSG_TYPE_STD));
    std_filter->initialize_button(tr("Standard Messages"), tr("Toggle visibility of standard output messages."),
                                  callable_mp(this, &FmodConsole::_set_filter_active));
    hbox->add_child(std_filter->toggle_button);
    type_filter_map.insert(MSG_TYPE_STD, std_filter);
    type_filter_map.insert(MSG_TYPE_STD_RICH, std_filter);

    LogFilter *error_filter = memnew(LogFilter(MSG_TYPE_ERROR));
    error_filter->initialize_button(tr("Errors"), tr("Toggle visibility of errors."),
                                    callable_mp(this, &FmodConsole::_set_filter_active));
    hbox->add_child(error_filter->toggle_button);
    type_filter_map.insert(MSG_TYPE_ERROR, error_filter);

    LogFilter *warning_filter = memnew(LogFilter(MSG_TYPE_WARNING));
    warning_filter->initialize_button(tr("Warnings"), tr("Toggle visibility of warnings."),
                                      callable_mp(this, &FmodConsole::_set_filter_active));
    hbox->add_child(warning_filter->toggle_button);
    type_filter_map.insert(MSG_TYPE_WARNING, warning_filter);

    LogFilter *input_filter = memnew(LogFilter(MSG_TYPE_INPUT));
    input_filter->initialize_button(tr("Editor Messages"), tr("Toggle visibility of sent commands."),
                                     callable_mp(this, &FmodConsole::_set_filter_active));
    hbox->add_child(input_filter->toggle_button);
    type_filter_map.insert(MSG_TYPE_INPUT, input_filter);
}

FmodConsole::~FmodConsole()
{
    for (const KeyValue<MessageType, LogFilter *> &E : type_filter_map)
    {
        // MSG_TYPE_STD_RICH is connected to the std_filter button, so we do this
        // to avoid it from being deleted twice, causing a crash on closing.
        if (E.key != MSG_TYPE_STD_RICH)
        {
            memdelete(E.value);
        }
    }
}
#undef EDSCALE
#undef EDITOR_GET
#undef ED_IS_SHORTCUT
#undef get_editor_theme_icon
#endif
