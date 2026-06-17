#pragma once
#ifdef TOOLS_ENABLED
#include "classes/button.hpp"
#include "classes/rich_text_label.hpp"
#include "variant/variant.hpp"
#include "variant/vector2.hpp"
#include <classes/input_event.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/h_flow_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodObjectDetails : public VBoxContainer
{
    GDCLASS(FmodObjectDetails, VBoxContainer);

  private:
    Button *header;
    RichTextLabel *text;
    Variant hovered_meta;
    bool hide_event_parameters = true;
    void _update_theme();
    void _meta_clicked(const Variant &p_meta);
    void _meta_hover_started(const Variant &p_meta);
    void _meta_hover_ended(const Variant &p_meta);

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    FmodObjectDetails(/* args */);
    ~FmodObjectDetails();
    void set_hide_event_parameter(bool hide);

    void display_fmod_object(const String &p_path);

    Variant _get_drag_data(const Vector2 &at_position) override;
};
} // namespace FmodGodot
#endif
