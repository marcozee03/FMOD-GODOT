#include "fmod_audio_server.h"
#include "fmod_object_details.h"
#include <classes/editor_interface.hpp>
#include <classes/theme.hpp>
#include <classes/rich_text_label.hpp>
#include <classes/button.hpp>
#include <classes/h_box_container.hpp>
#include <classes/display_server.hpp>
#include <classes/texture_rect.hpp>
#include "fmod_theme.h"
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
            button->set_button_icon(EditorInterface::get_singleton()->get_editor_theme()->get_icon("ActionCopy", "EditorIcons"));
            Callable copy = Callable(DisplayServer::get_singleton(), "clipboard_set");

            button->connect("pressed", copy.bind(p_copy_text));
            control->add_child(box, false, Node::INTERNAL_MODE_FRONT);
        }

    } // namespace
    void FmodObjectDetails::_bind_methods()
    {
    }
    FmodObjectDetails::FmodObjectDetails()
    {
        header = memnew(Label());
        box = memnew(HBoxContainer());
        icon = memnew(TextureRect());
        icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
        box->add_child(icon, false, INTERNAL_MODE_FRONT);
        box->add_child(header, false, INTERNAL_MODE_FRONT);
        add_child(box, false, InternalMode::INTERNAL_MODE_FRONT);
        flowlayout = memnew(HFlowContainer());
        set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        flowlayout->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        flowlayout->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        add_child(flowlayout, false, InternalMode::INTERNAL_MODE_FRONT);
    }
    FmodObjectDetails::~FmodObjectDetails()
    {
    }
    void FmodObjectDetails::display_fmod_object(const String &p_path)
    {
        for (int i = flowlayout->get_child_count(true) - 1; i >= 0; i--)
        {
            get_child(i)->queue_free();
        }
        const FmodTheme *theme = FmodEditorInterface::get_singleton()->get_theme();
        Label *label = memnew(Label());
        flowlayout->add_child(label);
        const FmodEditorCache *cache = FmodEditorInterface::get_singleton()->get_cache();
        if (p_path.begins_with("event"))
        {
            header->set_text("Event");
            icon->set_texture(theme->event_icon);
            Event event = cache->get_event(p_path);
            push_copy_label(flowlayout, "full path: ", event.full_path);
            push_label(flowlayout, "guid: " + fmod_guid_to_string(event.guid));
            if (event.is3d)
            {
                push_label(flowlayout, "3D: " + event.is3d);
                push_label(flowlayout, "Doppler Enabled: " + event.doppler_enabled);
                int minutes = event.lengthMS / (60 * 1000);
                int seconds = (event.lengthMS / 1000) % 60;
                int milliseconds = event.lengthMS % 1000;
                push_label(flowlayout, "Duration: " + String(":").join({itos(minutes).pad_zeros(2), itos(seconds).pad_zeros(2), itos(milliseconds).pad_zeros(3)}));
                push_label(flowlayout, "Min Distance: " + rtos(event.min));
                push_label(flowlayout, "Max Distance: " + rtos(event.max));
            }
            push_label(flowlayout, "One Shot: " + event.one_shot);
            push_label(flowlayout, "Stream: " + event.stream);
            // label->set_text("is3D: " + event.parameters);
        }
        else if (p_path.begins_with("bank"))
        {
            header->set_text("Bank:" + p_path.get_file().get_basename());
            icon->set_texture(theme->bank_icon);
            Bank bank = cache->get_bank(p_path);
            VBoxContainer *vbox = memnew(VBoxContainer());
            push_copy_label(vbox, "Full Path: ", bank.full_path);
            push_label(vbox, "Guid: " + fmod_guid_to_string(bank.guid));
            for (auto event : bank.events)
            {
                push_copy_label(vbox, "", event, theme->event_icon);
            }
            flowlayout->add_child(vbox, false, Node::INTERNAL_MODE_FRONT);
        }
        else if (p_path.begins_with("vca"))
        {
        }
        else if (p_path.begins_with("param:"))
        {
        }
    }
}