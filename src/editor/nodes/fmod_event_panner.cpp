#include "classes/rendering_server.hpp"
#include "core/math.hpp"
#include "core/math_defs.hpp"
#ifdef TOOLS_ENABLED
#include "classes/control.hpp"
#include "classes/global_constants.hpp"
#include "classes/input_event_mouse_button.hpp"
#include "classes/input_event_mouse_motion.hpp"
#include "classes/ref.hpp"
#include "core/class_db.hpp"
#include "core/object.hpp"
#include "fmod_event_panner.h"
#include "fmod_string_names.h"
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
void FmodEventPanner::_update_theme()
{
    theme_cache.background_color = get_theme_color("dark_color_1", "Editor");
    theme_cache.grid_color = get_theme_color("dark_color_2", "Editor");
    theme_cache.contrast_color = get_theme_color("contrast_color_1", "Editor");
    theme_cache.axis_x_color = get_theme_color("axis_x_color", "Editor");
    theme_cache.axis_y_color = get_theme_color("axis_y_color", "Editor");
    theme_cache.axis_z_color = get_theme_color("axis_z_color", "Editor");
}
void FmodEventPanner::_draw_grid()
{
    const float step = ProjectSettings::get_singleton()->get_setting(FmodGodot::DISTANCE_FACTOR);
    float grid_width = get_min_dimension() / world_size * step;
    // float grid_width = get_min_dimension() / 10;
    Rect2 square = get_local_square();
    draw_rect(square, theme_cache.background_color, true, -1, true);
    RenderingServer::get_singleton()->canvas_item_set_custom_rect(get_canvas_item(), true, square);
    RenderingServer::get_singleton()->canvas_item_set_clip(get_canvas_item(), true);

    int iterations = Math::floor(world_size / step) / 2;
    if (world_size / step > 10)
    {
        grid_width = get_min_dimension() / 10.0;
        iterations = 5;
    }
    for (int i = 0; i < iterations; i++)
    // for (int i = 0; i < 5; i++)
    {

        const float y_begin = square.position.y;
        const float y_end = square.get_end().y;
        const float left_begin = square.get_center().x - grid_width / 2;
        const float right_begin = square.get_center().x + grid_width / 2;
        const float bottom_begin = square.get_center().y + grid_width / 2;
        const float top_begin = square.get_center().y - grid_width / 2;
        draw_line(Vector2(right_begin + grid_width * i, y_begin), Vector2(right_begin + grid_width * i, y_end),
                  theme_cache.grid_color);
        draw_line(Vector2(left_begin - grid_width * i, y_begin), Vector2(left_begin - grid_width * i, y_end),
                  theme_cache.grid_color);

        draw_line(Vector2(square.position.x, top_begin - grid_width * i),
                  Vector2(square.get_end().x, top_begin - grid_width * i), theme_cache.grid_color);
        draw_line(Vector2(square.position.x, bottom_begin + grid_width * i),
                  Vector2(square.get_end().x, bottom_begin + grid_width * i), theme_cache.grid_color);
    }

    draw_rect(square, theme_cache.background_color, false, 2, false);
}
void FmodEventPanner::_draw_event()

{
    const Rect2 square = get_local_square();
    switch (view)
    {
    case FRONT: {
        const Vector2 draw_center = Vector2(event_position.x, -event_position.y) + square.get_center();
        draw_circle(draw_center, get_min_dimension() / 18, theme_cache.contrast_color, false, 2.0, true);
        draw_circle(draw_center, get_min_dimension() / world_size * attenuation_min / 2,
                    Color(theme_cache.contrast_color, 0.5), true, -1.0, true);
    }
    break;
    case TOP:
        const Vector2 draw_center = Vector2(event_position.x, event_position.z) + square.get_center();
        draw_circle(draw_center, get_min_dimension() / 18, theme_cache.contrast_color, false, 2.0, true);
        draw_circle(draw_center, get_min_dimension() / world_size * attenuation_min / 2,
                    Color(theme_cache.contrast_color, 0.5), true, -1.0, true);

        draw_line(draw_center - Vector2::from_angle(rotation + Math_PI / 2.0) * (get_min_dimension() / 18),
                  draw_center - Vector2::from_angle(rotation + Math_PI / 2.0) * (get_min_dimension() / 9),
                  theme_cache.contrast_color, 2.0, true);
    }
}
void FmodEventPanner::_draw_panning()
{
    auto square = get_local_square();
    const float scale = attenuation_max / world_size;
    const float normalized_distance =
        Math::clamp(event_position.length() / (get_local_square().size.x * scale), 0.0f, 1.0f);
    float c = pow(1 - normalized_distance, 2);
    float arc_length = 2 * Math_PI * c;
    switch (view)
    {
    case TOP:
        draw_arc(square.get_center(), get_min_dimension() / 2,
                 Vector2(event_position.x, event_position.z).angle() - (arc_length / 2),
                 Vector2(event_position.x, event_position.z).angle() - (arc_length / 2) + arc_length,
                 get_min_dimension(), theme_cache.contrast_color, 2.0, true);
        break;
    case FRONT:
        draw_arc(square.get_center(), get_min_dimension() / 2,
                 Vector2(event_position.x, -event_position.y).angle() - (arc_length / 2),
                 Vector2(event_position.x, -event_position.y).angle() - (arc_length / 2) + arc_length,
                 get_min_dimension(), theme_cache.contrast_color, 2.0, true);
        break;
    }
}

Vector3 FmodEventPanner::get_play_position() const
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

float FmodEventPanner::get_facing_angle() const
{
    return rotation;
};
void FmodEventPanner::set_world_size(float p_size)
{
    event_position *= world_size / p_size;
    world_size = p_size;
    queue_redraw();
}
float FmodEventPanner::get_world_size() const
{
    return world_size;
}
void FmodEventPanner::set_attenuation_min(float p_radius)
{
    attenuation_min = p_radius;
}
void FmodEventPanner::set_attenuation_max(float p_radius)
{
    attenuation_max = p_radius;
};

void FmodEventPanner::_bind_methods()
{
    BIND_PROPERTY(world_size, Variant::Type::FLOAT);
    BIND_ENUM_CONSTANT(FRONT);
    BIND_ENUM_CONSTANT(TOP);
    BIND_PROPERTY(view, Variant::Type::INT)
    ADD_SIGNAL(MethodInfo("play_position_changed", PropertyInfo(Variant::VECTOR3, "play_position")));
    ADD_SIGNAL(MethodInfo("event_rotation_changed", PropertyInfo(Variant::FLOAT, "event_rotation")));
}
void FmodEventPanner::_notification(int p_what)
{
    switch (p_what)
    {
    case NOTIFICATION_THEME_CHANGED:
    case NOTIFICATION_ENTER_TREE:
        _update_theme();
    }
}

void FmodEventPanner::_draw()
{

    _draw_grid();
    _draw_event();
    auto square = get_local_square();
    _draw_panning();
    draw_string(get_theme_default_font(),
                square.get_center() + Vector2((square.get_size().x - get_theme_default_font_size()) / 2,
                                              get_theme_default_font_size() / 2.0),
                "X", godot::HORIZONTAL_ALIGNMENT_CENTER, -1, get_theme_default_font_size(), theme_cache.axis_x_color);

    if (view == FRONT)
    {
        draw_string(get_theme_default_font(), square.get_position() + Vector2(0, get_theme_default_font_size()), "Y",
                    HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER, square.get_size().x,
                    get_theme_default_font_size(), theme_cache.axis_y_color);
    }
    else
    {
        draw_string(get_theme_default_font(), square.get_position() + Vector2(0, square.get_size().y), "Z",
                    HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER, square.get_size().x,
                    get_theme_default_font_size(), theme_cache.axis_z_color);
    }
}

void FmodEventPanner::_gui_input(const Ref<InputEvent> &p_event)
{
    Ref<InputEventMouseButton> mb = p_event;
    Ref<InputEventMouseMotion> mm = p_event;
    if (mb.is_valid())
    {
        switch (mb->get_button_index())
        {
        case MOUSE_BUTTON_LEFT: {
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
                        event_position = Vector3(position.x, -position.y, event_position.z);
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
        break;
        case MOUSE_BUTTON_WHEEL_UP:
            rotation = fmod(rotation + (Math_PI / 16), Math_TAU);
            emit_signal("event_rotation_changed", Vector3(0, rotation, 0));
            queue_redraw();
            break;
        case MOUSE_BUTTON_WHEEL_DOWN:
            rotation -= Math_PI / 16;
            rotation = fmod(rotation - (Math_PI / 16), Math_TAU);
            emit_signal("event_rotation_changed", get_play_position());
            queue_redraw();
            break;
        default:
            break;
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
                event_position = Vector3(position.x, -position.y, event_position.z);
                emit_signal("play_position_changed", get_play_position());
                queue_redraw();
                break;
            case TOP:
                event_position = Vector3(position.x, event_position.y, position.y);
                emit_signal("play_position_changed", get_play_position());
                queue_redraw();
                break;
            }
        }
    }
}
#endif
