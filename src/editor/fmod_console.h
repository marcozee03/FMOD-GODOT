#pragma once
#ifdef TOOLS_ENABLED
#include "fmod_script_client.h"

#include "classes/wrapped.hpp"
#include "command_input.h"
#include <classes/button.hpp>
#include <classes/config_file.hpp>
#include <classes/editor_dock.hpp>
#include <classes/line_edit.hpp>
#include <classes/rich_text_label.hpp>
#include <classes/thread.hpp>

using namespace godot;
namespace FmodGodot
{
class FmodConsole : public EditorDock
{
    GDCLASS(FmodConsole, EditorDock);

  public:
    enum MessageType
    {
        MSG_TYPE_STD,
        MSG_TYPE_ERROR,
        MSG_TYPE_STD_RICH,
        MSG_TYPE_WARNING,
        MSG_TYPE_INPUT,
    };

  private:
    struct LogMessage
    {
        String text;
        MessageType type;
        int count = 1;
        bool clear = true;

        LogMessage()
        {
        }

        LogMessage(const String &p_text, MessageType p_type, bool p_clear) : text(p_text), type(p_type), clear(p_clear)
        {
        }
    };

    struct
    {
        Color error_color;
        Ref<Texture2D> error_icon;

        Color warning_color;
        Ref<Texture2D> warning_icon;
        Ref<Texture2D> connected_icon;
        Ref<Texture2D> disconnected_icon;

        Color message_color;
    } theme_cache;

    // Encapsulates all data and functionality regarding filters.
    struct LogFilter
    {
      private:
        // Force usage of set method since it has functionality built-in.
        int message_count = 0;
        bool active = true;

      public:
        MessageType type;
        Button *toggle_button = nullptr;

        void initialize_button(const String &p_name, const String &p_tooltip, Callable p_toggled_callback)
        {
            toggle_button = memnew(Button);
            toggle_button->set_toggle_mode(true);
            toggle_button->set_pressed(true);
            toggle_button->set_text(itos(message_count));
            toggle_button->set_accessibility_name(toggle_button->tr(p_name));
            toggle_button->set_tooltip_text(toggle_button->tr(p_tooltip));
            toggle_button->set_focus_mode(FOCUS_ACCESSIBILITY);
            toggle_button->set_theme_type_variation("EditorLogFilterButton");
            // When toggled call the callback and pass the MessageType this button is for.
            toggle_button->connect("toggled", p_toggled_callback.bind(type));
        }

        int get_message_count()
        {
            return message_count;
        }

        void set_message_count(int p_count)
        {
            message_count = p_count;
            toggle_button->set_text(itos(message_count));
        }

        bool is_active()
        {
            return active;
        }

        void set_active(bool p_active)
        {
            toggle_button->set_pressed(p_active);
            active = p_active;
        }

        LogFilter(MessageType p_type) : type(p_type)
        {
        }
    };

    int line_limit = 10000;

    Vector<LogMessage> messages;
    // Maps MessageTypes to LogFilters for convenient access and storage (don't need 1 member per filter).
    HashMap<MessageType, LogFilter *> type_filter_map;

    RichTextLabel *log = nullptr;

    Button *clear_button = nullptr;

    Button *collapse_button = nullptr;
    bool collapse = false;

    CommandInput *input = nullptr;
    FmodScriptClient *client = nullptr;

    bool is_loading_state = false; // Used to disable saving requests while loading (some signals from buttons will try
                                   // to trigger a save, which happens during loading).
    // void _dragged(const Point2& p_ofs);
    void _meta_clicked(const String &p_meta);
    void _clear_request();

    void _rebuild_log();
    void _add_log_line(LogMessage &p_message, bool p_replace_previous = false);
    bool _check_display_message(LogMessage &p_message);

    void _set_filter_active(bool p_active, MessageType p_message_type);

    void _process_message(const String &p_msg, MessageType p_type, bool p_clear);
    void _reset_message_counts();
    void _set_dock_tab_icon(Ref<Texture2D> p_icon);
    void _command_submitted(const String &command);

    void _set_collapse(bool p_collapse);

    void _update_theme();
    void _editor_settings_changed();

  protected:
    void _notification(int p_what);
    static void _bind_methods();

  public:
    void add_message(const String &p_msg, MessageType p_type = MSG_TYPE_STD);
    void _save_layout_to_config(const Ref<ConfigFile> &p_config, const String &p_section) const override;
    void _load_layout_from_config(const Ref<ConfigFile> &p_config, const String &p_section) override;
    void _set_fmod_script_client(FmodScriptClient *p_client);

    void clear();

    FmodConsole();
    ~FmodConsole();
};
} // namespace FmodGodot
VARIANT_ENUM_CAST(FmodGodot::FmodConsole::MessageType)
#endif
