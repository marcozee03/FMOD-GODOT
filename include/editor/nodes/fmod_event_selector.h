#include "fmod_project_explorer.h"
#ifdef TOOLS_ENABLED
#pragma once
#include <classes/button.hpp>
#include <classes/h_box_container.hpp>
#include <classes/line_edit.hpp>
#include <classes/window.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEventSelector : public Control
{
    GDCLASS(FmodEventSelector, Control)
  private:
    HBoxContainer *hbox;
    LineEdit *lineEdit;
    Button *open_explorer;
    Window *window;
    FmodProjectExplorer *explorer;

  protected:
    static void _bind_methods();

  public:
    FmodEventSelector();
    ~FmodEventSelector();
    void _ready() override;
    void open_window();
    void on_fmod_object_selected(const String &p_path);
    LineEdit *get_line_edit();
};
} // namespace FmodGodot
#endif
