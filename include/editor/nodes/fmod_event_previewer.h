#pragma once
#include "classes/h_slider.hpp"
#include "classes/spin_box.hpp"
#include "classes/wrapped.hpp"
#ifdef TOOLS_ENABLED
#include "classes/button.hpp"
#include "classes/editor_property.hpp"
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include "fmod_event_emitter_3d.h"
#include "fmod_event_panner.h"
#include "variant/string.hpp"
#include "variant/variant.hpp"
#include "variant/vector4i.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodEventPreviewer : public VBoxContainer
{
    GDCLASS(FmodEventPreviewer, VBoxContainer)
  private:
    void start();
    void toggleFrontView();
    void go_to_event();
    Vector<EditorProperty *> editor_properties;
    void on_property_changed(const String &property, Variant value, const String &field, bool changing);
    void on_start_drag();
    void on_end_drag(bool value_changed);

  protected:
    static void _bind_methods();

  public:
    FmodEventEmitter3D *emitter;

    HBoxContainer *mediaControls;
    Button *play;
    Button *stop;
    Button *goToEvent;
    SpinBox *radius;
    Button *frontview;
    FmodEventPanner *panner;
    FmodEventPreviewer();
    HSlider *scrub;
    Vector4i get_event_guid() const;
    void _process(double delta) override;
    void set_event_guid(Vector4i p_event_guid);
    void set_event_path(const String &p_event_path);
    void set_panner_size(float size);
};
} // namespace FmodGodot
#endif
