#pragma once
#include "classes/input_event.hpp"
#include "variant/rect2.hpp"
#include "variant/vector3.hpp"
#ifdef TOOLS_ENABLED
#include "classes/control.hpp"
#include "classes/wrapped.hpp"
#include "variant/vector4i.hpp"
using namespace godot;
class FmodEventPanner : public Control
{
    GDCLASS(FmodEventPanner, Control)
  public:
    enum View
    {
        FRONT,
        TOP,
    };

  private:
    float world_size = 100;
    float attenuation_min = 0;
    float attenuation_max = 0;
    bool dragging;
    float get_min_dimension() const;
    Rect2 get_local_square() const;
    Rect2 get_square() const;
    View view = TOP;
    Vector3 event_position;
    float rotation;
    void _update_theme();
    void _draw_grid();
    void _draw_event();
    void _draw_panning();
    struct
    {
        Color background_color;
        Color grid_color;
        Color contrast_color;
        Color axis_x_color;
        Color axis_y_color;
        Color axis_z_color;
    } theme_cache;

  protected:
    static void _bind_methods();
    void _notification(int p_what);

  public:
    FmodEventPanner();
    ~FmodEventPanner();

    void set_view(View view);
    View get_view() const;
    Vector3 get_play_position() const;
    float get_facing_angle() const;

    void set_world_size(float p_size);
    float get_world_size() const;

    void set_attenuation_min(float p_radius);
    void set_attenuation_max(float p_radius);

    void _gui_input(const Ref<InputEvent> &p_event) override;
    void _draw() override;
};

VARIANT_ENUM_CAST(FmodEventPanner::View);
#endif
