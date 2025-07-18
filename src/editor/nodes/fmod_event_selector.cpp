#ifdef TOOLS_ENABLED
#include "fmod_event_selector.h"
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/memory.hpp>
#include "fmod_event_tree.h"
#include "fmod_globals.h"
#include "fmod_audio_server.h"
#include <godot_cpp/godot.hpp>
using namespace godot;
namespace FmodGodot
{
    void FmodEventPathSelector::_bind_methods()
    {

        ClassDB::bind_method(D_METHOD("open_window"), &FmodEventPathSelector::open_window);
    }

    FmodEventPathSelector::FmodEventPathSelector()
    {
        hbox = memnew(HBoxContainer);
        hbox->set_anchors_preset(LayoutPreset::PRESET_FULL_RECT);
        hbox->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        add_child(hbox);
        window = memnew(Window);
        window->set_initial_position(Window::WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_MAIN_WINDOW_SCREEN);
        add_child(window);
        windowTree = memnew(EventTree);
        windowTree->set_display_flags(EventTree::DisplayFlags::EVENTS);
        windowTree->set_select_mode(Tree::SelectMode::SELECT_SINGLE);
        windowTree->connect("fmod_object_activated", callable_mp(this, &FmodEventPathSelector::on_fmod_object_selected));
        windowTree->set_anchors_preset(LayoutPreset::PRESET_FULL_RECT);
        window->connect("close_requested", Callable(window, "hide"));
        window->add_child(windowTree);
        lineEdit = memnew(LineEdit);
        lineEdit->set_h_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        open_explorer = memnew(Button);
        hbox->add_child(lineEdit);
        Ref<Texture2D> icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
        hbox->add_child(open_explorer);
        open_explorer->set_button_icon(icon);
        open_explorer->connect("pressed", Callable(window, "show"));
        window->hide();
    }

    FmodEventPathSelector::~FmodEventPathSelector()
    {
        // lineEdit->queue_free();
        // hbox->queue_free();
        // open_explorer->queue_free();
        // window->queue_free();
        // windowTree->queue_free();
    }

    void FmodEventPathSelector::_ready()
    {
    }

    inline void FmodEventPathSelector::open_window()
    {
        window->show();
    }
    void FmodEventPathSelector::on_fmod_object_selected(const String &p_path)
    {
        auto cache = FmodEditorInterface::get_singleton()->get_cache();
        // windowTree.get
        lineEdit->set_text(p_path);
        // TODO SEGFAULT MAYBE
        lineEdit->set_tooltip_text(FmodGodot::fmod_guid_to_string(cache->get_event(p_path).guid));
        lineEdit->emit_signal("text_submitted", p_path);
        window->hide();
    }
    LineEdit *FmodEventPathSelector::get_line_edit()
    {
        return lineEdit;
    }
    EventTree *FmodEventPathSelector::get_event_tree()
    {
        return windowTree;
    }
}
#endif