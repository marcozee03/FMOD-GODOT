#pragma once
#include "fmod_project_explorer.h"
#include "variant/string.hpp"
#include "variant/variant.hpp"
#include <binding/conversions.h>
#include <classes/button.hpp>
#include <classes/h_box_container.hpp>
#include <classes/line_edit.hpp>
#include <classes/window.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodEventSelector : public LineEdit
{
    GDCLASS(FmodEventSelector, LineEdit)
  private:
    Button *open_explorer;
    Window *window;
    FmodProjectExplorer *explorer;
    bool text_changed = false;

    void _editing_toggled(bool p_toggled_on);
    void _text_submitted(const String &p_new_text);
    void _text_changed(const String &p_new_text);

  protected:
    static void _bind_methods();

  public:
    FmodEventSelector();
    ~FmodEventSelector();
    void set_guid(const Vector4i &p_guid);
    void set_path(const String &p_path_or_guid);
    void open_window();
    void on_fmod_object_selected(const String &p_path);
    bool _can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const override;
    void _drop_data(const Vector2 &p_at_position, const Variant &p_data) override;
};
} // namespace FmodGodot
