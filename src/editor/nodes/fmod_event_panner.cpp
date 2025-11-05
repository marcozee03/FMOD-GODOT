#include "classes/global_constants.hpp"
#include "fmod_string_names.h"
#ifdef TOOLS_ENABLED
#include "classes/control.hpp"
#include "classes/editor_interface.hpp"
#include "classes/input_event_mouse_button.hpp"
#include "classes/input_event_mouse_motion.hpp"
#include "classes/ref.hpp"
#include "classes/style_box.hpp"
#include "core/class_db.hpp"
#include "fmod_event_panner.h"
#include "globals.h"
#include "variant/variant.hpp"
#include "variant/vector2.hpp"
#include "variant/vector3.hpp"
#include <classes/font.hpp>
#include <classes/project_settings.hpp>
#include <classes/theme.hpp>
using namespace godot;
float FmodEventPanner::get_min_dimension() const
{
    return Math::min(get_rect().size.x, get_rect().size.y);
}

FmodEventPanner::FmodEventPanner()
{
    event_position = Vector3(0, 0, 0);
    dragging = false;
    set_focus_mode(FOCUS_CLICK);
}
FmodEventPanner::~FmodEventPanner()
{
}
Rect2 FmodEventPanner::get_local_square() const
{
    const float x_diff = get_rect().size.x - get_min_dimension();
    const float y_diff = get_rect().size.y - get_min_dimension();
    Rect2 base_rect = get_rect();
    base_rect.set_position(Vector2(0, 0));
    return base_rect.grow_individual(-x_diff / 2, -y_diff / 2, -x_diff / 2, -y_diff / 2);
}
Rect2 FmodEventPanner::get_square() const
{
    const float x_diff = get_rect().size.x - get_min_dimension();
    const float y_diff = get_rect().size.y - get_min_dimension();
    return get_rect().grow_individual(-x_diff / 2, -y_diff / 2, -x_diff / 2, -y_diff / 2);
}
Vector3 FmodEventPanner::get_play_position()
{
    return event_position / (get_min_dimension() / world_size);
}

void FmodEventPanner::set_view(View p_view)
{
    view = p_view;
    queue_redraw();
}
FmodEventPanner::View FmodEventPanner::get_view() const
{
    return view;
}

void FmodEventPanner::set_world_size(float p_size)
{
    world_size = p_size;
    queue_redraw();
}
float FmodEventPanner::get_world_size() const
{
    return world_size;
}

void FmodEventPanner::_bind_methods()
{
    BIND_PROPERTY(world_size, Variant::Type::FLOAT);
    BIND_ENUM_CONSTANT(FRONT);
    BIND_ENUM_CONSTANT(TOP);
    BIND_PROPERTY(view, Variant::Type::INT)
}

void FmodEventPanner::_draw()
{
    const float step = ProjectSettings::get_singleton()->get_setting(FmodGodot::DISTANCE_FACTOR);
    float grid_width = get_min_dimension() / world_size * step;
    Rect2 square = get_local_square();
    auto et = EditorInterface::get_singleton()->get_editor_theme();
    Ref<StyleBox> sb = et->get_theme_item(godot::Theme::DATA_TYPE_STYLEBOX, "panel", "Panel");
    draw_style_box(sb, get_local_square());
    for (int i = 0; i < Math::floor(world_size / step) / 2; i++)
    {

        const float y_begin = square.position.y;
        const float y_end = square.get_end().y;
        const float left_begin = square.get_center().x - grid_width / 2;
        const float right_begin = square.get_center().x + grid_width / 2;
        const float bottom_begin = square.get_center().y + grid_width / 2;
        const float top_begin = square.get_center().y - grid_width / 2;
        draw_line(Vector2(right_begin + grid_width * i, y_begin), Vector2(right_begin + grid_width * i, y_end),
                  Color(0.102, .102, .102, .75));
        draw_line(Vector2(left_begin - grid_width * i, y_begin), Vector2(left_begin - grid_width * i, y_end),
                  Color(.102, .102, .102, .75f));

        draw_line(Vector2(square.position.x, top_begin - grid_width * i),
                  Vector2(square.get_end().x, top_begin - grid_width * i), Color(.102, .102, .102, .75));
        draw_line(Vector2(square.position.x, bottom_begin + grid_width * i),
                  Vector2(square.get_end().x, bottom_begin + grid_width * i), Color(.102, .102, .102, .75));
    }
    switch (view)
    {
    case FRONT:
        draw_circle(Vector2(event_position.x, event_position.y) + square.get_center(), 4, Color(1, 1, 1));
        break;
    case TOP:
        draw_circle(Vector2(event_position.x, event_position.z) + square.get_center(), 4, Color(1, 1, 1));
    }
    draw_rect(square, Color(.105, .105, .105, 1), false, 2, false);
    draw_string(get_theme_default_font(),
                square.get_center() + Vector2(square.get_size().x / 2 - get_theme_default_font_size(),
                                              get_theme_default_font_size() / 2.0),
                "+X", HORIZONTAL_ALIGNMENT_LEFT, -1, get_theme_default_font_size());

    if (view == FRONT)
    {
        draw_string(get_theme_default_font(), square.get_position() + Vector2(0, square.get_size().y), "+Y",
                    HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER, square.get_size().x,
                    get_theme_default_font_size());
    }
    else
    {
        draw_string(get_theme_default_font(), square.get_position() + Vector2(0, square.get_size().y), "+Z",
                    HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER, square.get_size().x,
                    get_theme_default_font_size());
    }
}

void FmodEventPanner::_gui_input(const Ref<InputEvent> &p_event)
{
    Ref<InputEventMouseButton> mb = p_event;
    Ref<InputEventMouseMotion> mm = p_event;
    if (mb.is_valid() && mb->get_button_index() == godot::MOUSE_BUTTON_LEFT)
    {
        if (mb->is_pressed())
        {
            dragging = true;
            Rect2 square = get_local_square();
            if (square.has_point(get_local_mouse_position()))
            {
                Vector2 position = get_local_mouse_position() - square.get_center();
                switch (view)
                {
                case FRONT:
                    event_position = Vector3(position.x, position.y, event_position.z);
                    queue_redraw();
                    break;
                case TOP:
                    event_position = Vector3(position.x, event_position.y, position.y);
                    queue_redraw();
                    break;
                }
            }
        }
        else
        {
            dragging = false;
        }
    }

    if (mm.is_valid() && dragging)
    {
        dragging = true;
        Rect2 square = get_local_square();
        if (square.has_point(get_local_mouse_position()))
        {
            Vector2 position = get_local_mouse_position() - square.get_center();
            switch (view)
            {
            case FRONT:
                event_position = Vector3(position.x, position.y, event_position.z);
                queue_redraw();
                break;
            case TOP:
                event_position = Vector3(position.x, event_position.y, position.y);
                queue_redraw();
                break;
            }
        }
    }
}
#endif
