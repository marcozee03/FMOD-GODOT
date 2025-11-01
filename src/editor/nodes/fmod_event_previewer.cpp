#include "fmod_event_previewer.h"
#include "classes/button.hpp"
#include "classes/editor_interface.hpp"
#include "classes/theme.hpp"
#include "core/memory.hpp"
#include "fmod_editor_interface.h"
#include "fmod_event_panner.h"
#include "variant/callable.hpp"
#include "variant/callable_method_pointer.hpp"
#include "variant/vector4i.hpp"
using namespace FmodGodot;
FmodEventPreviewer::FmodEventPreviewer()
{
    emitter = memnew(FmodEventEmitter3D);
    mediaControls = memnew(HBoxContainer);
    mediaControls->set_h_size_flags(SIZE_SHRINK_CENTER);
    frontview = memnew(Button);
    frontview->set_toggle_mode(true);
    frontview->connect("pressed", callable_mp(this, &FmodEventPreviewer::toggleFrontView));
    panner = memnew(FmodEventPanner);
    panner->set_h_size_flags(SIZE_EXPAND_FILL);
    panner->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(emitter);

    play = memnew(Button);
    play->set_tooltip_text("Play Sound");
    stop = memnew(Button);
    stop->set_tooltip_text("Stop Sound");
    goToEvent = memnew(Button);
    goToEvent->set_tooltip_text("Show in FMOD Studio");
    Ref<Theme> et = EditorInterface::get_singleton()->get_editor_theme();
    goToEvent->connect("pressed", callable_mp(this, &FmodEventPreviewer::go_to_event));
    add_child(mediaControls);
    mediaControls->add_child(play);
    play->set_button_icon(et->get_icon("Play", "EditorIcons"));
    play->connect("pressed", callable_mp(this, &FmodEventPreviewer::start));
    mediaControls->add_child(stop);
    stop->connect("pressed", Callable(emitter, "stop"));
    stop->set_button_icon(et->get_icon("Stop", "EditorIcons"));
    mediaControls->add_child(goToEvent);
    goToEvent->set_button_icon(FmodEditorInterface::get_singleton()->get_theme()->fmod_icon);
    frontview->set_text("Top");
    mediaControls->add_child(frontview);
    add_child(panner);
}
void FmodEventPreviewer::go_to_event()
{
    FmodEditorInterface::get_singleton()->show_event_in_fmod_studio(get_event_guid());
}
Vector4i FmodEventPreviewer::get_event_guid() const
{
    return emitter->get_event();
}
void FmodEventPreviewer::set_event_guid(Vector4i p_event_guid)
{
    emitter->set_event(p_event_guid);
}
void FmodGodot::FmodEventPreviewer::start()
{
    emitter->set_global_position(panner->get_play_position());
    emitter->start();
}
void FmodGodot::FmodEventPreviewer::set_panner_size(float size)
{
    panner->set_size(size);
}
void FmodGodot::FmodEventPreviewer::toggleFrontView()
{
    switch (panner->get_view())
    {
    case FmodEventPanner::FRONT:
        panner->set_view(FmodEventPanner::TOP);
        frontview->set_text("Top");
        break;
    case FmodEventPanner::TOP:
        panner->set_view(FmodEventPanner::FRONT);
        frontview->set_text("Front");
        break;
    }
}
