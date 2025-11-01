#pragma once
#include "fmod_event_panner.h"
#include "variant/vector4i.hpp"
#ifdef TOOLS_ENABLED
#include "classes/button.hpp"
#include "classes/control.hpp"
#include "classes/h_box_container.hpp"
#include "classes/v_box_container.hpp"
#include "core/memory.hpp"
#include "fmod_event_emitter_3d.h"
using namespace godot;
namespace FmodGodot
{
class FmodEventPreviewer : public VBoxContainer
{
  private:
    void start();
    void toggleFrontView();
    void go_to_event();

  public:
    FmodEventEmitter3D *emitter;

    HBoxContainer *mediaControls;
    Button *play;
    Button *stop;
    Button *goToEvent;
    Button *frontview;
    FmodEventPanner *panner;
    FmodEventPreviewer();
    Vector4i get_event_guid() const;

    void set_event_guid(Vector4i p_event_guid);
    void set_panner_size(float size);
};
} // namespace FmodGodot
#endif
