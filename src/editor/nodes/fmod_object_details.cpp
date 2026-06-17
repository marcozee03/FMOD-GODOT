#include "variant/vector2.hpp"
#ifdef TOOLS_ENABLED
#include "classes/control.hpp"
#include "classes/node.hpp"
#include "core/memory.hpp"
#include "fmod_editor_interface.h"
#include "fmod_globals.h"
#include "fmod_object_details.h"
#include "fmod_theme.h"
#include "variant/callable_method_pointer.hpp"
#include "variant/variant.hpp"
#include <classes/button.hpp>
#include <classes/display_server.hpp>
#include <classes/editor_interface.hpp>
#include <classes/h_box_container.hpp>
#include <classes/rich_text_label.hpp>
#include <classes/texture_rect.hpp>
#include <classes/theme.hpp>
using namespace godot;
namespace FmodGodot
{
namespace
{
void push_icon(RichTextLabel *rich_text, Ref<Texture2D> icon)
{
    if (icon.is_valid())
    {
        int font_size = rich_text->get_theme_font_size("font_size");
        rich_text->add_image(icon, font_size, font_size);
    }
}
void push_labeln(RichTextLabel *rich_text, const String &label, const String &text, Ref<Texture2D> icon = nullptr)
{
    push_icon(rich_text, icon);
    rich_text->push_bold();
    rich_text->add_text(label);
    rich_text->pop();
    rich_text->add_text(text);
    rich_text->newline();
}
void push_label(RichTextLabel *rich_text, const String &label, const String &text, Ref<Texture2D> icon = nullptr)
{
    push_icon(rich_text, icon);
    rich_text->push_bold();
    rich_text->add_text(label);
    rich_text->pop();
    rich_text->add_text(text);
}
void push_label_str_meta(RichTextLabel *rich_text, const String &label, const String &text,
                         const String &tooltip = "Copy", Ref<Texture2D> icon = nullptr)
{
    push_icon(rich_text, icon);
    rich_text->push_bold();
    rich_text->add_text(label);
    rich_text->pop();
    rich_text->push_meta(text, RichTextLabel::META_UNDERLINE_ON_HOVER, tooltip);
    rich_text->add_text(text);
    rich_text->pop();
    rich_text->newline();
}
void push_label_var_meta(RichTextLabel *rich_text, const String &label, const String &text, const Variant &meta,
                         const String &tooltip = "", Ref<Texture2D> icon = nullptr)
{
    push_icon(rich_text, icon);
    rich_text->push_bold();
    rich_text->add_text(label);
    rich_text->pop();
    rich_text->push_meta(meta, RichTextLabel::META_UNDERLINE_ON_HOVER, tooltip);
    rich_text->add_text(text);
    rich_text->pop();
    rich_text->newline();
}

} // namespace
void FmodObjectDetails::_update_theme()
{
    header->add_theme_stylebox_override("disabled", get_theme_stylebox("prop_subsection_stylebox", "Editor"));
    header->add_theme_color_override("font_disabled_color", get_theme_color("font_color", "Button"));
    header->add_theme_color_override("icon_disabled_color", get_theme_color("icon_normal_color", "Button"));
    text->add_theme_constant_override("line_separation", get_theme_constant("line_separation", "ItemList"));
    text->add_theme_stylebox_override("normal", get_theme_stylebox("panel", "EditorInspector"));
}
void FmodObjectDetails::_meta_clicked(const Variant &p_meta)
{
    if (p_meta.get_type() == Variant::STRING)
    {
        DisplayServer::get_singleton()->clipboard_set(p_meta);
    }
}
void FmodObjectDetails::_meta_hover_started(const Variant &p_meta)
{
    hovered_meta = p_meta;
}
void FmodObjectDetails::_meta_hover_ended(const Variant &p_meta)
{
    if (hovered_meta == p_meta)
    {
        hovered_meta = Variant();
    }
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
    // add_child(panel, false, INTERNAL_MODE_FRONT);
    // panel->add_child(box, false, INTERNAL_MODE_FRONT);
    header->set_expand_icon(true);
    // icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
    add_child(header, false, InternalMode::INTERNAL_MODE_FRONT);
    text = memnew(RichTextLabel());
    text->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    text->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    text->set_fit_content(true);
    text->set_autowrap_mode(TextServer::AUTOWRAP_OFF);
    text->set_mouse_filter(Control::MOUSE_FILTER_PASS);

    text->connect("meta_clicked", callable_mp(this, &FmodObjectDetails::_meta_clicked));
    text->connect("meta_hover_started", callable_mp(this, &FmodObjectDetails::_meta_hover_started));
    text->connect("meta_hover_ended", callable_mp(this, &FmodObjectDetails::_meta_hover_ended));
    add_child(text);
}
FmodObjectDetails::~FmodObjectDetails()
{
}
void FmodObjectDetails::set_hide_event_parameter(bool hide)
{
    hide_event_parameters = hide;
}

void FmodObjectDetails::display_fmod_object(const String &p_path)
{
    text->clear();
    const FmodTheme *theme = FmodEditorInterface::get_singleton()->get_theme();
    const FmodEditorCache *cache = FmodEditorInterface::get_singleton()->get_cache();
    if (p_path.begins_with("event"))
    {

        header->set_text("Event");
        header->set_button_icon(theme->event_icon);
        Event event = cache->get_event(p_path);
        push_label_str_meta(text, "Full Path: ", event.full_path, "Copy event path");
        push_label_var_meta(text, "Guid: ", fmod_guid_to_string(event.guid), event.guid, "Drag Event");
        if (event.is3d)
        {
            push_label(text, "3D: ", Variant(event.is3d));
            text->add_text("  ");
            push_labeln(text, "Doppler Enabled: ", Variant(event.doppler_enabled));

            push_label(text, "Min Distance: ", rtos(event.min));
            text->add_text("  ");
            push_labeln(text, "Max Distance: ", rtos(event.max));
        }
        int hours = event.lengthMS / (60 * 1000 * 60);
        int minutes = event.lengthMS / (60 * 1000) % 60;
        int seconds = (event.lengthMS / 1000) % 60;
        int milliseconds = event.lengthMS % 1000;

        push_label(text, "Duration:", vformat("%02d:%02d:%03d", minutes, seconds, milliseconds));
        text->add_text("  ");
        push_label(text, "One Shot:", Variant(event.is3d));
        text->add_text("  ");
        push_label(text, "Stream:", Variant(event.stream));
        if (!hide_event_parameters)
        {
            text->push_indent(1);
            for (auto param : event.parameters)
            {
                text->newline();
                if (param.discrete)
                {
                    push_label_var_meta(text, param.full_path, "", param.full_path, "Copy parameter path",
                                        theme->d_parameter_icon);
                }
                else
                {
                    push_label_var_meta(text, param.full_path, "", param.full_path, "Copy parameter path",
                                        theme->c_parameter_icon);
                }
                push_label_var_meta(text, "Guid: ", fmod_guid_to_string(param.guid), param.guid, "Drag parameter");
                push_label(text, "Initial Value: ", itos(param.default_value));
                text->add_text("\t");
                push_labeln(text, "Range: ", vformat("%d->%d", param.min_value, param.max_value));
            }
            text->pop();
        }
    }
    else if (p_path.begins_with("bank"))
    {
        header->set_text("Bank:" + p_path.get_file().get_basename());
        header->set_button_icon(theme->bank_icon);
        Bank bank = cache->get_bank(p_path);
        push_label_str_meta(text, "Full Path: ", bank.full_path, "Copy bank path");
        push_label_var_meta(text, "Guid: ", fmod_guid_to_string(bank.guid), bank.guid, "Drag Event");
        text->push_indent(1);
        for (auto event : bank.children)
        {
            if (event.begins_with("event:/"))
            {
                push_labeln(text, "", event, theme->event_icon);
            }
            else
            {
                push_labeln(text, "", event, theme->vca_icon);
            }
        }
        text->pop();
    }
    else if (p_path.begins_with("vca"))
    {
        header->set_text("VCA:" + p_path.get_file().get_basename());
        header->set_button_icon(theme->vca_icon);
        VCA vca = cache->get_vca(p_path);

        push_label_str_meta(text, "Full Path: ", vca.full_path, "Copy vca path");
        push_label_var_meta(text, "Guid: ", fmod_guid_to_string(vca.guid), vca.guid, "Drag Event");
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
        push_label_str_meta(text, "Full Path: ", parameter.full_path, "Copy vca path");
        push_labeln(text, "Name: ", parameter.full_path.get_file());
        push_label_var_meta(text, "Guid: ", fmod_guid_to_string(parameter.guid), parameter.guid, "Drag Event");
    }
}
Variant FmodObjectDetails::_get_drag_data(const Vector2 &at_position)
{
    if (hovered_meta != Variant())
    {
        HBoxContainer *box = memnew(HBoxContainer);
        Label *label = memnew(Label());
        switch (hovered_meta.get_type())
        {

        case Variant::STRING:
            label->set_text(hovered_meta);
            break;
        // case Variant::VECTOR2I:
        // break;
        case Variant::VECTOR4I:
            label->set_text(fmod_guid_to_string((Vector4i)hovered_meta));
            break;
        default:
            break;
        }
        box->add_child(label);
        set_drag_preview(box);
    }
    return hovered_meta;
}

} // namespace FmodGodot
#endif
