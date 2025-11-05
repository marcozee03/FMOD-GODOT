#pragma once
#include "classes/input_event.hpp"
#include "variant/rect2.hpp"
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
    bool dragging;
    float get_min_dimension() const;
    Rect2 get_local_square() const;
    Rect2 get_square() const;
    View view = TOP;
    Vector3 event_position;

  protected:
    static void _bind_methods();

  public:
    FmodEventPanner();
    ~FmodEventPanner();

    void set_view(View view);
    View get_view() const;
    Vector3 get_play_position();

    Vector4i get_event() const;
    void set_event(Vector4i p_event);

    void set_world_size(float p_size);
    float get_world_size() const;
    void _gui_input(const Ref<InputEvent> &p_event) override;
    void _draw() override;
};

VARIANT_ENUM_CAST(FmodEventPanner::View);
#endif
