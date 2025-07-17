// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include "fmod_event_guid_selector_property.h"
#include <godot_cpp/core/class_db.hpp>
#include <fmod_globals.h>
#include <fmod_studio.h>
#include "fmod_audio_server.h"
#include <classes/project_settings.hpp>
#include <fmod_errors.h>
// #include <godot_cpp/classes/event_s
using namespace godot;
namespace FmodGodot
{
    EventGUIDSelectorProperty::EventGUIDSelectorProperty()
    {
        eventSelector = memnew(FmodEventPathSelector);
        add_child(eventSelector);
        eventSelector->get_line_edit()->connect("text_submitted", callable_mp(this, &EventGUIDSelectorProperty::on_text_changed));
        // eventSelector->get_line_edit()->connect("text_changed", callable_mp(this, &EventGUIDSelectorProperty::on_text_changed));
        eventSelector->get_line_edit()->connect("editing_toggled", callable_mp(this, &EventGUIDSelectorProperty::on_editing_toggled));
    }
    EventGUIDSelectorProperty::~EventGUIDSelectorProperty()
    {
        eventSelector->queue_free();
    }
    void EventGUIDSelectorProperty::on_editing_toggled(bool toggled_on)
    {
        if (!toggled_on)
        {
            on_text_changed(eventSelector->get_line_edit()->get_text());
        }
    }

    void EventGUIDSelectorProperty::on_text_changed(String newText)
    {
        FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_global_studio();
        FMOD_GUID guid;
        FMOD_Studio_System_LookupID(studio, newText.utf8(), &guid);
        eventSelector->get_line_edit()->set_tooltip_text(fmod_guid_to_string(guid));

        Vector4i newValue = cast_to_Vector4i(guid);
        if (newValue == Vector4i(0, 0, 0, 0))
        {
            _err_print_error("on_text_changed", __FILE__, __LINE__, "Could not find FMOD_GUID for [" + newText + "] setting will not persist only guid is stored", true, true);
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
        // ClassDB::bind_method(D_METHOD("on_editing_toggled", "toggled_on"), &EventGUIDSelectorProperty::on_editing_toggled);
    }

    void EventGUIDSelectorProperty::_update_property()
    {
        auto newValue = (Vector4i)get_edited_object()->get(get_edited_property());
        if (newValue == currentValue)
        {
            return;
        }
        // Update the control with the new value.
        updating = true;
        currentValue = newValue;
        int size = 128;
        int retrieved = 0;
        char *str = new char[size];
        FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_global_studio();
        if (!FMOD_Studio_System_IsValid(studio))
        {
            print_error("fmod studio system is not valid");
            updating = false;
            return;
        }
        FMOD_GUID guid = cast_to_FMOD_GUID(currentValue);
        FMOD_LOOKUP_FULL_STRING(FMOD_Studio_System_LookupPath, studio, &guid, str, size, retrieved)
        eventSelector->get_line_edit()->set_text(str);
        eventSelector->get_line_edit()->set_tooltip_text(fmod_guid_to_string(guid));
        updating = false;
        delete[] str;
    }
}
#endif