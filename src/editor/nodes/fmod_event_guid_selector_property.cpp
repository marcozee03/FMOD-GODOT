#include "fmod_event_guid_selector_property.h"
#include "conversions.h"
#include "fmod_audio_server.h"
#include "fmod_event_selector.h"
#include <classes/project_settings.hpp>
#include <fmod_defs.h>
#include <fmod_errors.h>
#include <fmod_studio.h>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
// #include <godot_cpp/classes/event_s
using namespace godot;
namespace FmodGodot
{
EventGUIDSelectorProperty::EventGUIDSelectorProperty()
{
    eventSelector = memnew(FmodEventSelector);
    add_child(eventSelector);
    eventSelector->get_line_edit()->connect("text_submitted",
                                            callable_mp(this, &EventGUIDSelectorProperty::on_text_changed));
    // eventSelector->get_line_edit()->connect("text_changed", callable_mp(this,
    // &EventGUIDSelectorProperty::on_text_changed));
    eventSelector->get_line_edit()->connect("editing_toggled",
                                            callable_mp(this, &EventGUIDSelectorProperty::on_editing_toggled));
}
EventGUIDSelectorProperty::~EventGUIDSelectorProperty()
{
}
void EventGUIDSelectorProperty::on_editing_toggled(bool p_toggled_on)
{
    if (!p_toggled_on)
    {
        on_text_changed(eventSelector->get_line_edit()->get_text());
    }
}

void EventGUIDSelectorProperty::on_text_changed(String p_new_text)
{
    FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_singleton()->get_studio();
    FMOD_GUID guid;
    FMOD_Studio_System_LookupID(studio, p_new_text.utf8().ptr(), &guid);
    eventSelector->get_line_edit()->set_tooltip_text(fmod_guid_to_string(guid));

    Vector4i newValue = cast_to_vector4i(guid);
    if (newValue == Vector4i(0, 0, 0, 0))
    {
        _err_print_error("on_text_changed", __FILE__, __LINE__,
                         "Could not find FMOD_GUID for [" + p_new_text +
                             "] setting will not persist only guid is stored",
                         true, true);
    }
    if (currentValue != newValue)
    {
        currentValue = newValue;
        emit_changed(get_edited_property(), currentValue);
    }
}

void EventGUIDSelectorProperty::_bind_methods()
{
    // ClassDB::bind_method(D_METHOD("on_text_changed", "new_text"), &EventGUIDSelectorProperty::on_text_changed);
    // ClassDB::bind_method(D_METHOD("on_editing_toggled", "toggled_on"),
    // &EventGUIDSelectorProperty::on_editing_toggled);
}

void EventGUIDSelectorProperty::_update_property()
{
    auto newValue = static_cast<Vector4i>(get_edited_object()->get(get_edited_property()));
    if (newValue == currentValue)
    {
        return;
    }
    // Update the control with the new value.
    updating = true;
    currentValue = newValue;
    FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_singleton()->get_studio();
    if (!FMOD_Studio_System_IsValid(studio))
    {
        print_error("fmod studio system is not valid");
        updating = false;
        return;
    }
    FMOD_GUID guid = cast_to_fmod_guid(currentValue);
    FMOD_LOOKUP_STRING(FMOD_Studio_System_LookupPath, studio, event_path, &guid)
    eventSelector->get_line_edit()->set_text(event_path);
    eventSelector->get_line_edit()->set_tooltip_text(fmod_guid_to_string(guid));
    updating = false;
}
} // namespace FmodGodot
