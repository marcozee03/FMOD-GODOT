#include "classes/control.hpp"
#include "classes/display_server.hpp"
#include "classes/popup_panel.hpp"
#include "fmod_editor_interface.h"
#include "fmod_project_explorer.h"
#include "variant/vector2i.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include "fmod_globals.h"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/godot.hpp>
using namespace godot;
namespace FmodGodot
{
void FmodEventSelector::_bind_methods()
{

    ClassDB::bind_method(D_METHOD("open_window"), &FmodEventSelector::open_window);
}

FmodEventSelector::FmodEventSelector()
{
    hbox = memnew(HBoxContainer);
    hbox->set_anchors_preset(LayoutPreset::PRESET_FULL_RECT);
    hbox->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    add_child(hbox);
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
    lineEdit = memnew(LineEdit);
    lineEdit->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
    open_explorer = memnew(Button);
    hbox->add_child(lineEdit);
    Ref<Texture2D> icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
    hbox->add_child(open_explorer);
    open_explorer->set_button_icon(icon);
    open_explorer->connect("pressed", Callable(this, "open_window"));
    window->hide();
    window->set_exclusive(true);

    window->add_child(explorer);
}

FmodEventSelector::~FmodEventSelector()
{
    // lineEdit->queue_free();
    // hbox->queue_free();
    // open_explorer->queue_free();
    // window->queue_free();
    // windowTree->queue_free();
}

void FmodEventSelector::_ready()
{
}

void FmodEventSelector::open_window()
{
    window->popup_centered(Vector2(800, 600));
}
void FmodEventSelector::on_fmod_object_selected(const String &p_path)
{
    print_line("activated");
    auto cache = FmodEditorInterface::get_singleton()->get_cache();
    // windowTree.get
    lineEdit->set_text(p_path);
    // TODO SEGFAULT MAYBE
    lineEdit->set_tooltip_text(FmodGodot::fmod_guid_to_string(cache->get_event(p_path).guid));
    lineEdit->emit_signal("text_submitted", p_path);
    window->hide();
}
LineEdit *FmodEventSelector::get_line_edit()
{
    return lineEdit;
}
} // namespace FmodGodot
#endif
