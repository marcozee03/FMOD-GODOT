#include "fmod_installer.h"
#include "classes/global_constants.hpp"
#include "classes/h_box_container.hpp"
#include "classes/resource_loader.hpp"
#include "classes/v_box_container.hpp"
#include "classes/object.hpp"
#include "classes/panel.hpp"
using namespace godot;
void FmodInstaller::_show_password()
{
    password->set_secret(false);
    show_password->set_button_icon(get_theme_icon("GuiVisibilityVisible", "EditorIcons"));
}
void FmodInstaller::_hide_password()
{
    password->set_secret(true);
    show_password->set_button_icon(get_theme_icon("GuiVisibilityHidden", "EditorIcons"));
}
godot::String FmodInstaller::get_username() const
{
    return username->get_text();
}
godot::String FmodInstaller::get_password() const
{
    return password->get_text();
}
void FmodInstaller::set_indeterminate_progress_bar(bool p_indeterminate)
{
    progress_bar->set_indeterminate(p_indeterminate);
}
bool FmodInstaller::get_indeterminate_progress_bar() const
{
    return progress_bar->is_indeterminate();
}

void FmodInstaller::set_install_message_visible_characters(int p_visible_characters)
{
    installInfo->set_visible_characters(p_visible_characters);
}
int FmodInstaller::get_install_message_visible_characters() const
{
    return installInfo->get_visible_characters();
}
void FmodInstaller::clear_username()
{
    username->clear();
}
void FmodInstaller::clear_password()
{
    password->clear();
}
void FmodInstaller::show_login()
{
    current_state = INSTALLER_NONE;
    user_inputs->show();
    progress_bar->hide();
    installInfo->hide();
}
void FmodInstaller::show_progress()
{
    current_state = INSTALLER_BUSY;
    user_inputs->hide();
    progress_bar->show();
    installInfo->show();
}
void FmodInstaller::fail(const String &p_message)
{
    show_login();
    clear_password();
    error->set_text(p_message);
}
void FmodInstaller::set_progress_bar_value(float p_value)
{
    progress_bar->set_value(p_value);
}
void FmodInstaller::set_install_message(const String &p_message)
{
    installInfo->set_text(p_message);
    set_install_message_visible_characters(p_message.length());
}
godot::String FmodInstaller::get_install_message() const
{
    return installInfo->get_text();
}

godot::HTTPRequest *FmodInstaller::get_http() const
{
    return client;
}
FmodInstaller::FmodInstaller()
{
    client = memnew(HTTPRequest);
    add_child(client);
    {
        // window settings
        set_disable_3d(true);
        set_title("Fmod Godot Installer");
        set_initial_position(WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
        set_size(Vector2i(500, 300));
        set_wrap_controls(true);
        set_transient(true);
        set_flag(FLAG_BORDERLESS, true);
        set_flag(FLAG_RESIZE_DISABLED, true);
        set_flag(FLAG_POPUP_WM_HINT, true);
        set_flag(FLAG_POPUP, true);
        set_use_oversampling(true);
        set_oversampling_override(1);
        set_flag(FLAG_MINIMIZE_DISABLED, true);
        set_flag(FLAG_MAXIMIZE_DISABLED, true);
        connect("close_requested", callable_mp(this, &FmodInstaller::on_hide));
    }
    Panel *bg = memnew(Panel);
    bg->set_theme_type_variation("PanelForeground");
    bg->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_FULL_RECT);
    add_child(bg);
    // Fmod Godot Logo
    {
        HBoxContainer *title = memnew(HBoxContainer);
        title->set_anchor(godot::SIDE_LEFT, 0.02);
        title->set_anchor(godot::SIDE_TOP, 0.02);
        title->set_anchor_and_offset(godot::SIDE_RIGHT, 0.02, 132);
        title->set_anchor_and_offset(godot::SIDE_BOTTOM, 0.02, 62);

        godot_icon = memnew(TextureRect);
        godot_icon->set_expand_mode(godot::TextureRect::EXPAND_FIT_WIDTH);
        godot_icon->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT);
        bg->add_child(title);
        title->add_child(godot_icon);

        TextureRect *fmod_icon = memnew(TextureRect);
        fmod_icon->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT);
        fmod_icon->set_texture(ResourceLoader::get_singleton()->load("res://addons/FmodGodot/fmod-logo.svg"));
        title->add_child(fmod_icon);
        fmod_icon->set_v_size_flags(Control::SIZE_SHRINK_CENTER);

        Label *installer_subtitle = memnew(Label);
        installer_subtitle->set_text("Installer");
        installer_subtitle->set_anchors_and_offsets_preset(godot::Control::PRESET_CENTER_TOP);
        installer_subtitle->set_anchor(SIDE_TOP, 1.0);
        installer_subtitle->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);
        installer_subtitle->set_vertical_alignment(VERTICAL_ALIGNMENT_CENTER);

        fmod_icon->add_child(installer_subtitle);
    }
    Control *container = memnew(Control);
    container->set_anchor(SIDE_TOP, 0.221);
    container->set_anchor(SIDE_RIGHT, 1.0);
    container->set_anchor(SIDE_BOTTOM, 1.0);
    bg->add_child(container);

    {
        // User Inputs
        user_inputs = memnew(VBoxContainer);
        user_inputs->set_anchors_and_offsets_preset(Control::PRESET_CENTER);

        user_inputs->set_anchor_and_offset(Side::SIDE_LEFT, 0.5, -110, false);
        user_inputs->set_anchor_and_offset(Side::SIDE_RIGHT, 0.5, 110, false);
        user_inputs->set_anchor_and_offset(Side::SIDE_BOTTOM, 0.5, 64, false);
        user_inputs->set_anchor_and_offset(Side::SIDE_TOP, 0.5, -64, false);
        user_inputs->set_h_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        user_inputs->set_v_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        container->add_child(user_inputs);
        username = memnew(LineEdit);
        username->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        username->set_placeholder("Enter Username");

        user_inputs->add_child(username);
        Label *username_label = memnew(Label);
        username_label->set_anchor_and_offset(Side::SIDE_BOTTOM, 0.5, 11.5, false);
        username_label->set_anchor_and_offset(Side::SIDE_TOP, 0.5, -11.5, false);
        username_label->set_h_grow_direction(Control::GROW_DIRECTION_BEGIN);
        username_label->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);
        username_label->set_text("Username: ");
        username->add_child(username_label);
        password = memnew(LineEdit);
        password->set_h_size_flags(Control::SIZE_EXPAND_FILL);
        password->set_placeholder("Enter Password");
        password->set_secret(true);
        user_inputs->add_child(password);

        Label *password_label = memnew(Label);
        password_label->set_anchor_and_offset(Side::SIDE_BOTTOM, 0.5, 11.5, false);
        password_label->set_anchor_and_offset(Side::SIDE_TOP, 0.5, -11.5, false);
        password_label->set_h_grow_direction(Control::GROW_DIRECTION_BEGIN);
        password_label->set_v_grow_direction(Control::GROW_DIRECTION_BOTH);
        password_label->set_text(" Password: ");
        password->add_child(password_label);

        show_password = memnew(Button);
        show_password->set_anchors_preset(godot::Control::PRESET_RIGHT_WIDE);
        show_password->set_anchor_and_offset(SIDE_LEFT, 1.0, -8.0);
        show_password->set_anchor(SIDE_RIGHT, 1.0);
        show_password->set_anchor(SIDE_BOTTOM, 1.0);
        show_password->set_h_grow_direction(godot::Control::GROW_DIRECTION_BEGIN);
        show_password->set_v_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        show_password->set_flat(true);
        show_password->set_icon_alignment(HORIZONTAL_ALIGNMENT_CENTER);
        show_password->connect("button_down", callable_mp(this, &FmodInstaller::_show_password));
        show_password->connect("button_up", callable_mp(this, &FmodInstaller::_hide_password));
        password->add_child(show_password);

        Label *error_par = memnew(Label);
        error = memnew(Label);
        user_inputs->add_child(error_par);
        error_par->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
        error->set_anchors_and_offsets_preset(godot::Control::PRESET_FULL_RECT);
        error->set_h_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        error->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
        error_par->add_child(error);
        Button *submit = memnew(Button);
        submit->set_h_size_flags(Control::SIZE_SHRINK_CENTER);
        submit->set_text("Submit");
        submit->connect("pressed", callable_mp(this, &FmodInstaller::submit));
        user_inputs->add_child(submit);
        installInfo = memnew(Label);
        installInfo->set_anchors_and_offsets_preset(godot::Control::PRESET_CENTER);
        installInfo->set_h_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        installInfo->set_v_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        installInfo->set_visible_characters_behavior(TextServer::VisibleCharactersBehavior::VC_CHARS_AFTER_SHAPING);
        container->add_child(installInfo);
        progress_bar = memnew(ProgressBar);
        progress_bar->set_anchor(SIDE_LEFT, .05);
        progress_bar->set_anchor(SIDE_TOP, .8335948);
        progress_bar->set_anchor(SIDE_BOTTOM, .88113916);
        progress_bar->set_anchor(SIDE_RIGHT, .96000005);
        progress_bar->set_h_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
        progress_bar->set_v_grow_direction(godot::Control::GROW_DIRECTION_BEGIN);
        progress_bar->set_max(1.0);
        progress_bar->set_step(0.0);
        progress_bar->set_show_percentage(false);
        container->add_child(progress_bar);
        progress_bar->hide();
    }
    set_script(ResourceLoader::get_singleton()->load("res://addons/FmodGodot/fmod_installer.gd"));
}
void FmodInstaller::on_hide()
{
    if (current_state == INSTALLER_NONE)
    {
        hide();
        if (timer.is_valid() && timer->get_time_left() > 0)
        {
            timer->set_time_left(10);
        }
        else
        {
            timer = get_tree()->create_timer(10)->connect("timeout", callable_mp(this, &FmodInstaller::_timeout),
                                                          CONNECT_ONE_SHOT);
        }
    }
}
void FmodInstaller::_timeout()
{
    if (is_visible())
    {
        timer = nullptr;
        return;
    }
    else
    {
        queue_free();
    }
}
void FmodInstaller::_unhandled_input(const Ref<InputEvent> &p_event)
{
    if (p_event->is_pressed() && p_event->is_action("ui_cancel"))
    {
        on_hide();
    }
}
