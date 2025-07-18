#ifdef TOOLS_ENABLED
#pragma once
#include <classes/button.hpp>
#include <classes/line_edit.hpp>
#include <classes/button.hpp>
#include <classes/window.hpp>
#include <classes/h_box_container.hpp>
#include "fmod_event_tree.h"
using namespace godot;
namespace FmodGodot
{
    class FmodEventPathSelector : public Control
    {
        GDCLASS(FmodEventPathSelector, Control)
    private:
        HBoxContainer *hbox;
        LineEdit *lineEdit;
        Button *open_explorer;
        Window *window;
        EventTree *windowTree;

    protected:
        static void _bind_methods();

    public:
        FmodEventPathSelector();
        ~FmodEventPathSelector();
        void _ready() override;
        void open_window();
        void on_fmod_object_selected(const String& p_path);
        LineEdit *get_line_edit();
        EventTree *get_event_tree();
    };
}
#endif