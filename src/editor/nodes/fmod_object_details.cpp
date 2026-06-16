#include "classes/control.hpp"
#include "classes/node.hpp"
#include "classes/panel.hpp"
#include "classes/panel_container.hpp"
#include "core/memory.hpp"
#include "fmod_globals.h"
#include "variant/variant.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_editor_interface.h"
#include "fmod_object_details.h"
#include "fmod_theme.h"
#include <classes/button.hpp>
#include <classes/display_server.hpp>
#include <classes/editor_interface.hpp>
#include <classes/h_box_container.hpp>
#include <classes/rich_text_label.hpp>
#include <classes/texture_rect.hpp>
#include <classes/theme.hpp>
namespace FmodGodot
{
namespace
{

void push_label(Control *control, const String &text, Ref<Texture2D> icon = nullptr)
{

    Label *label = memnew(Label());
    label->set_text(text);

    if (icon.is_valid())
    {
        HBoxContainer *box = memnew(HBoxContainer());
        TextureRect *rect = memnew(TextureRect());
        rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
        rect->set_texture(icon);
        box->add_child(rect, false, Node::INTERNAL_MODE_FRONT);
        box->add_child(label, false, Node::INTERNAL_MODE_FRONT);
        control->add_child(box, false, Node::INTERNAL_MODE_FRONT);
    }
    else
    {
        control->add_child(label, false, Node::INTERNAL_MODE_FRONT);
    }
}
void push_bg_label(Control *control, const String &text, Ref<Texture2D> icon = nullptr)
{

    Button *label = memnew(Button());
    label->set_text_alignment(godot::HORIZONTAL_ALIGNMENT_LEFT);
    label->add_theme_stylebox_override("disabled", control->get_theme_stylebox("prop_subsection_stylebox", "Editor"));
    label->add_theme_color_override("font_disabled_color", control->get_theme_color("font_color", "Button"));
    label->add_theme_color_override("icon_disabled_color", control->get_theme_color("icon_normal_color", "Button"));
    label->set_button_icon(icon);
    label->set_text(text);
    label->set_disabled(true);
    control->add_child(label, false, Node::INTERNAL_MODE_FRONT);
}

void push_copy_label(Control *control, const String &p_label, const String &p_copy_text, Ref<Texture2D> icon = nullptr)
{
    HBoxContainer *box = memnew(HBoxContainer);
    Label *label = memnew(Label());
    Button *button = memnew(Button());
    if (icon.is_valid())
    {
        TextureRect *rect = memnew(TextureRect());
        rect->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
        rect->set_texture(icon);
        box->add_child(rect, false, Node::INTERNAL_MODE_FRONT);
    }
    box->add_child(label);
    box->add_child(button);
    label->set_text(p_label + p_copy_text);
    button->set_button_icon(
        EditorInterface::get_singleton()->get_editor_theme()->get_icon("ActionCopy", "EditorIcons"));
    Callable copy = Callable(DisplayServer::get_singleton(), "clipboard_set");

    button->connect("pressed", copy.bind(p_copy_text));
    button->set_flat(true);
    control->add_child(box, false, Node::INTERNAL_MODE_FRONT);
}

} // namespace
void FmodObjectDetails::_update_theme()
{
    header->add_theme_stylebox_override("disabled", get_theme_stylebox("prop_subsection_stylebox", "Editor"));
    header->add_theme_color_override("font_disabled_color", get_theme_color("font_color", "Button"));
    header->add_theme_color_override("icon_disabled_color", get_theme_color("icon_normal_color", "Button"));
}
void FmodObjectDetails::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("display_fmod_object", "p_path"), &FmodObjectDetails::display_fmod_object);
}
void FmodObjectDetails::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_ENTER_TREE:
    case NOTIFICATION_THEME_CHANGED:
        _update_theme();
    }
}

FmodObjectDetails::FmodObjectDetails()
{
    header = memnew(Button);
    header->set_h_size_flags(SIZE_EXPAND_FILL);
    header->set_disabled(true);
    Panel *panel = memnew(Panel);
    PanelContainer *panel_container = memnew(PanelContainer);
    panel_container->set_v_size_flags(SIZE_EXPAND_FILL);
    // add_child(panel, false, INTERNAL_MODE_FRONT);
    // panel->add_child(box, false, INTERNAL_MODE_FRONT);
    header->set_expand_icon(true);
    // icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
    add_child(header, false, InternalMode::INTERNAL_MODE_FRONT);
    flowlayout = memnew(HFlowContainer());
    flowlayout->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    flowlayout->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    add_child(panel_container);
    panel_container->add_child(panel, false, INTERNAL_MODE_FRONT);
    panel_container->add_child(flowlayout, false, InternalMode::INTERNAL_MODE_FRONT);
}
FmodObjectDetails::~FmodObjectDetails()
{
}

void FmodObjectDetails::display_fmod_object(const String &p_path)
{
    for (int i = flowlayout->get_child_count(true) - 1; i >= 0; i--)
    {
        flowlayout->get_child(i, true)->queue_free();
        flowlayout->remove_child(flowlayout->get_child(i, true));
    }
    const FmodTheme *theme = FmodEditorInterface::get_singleton()->get_theme();
    const FmodEditorCache *cache = FmodEditorInterface::get_singleton()->get_cache();
    if (p_path.begins_with("event"))
    {

        header->set_text("Event");
        header->set_button_icon(theme->event_icon);
        Event event = cache->get_event(p_path);
        push_copy_label(flowlayout, "Full Path: ", event.full_path);
        push_label(flowlayout, vformat("Guid: %s", fmod_guid_to_string(event.guid)));
        if (event.is3d)
        {
            push_label(flowlayout, vformat("3D: %s", event.is3d));
            push_label(flowlayout, vformat("Doppler Enabled: %s", event.doppler_enabled));

            push_label(flowlayout, String("Min Distance: ") + rtos(event.min));
            push_label(flowlayout, String("Max Distance: ") + rtos(event.max));
        }
        int minutes = event.lengthMS / (60 * 1000);
        int seconds = (event.lengthMS / 1000) % 60;
        int milliseconds = event.lengthMS % 1000;

        push_label(flowlayout, vformat("Duration: %2d:%2d:%3d", minutes, seconds, milliseconds));
        push_label(flowlayout, vformat("One Shot: %s", event.is3d));
        push_label(flowlayout, vformat("Stream: %s", event.stream));

        VBoxContainer *vbox = memnew(VBoxContainer());
        flowlayout->add_child(vbox, false, INTERNAL_MODE_FRONT);
        for (auto param : event.parameters)
        {
            if (param.discrete)
            {
                push_bg_label(vbox, param.full_path, theme->d_parameter_icon);
            }
            else
            {
                push_bg_label(vbox, param.full_path, theme->c_parameter_icon);
            }

            push_label(vbox, vformat("\tGuid: %s", fmod_guid_to_string(param.guid)));
            push_label(vbox, vformat("\tRange [Min...Default..Max] [%d...%d...%d]", param.min_value,
                                     param.default_value, param.max_value));
        }

        // label->set_text("is3D: " + event.parameters);
    }
    else if (p_path.begins_with("bank"))
    {
        header->set_text("Bank:" + p_path.get_file().get_basename());
        header->set_button_icon(theme->bank_icon);
        Bank bank = cache->get_bank(p_path);
        VBoxContainer *vbox = memnew(VBoxContainer());
        push_copy_label(vbox, "Full Path: ", bank.full_path);
        push_label(vbox, "Guid: " + fmod_guid_to_string(bank.guid));
        for (auto event : bank.children)
        {
            if (event.begins_with("event:/"))
            {
                push_copy_label(vbox, "", event, theme->event_icon);
            }
            else
            {
                push_copy_label(vbox, "", event, theme->vca_icon);
            }
        }
        flowlayout->add_child(vbox, false, Node::INTERNAL_MODE_FRONT);
    }
    else if (p_path.begins_with("vca"))
    {
        header->set_text("VCA:" + p_path.get_file().get_basename());
        VCA vca = cache->get_vca(p_path);
        push_label(flowlayout, vformat("Full Path: ", p_path));
        push_label(flowlayout, vformat("Guid: %s", fmod_guid_to_string(vca.guid)));

        header->set_button_icon(theme->vca_icon);
    }
    else if (p_path.begins_with("param:"))
    {
        header->set_text("Global Parameter:" + p_path.get_file().get_basename());
        Parameter parameter = cache->get_parameter(p_path);
        if (parameter.discrete)
        {
            header->set_button_icon(theme->d_parameter_icon);
        }
        else
        {
            header->set_button_icon(theme->c_parameter_icon);
        }
        push_copy_label(flowlayout, "Name:", parameter.full_path.get_file());
        push_label(flowlayout, "Guid:" + fmod_guid_to_string(parameter.guid));
    }
}
} // namespace FmodGodot
#endif
