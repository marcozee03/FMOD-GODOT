#include "classes/control.hpp"
#include "classes/display_server.hpp"
#include "classes/popup_panel.hpp"
#include "core/object.hpp"
#include "core/property_info.hpp"
#include "fmod_editor_interface.h"
#include "fmod_project_explorer.h"
#include "variant/variant.hpp"
#include "variant/vector2i.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/godot.hpp>
using namespace godot;
namespace FmodGodot
{
void FmodEventSelector::_editing_toggled(bool p_toggled_on)
{
    if (!p_toggled_on && text_changed)
    {
        if (!get_text().is_empty())
        {
            set_path(get_text());
        }
    }
    text_changed = false;
}
void FmodEventSelector::_text_submitted(const String &p_new_text)
{
    if (!p_new_text.is_empty() && text_changed)
    {
        set_path(p_new_text);
    }
    text_changed = false;
}
void FmodEventSelector::_text_changed(const String &p_new_text)
{
    text_changed = true;
}

void FmodEventSelector::_bind_methods()
{

    ADD_SIGNAL(MethodInfo("fmod_guid_and_path_selected",
                          PropertyInfo(Variant::VECTOR4I, "guid", godot::PROPERTY_HINT_NONE, "FmodEvent"),
                          PropertyInfo(Variant::STRING, "path", PROPERTY_HINT_NONE, "FmodEvent")));
    BIND_METHOD(set_path, "p_path_or_guid");
}

FmodEventSelector::FmodEventSelector()
{
    set_anchors_preset(LayoutPreset::PRESET_FULL_RECT);
    set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    window = memnew(PopupPanel);
    window->set_initial_position(Window::WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
    add_child(window);
    explorer = memnew(FmodProjectExplorer);
    explorer->set_display_flags(EventTree::DisplayFlags::EVENTS);
    explorer->connect("fmod_object_activated", callable_mp(this, &FmodEventSelector::on_fmod_object_selected));
    explorer->set_anchors_preset(LayoutPreset::PRESET_FULL_RECT);
    explorer->set_v_size_flags(SIZE_EXPAND_FILL);
    explorer->set_h_size_flags(SIZE_EXPAND_FILL);
    window->connect("close_requested", Callable(window, "hide"));
    open_explorer = memnew(Button);
    Ref<Texture2D> icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
    add_child(open_explorer);
    open_explorer->set_anchors_and_offsets_preset(Control::LayoutPreset::PRESET_RIGHT_WIDE);
    open_explorer->set_button_icon(icon);
    open_explorer->connect("pressed", callable_mp(this, &FmodEventSelector::open_window));
    open_explorer->set_anchor_and_offset(SIDE_LEFT, 1.0, -8.0);
    open_explorer->set_anchor(SIDE_RIGHT, 1.0);
    open_explorer->set_anchor(SIDE_BOTTOM, 1.0);
    open_explorer->set_h_grow_direction(godot::Control::GROW_DIRECTION_BEGIN);
    open_explorer->set_v_grow_direction(godot::Control::GROW_DIRECTION_BOTH);
    open_explorer->set_icon_alignment(HORIZONTAL_ALIGNMENT_CENTER);

    window->hide();
    window->set_exclusive(true);

    window->add_child(explorer);
    set_drag_and_drop_selection_enabled(false);
    connect("editing_toggled", callable_mp(this, &FmodEventSelector::_editing_toggled));
    connect("text_submitted", callable_mp(this, &FmodEventSelector::_text_submitted));
}

FmodEventSelector::~FmodEventSelector()
{
    // lineEdit->queue_free();
    // hbox->queue_free();
    // open_explorer->queue_free();
    // window->queue_free();
    // windowTree->queue_free();
}

void FmodEventSelector::set_guid(const Vector4i &p_guid)
{
    clear();
    set_text(FmodEditorInterface::get_singleton()->get_cache()->get_event(p_guid).full_path);
    text_changed = false;
    set_tooltip_text(fmod_guid_to_string(p_guid));
    emit_signal("fmod_guid_and_path_selected", p_guid,
                FmodEditorInterface::get_singleton()->get_cache()->get_event(p_guid).full_path);
}

void FmodEventSelector::set_path(const String &p_path_or_guid)
{
    Vector4i guid;
    if (FMOD_OK == FMOD_Studio_ParseID(p_path_or_guid.utf8().ptr(), reinterpret_cast<FMOD_GUID *>(&guid)))
    {
        set_guid(guid);
        return;
    }
    guid = FmodEditorInterface::get_singleton()->get_cache()->lookup_id(p_path_or_guid);
    set_text(p_path_or_guid);
    text_changed = false;
    set_tooltip_text(fmod_guid_to_string(guid));
    emit_signal("fmod_guid_and_path_selected", guid, p_path_or_guid);
}
void FmodEventSelector::open_window()
{
    window->popup_centered(Vector2(800, 600));
}
void FmodEventSelector::on_fmod_object_selected(const String &p_path)
{
    set_path(p_path);
    window->hide();
}
bool FmodEventSelector::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const
{
    return p_data.get_type() == Variant::VECTOR4I || p_data.get_type() == Variant::STRING;
}
void FmodEventSelector::_drop_data(const Vector2 &p_at_position, const Variant &p_data)
{
    if (p_data.get_type() == Variant::STRING)
    {
        call_deferred("set_path", p_data);
    }
    if (p_data.get_type() == Variant::VECTOR4I)
    {
        set_guid(p_data);
    }
}

} // namespace FmodGodot
#endif
