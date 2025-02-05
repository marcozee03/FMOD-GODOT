#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include "fmod_event_guid_selector_property.h"
#include <godot_cpp/core/class_db.hpp>
#include <fmod_globals.h>
#include <fmod_studio.hpp>
#include "fmodengine.h"
#include <classes/project_settings.hpp>
#include <fmod_errors.h>
#include "fmodengine.h"
// #include <godot_cpp/classes/event_s
using namespace godot;
using namespace FMOD;
namespace FMODGodot
{
    EventGUIDSelectorProperty::EventGUIDSelectorProperty()
    {
        eventSelector = memnew(FmodEventPathSelector);
        add_child(eventSelector);
        eventSelector->get_line_edit()->connect("text_changed", Callable(this, "on_text_changed"));
    }
    EventGUIDSelectorProperty::~EventGUIDSelectorProperty()
    {
        // eventSelector->queue_free();
    }
    void EventGUIDSelectorProperty::on_text_changed(String newText)
    {
        if (updating)
        {
            return;
        }
        updating = true;
        Vector4i newValue = eventSelector->get_event_tree()->get_selected()->get_metadata(0);
        if (currentValue != newValue)
        {
            currentValue = newValue;
            emit_changed(get_edited_property(), currentValue);
        }
    }

    void EventGUIDSelectorProperty::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("on_text_changed", "new_text"), &EventGUIDSelectorProperty::on_text_changed);
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
        int retrieved = 0;
        char *str = new char[128];
        Studio::System *studio = FMODEngine::get_global_studio();
        if (!studio->isValid())
        {
            print_error("fmod studio system is not valid");
            return;
        }
        Studio::Bank *MasterStrings;
        ProjectSettings* pj = ProjectSettings::get_singleton();
        FMOD_GUID guid = cast_to_FMOD_GUID(currentValue);
        auto result = studio->lookupPath(&guid, str, 128, &retrieved);
        if (result == FMOD_ERR_TRUNCATED)
        {
            delete[] str;
            str = new char[retrieved];
            studio->lookupPath(&guid, str, retrieved, &retrieved);
        }

        eventSelector->get_line_edit()->set_text(str);
        eventSelector->get_line_edit()->set_tooltip_text(fmod_guid_to_string(guid));
        updating = false;
        delete[] str;
    }
}
#endif