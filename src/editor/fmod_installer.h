#pragma once
#include "classes/button.hpp"
#include "classes/control.hpp"
#include "classes/dpi_texture.hpp"
#include "classes/http_request.hpp"
#include "classes/input_event.hpp"
#include "classes/label.hpp"
#include "classes/line_edit.hpp"
#include "classes/progress_bar.hpp"
#include "classes/scene_tree.hpp"
#include "classes/scene_tree_timer.hpp"
#include "classes/texture_rect.hpp"
#include "classes/window.hpp"
#include "classes/wrapped.hpp"
#include "core/class_db.hpp"
#include "fmod_common.h"
#include "globals.h"
#include "variant/callable.hpp"
#include "variant/variant.hpp"
#include <godot_cpp/core/gdvirtual.gen.inc>
using namespace godot;
class FmodInstaller : public Window
{
    GDCLASS(FmodInstaller, Window)
  private:
    LineEdit *username = nullptr;
    LineEdit *password = nullptr;
    Control *user_inputs = nullptr;
    Label *error = nullptr;
    Label *installInfo = nullptr;
    ProgressBar *progress_bar = nullptr;
    TextureRect *godot_icon = nullptr;
    HTTPRequest *client = nullptr;

    Button *show_password = nullptr;
    void _show_password();
    void _hide_password();

    void on_hide();
    void _timeout();

  public:
    enum InstallerState
    {
        INSTALLER_BUSY,
        INSTALLER_NONE,
    };

  private:
    InstallerState current_state = INSTALLER_NONE;

    String get_version_id()
    {
        const unsigned int major = (FMOD_VERSION & 0xffff0000) >> 16;
        const unsigned int minor = (FMOD_VERSION & 0x0000ff00) >> 8;
        const unsigned int patch = (FMOD_VERSION & 0x000000ff);
        // for some reason patch 13 is 0x13 instead of 0x0d so this fixes that
        // I expect similar case for minor if that reaches a double digit. am Ignoring major for now
        return vformat("%x%02x%02x", major, minor, patch);
    }

  protected:
    static void _bind_methods()
    {
        BIND_ENUM_CONSTANT(INSTALLER_BUSY);
        BIND_ENUM_CONSTANT(INSTALLER_NONE);
        GDVIRTUAL_BIND(_submit);
        BIND_METHOD(get_username);
        BIND_METHOD(clear_username);
        BIND_METHOD(get_password);
        BIND_METHOD(clear_password);
        BIND_METHOD(show_login);
        BIND_METHOD(show_progress);
        BIND_METHOD(fail, "message");
        BIND_METHOD(get_http);
        BIND_METHOD(set_progress_bar_value, "value");
        BIND_PROPERTY(install_message, Variant::STRING);
        BIND_PROPERTY(indeterminate_progress_bar, Variant::BOOL);
        BIND_PROPERTY(install_message_visible_characters, Variant::INT);
        BIND_METHOD(get_version_id);
        BIND_METHOD(on_hide);
    }
    void _notification(int p_what)
    {
        switch (p_what)
        {
        case NOTIFICATION_THEME_CHANGED: {
            if (show_password->is_pressed())
            {
                show_password->set_button_icon(get_theme_icon("GuiVisibilityVisible", "EditorIcons"));
            }
            else
            {
                show_password->set_button_icon(get_theme_icon("GuiVisibilityHidden", "EditorIcons"));
            }
        }
        break;
        case NOTIFICATION_READY: {
            Ref<DPITexture> icon = get_theme_icon("GodotMonochrome", "EditorIcons")->duplicate();
            icon->set_base_scale(10);
            godot_icon->set_texture(icon);
            _hide_password();
        }
        break;
        }
    }

    GDVIRTUAL0(_submit)
  public:
    virtual void submit()
    {
        GDVIRTUAL_CALL(_submit);
    }
    String get_username() const;
    String get_password() const;
    void set_indeterminate_progress_bar(bool p_indeterminate);
    bool get_indeterminate_progress_bar() const;
    void set_install_message_visible_characters(int visible_characters);

    int get_install_message_visible_characters() const;
    void clear_username();
    void clear_password();
    void show_login();
    void show_progress();
    void fail(const String &message);
    void set_progress_bar_value(float value);
    void set_install_message(const String &message);
    String get_install_message() const;
    HTTPRequest *get_http() const;
    FmodInstaller();
    Ref<SceneTreeTimer> timer = nullptr;
    void _unhandled_input(const Ref<InputEvent> &p_event) override;
};
VARIANT_ENUM_CAST(FmodInstaller::InstallerState)
