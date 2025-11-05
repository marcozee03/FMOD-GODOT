#include "classes/spin_box.hpp"
#include "fmod_studio.h"
#ifdef TOOLS_ENABLED
#include "classes/button.hpp"
#include "classes/control.hpp"
#include "classes/editor_interface.hpp"
#include "classes/global_constants.hpp"
#include "classes/h_box_container.hpp"
#include "classes/theme.hpp"
#include "core/memory.hpp"
#include "fmod_editor_interface.h"
#include "fmod_event_panner.h"
#include "fmod_event_previewer.h"
#include "variant/callable.hpp"
#include "variant/callable_method_pointer.hpp"
#include "variant/dictionary.hpp"
#include "variant/string.hpp"
#include "variant/variant.hpp"
#include "variant/vector4i.hpp"
#include <classes/editor_inspector.hpp>
#include <classes/editor_property.hpp>
using namespace FmodGodot;
FmodEventPreviewer::FmodEventPreviewer()
{
    emitter = memnew(FmodEventEmitter3D);
    emitter->set_one_shot(false);
    mediaControls = memnew(HBoxContainer);
    mediaControls->set_h_size_flags(SIZE_SHRINK_CENTER);
    frontview = memnew(Button);
    frontview->set_toggle_mode(true);
    frontview->connect("pressed", callable_mp(this, &FmodEventPreviewer::toggleFrontView));
    panner = memnew(FmodEventPanner);
    panner->set_h_size_flags(SIZE_EXPAND_FILL);
    panner->set_v_size_flags(SIZE_EXPAND_FILL);
    add_child(emitter);

    scrub = memnew(HSlider);
    scrub->set_h_size_flags(SIZE_EXPAND_FILL);

    play = memnew(Button);
    play->set_tooltip_text("Play Sound");
    stop = memnew(Button);
    stop->set_tooltip_text("Stop Sound");
    goToEvent = memnew(Button);
    goToEvent->set_tooltip_text("Show in FMOD Studio");
    Ref<Theme> et = EditorInterface::get_singleton()->get_editor_theme();
    goToEvent->connect("pressed", callable_mp(this, &FmodEventPreviewer::go_to_event));
    mediaControls->add_child(play);
    play->set_button_icon(et->get_icon("Play", "EditorIcons"));
    play->connect("pressed", callable_mp(this, &FmodEventPreviewer::start));
    mediaControls->add_child(stop);
    stop->connect("pressed", Callable(emitter, "stop"));
    stop->connect("pressed", Callable(this, "set_process").bind(false));
    stop->set_button_icon(et->get_icon("Stop", "EditorIcons"));
    mediaControls->add_child(goToEvent);
    goToEvent->set_button_icon(FmodEditorInterface::get_singleton()->get_theme()->fmod_icon);
    frontview->set_text("Top");
    mediaControls->add_child(frontview);
    add_child(panner);
    add_child(mediaControls);
    add_child(scrub);
    scrub->set_step(0);
    scrub->connect("drag_started", callable_mp(this, &FmodEventPreviewer::on_start_drag));
    scrub->connect("drag_ended", callable_mp(this, &FmodEventPreviewer::on_end_drag));

    radius = memnew(SpinBox);
    radius->set_min(0.01);
    radius->set_step(0.01);
    radius->set_allow_greater(true);
    radius->set_max(INFINITY);
    mediaControls->add_child(radius);
    radius->connect("value_changed", callable_mp(this, &FmodEventPreviewer::set_panner_size));
    panner->connect("play_position_changed", Callable(emitter, "set_position"));
}
void FmodEventPreviewer::go_to_event()
{
    FmodEditorInterface::get_singleton()->show_event_in_fmod_studio(get_event_guid());
}
Vector4i FmodEventPreviewer::get_event_guid() const
{
    return emitter->get_event();
}

void FmodEventPreviewer::_process(double delta)
{
    if (!scrub->is_visible())
    {
        set_process(false);
        return;
    }
    if (emitter->is_paused())
    {
        return;
    }
    int position;
    FMOD_Studio_EventInstance_GetTimelinePosition(emitter->event_instance, &position);
    scrub->set_value(position);
}
void FmodEventPreviewer::set_event_path(const String &p_event_path)
{
    emitter->stop();
    set_process(false);
    if (!p_event_path.begins_with("event:/"))
    {
        hide();
        return;
    }
    show();
    auto event = FmodEditorInterface::get_singleton()->get_cache()->get_event(p_event_path);
    radius->set_value(event.max * 2);
    // set_panner_size(event.max * 2);
    if (event.lengthMS == 0)
    {
        scrub->hide();
    }
    else
    {
        scrub->show();
    }
    scrub->set_value(0);
    scrub->set_max(event.lengthMS);
    set_event_guid(event.guid);
}
void FmodEventPreviewer::set_event_guid(Vector4i p_event_guid)
{
    emitter->set_event(p_event_guid);
    for (auto e_property : editor_properties)
    {
        e_property->queue_free();
    }
    editor_properties.clear();
    for (Dictionary property : emitter->get_property_list())
    {
        if (!property["name"].stringify().begins_with("param:"))
        {
            continue;
        }
        auto inspector = EditorInterface::get_singleton()->get_inspector();
        String name = property["name"];
        Variant::Type vType = (Variant::Type)(int)property["type"];
        PropertyHint hint = (PropertyHint)(int)property["hint"];
        EditorProperty *prop = inspector->instantiate_property_editor(emitter, vType, name, hint,
                                                                      property["hint_string"], property["usage"]);
        prop->set_object_and_property(emitter, name);
        editor_properties.push_back(prop);
        prop->connect("property_changed", callable_mp(this, &FmodEventPreviewer::on_property_changed));
        prop->set_label(name.substr(6));
        add_child(prop);
    }
}
void FmodGodot::FmodEventPreviewer::start()
{
    emitter->set_global_position(panner->get_play_position());
    emitter->start();
    if (scrub->get_value() < scrub->get_max())
    {
        FMOD_Studio_EventInstance_SetTimelinePosition(emitter->event_instance, scrub->get_value());
    }
    set_process(true);
}
void FmodGodot::FmodEventPreviewer::set_panner_size(float size)
{
    panner->set_world_size(size);
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
void FmodGodot::FmodEventPreviewer::on_property_changed(const String &property, Variant value, const String &field,
                                                        bool changing)
{
    emitter->set(property, value);
}
void FmodGodot::FmodEventPreviewer::_bind_methods()
{
}
void FmodEventPreviewer::on_start_drag()
{
    emitter->set_paused(true);
}

void FmodEventPreviewer::on_end_drag(bool value_changed)
{
    if (value_changed)
    {
        FMOD_Studio_EventInstance_SetTimelinePosition(emitter->event_instance, scrub->get_value());
    }
    emitter->set_paused(false);
}
#endif
