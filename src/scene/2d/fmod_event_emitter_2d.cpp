#include "fmod_event_emitter_2d.h"
#include "globals.h"
namespace FmodGodot
{
    FmodEventEmitter2D::FmodEventEmitter2D()
    {
    }
    FmodEventEmitter2D::~FmodEventEmitter2D()
    {
    }
    void FmodEventEmitter2D::_notification(int p_what)
    {
        switch (p_what)
        {
        case NOTIFICATION_PREDELETE:
            /* code */
            break;
        case NOTIFICATION_EXIT_TREE:
            if (FMOD_Studio_EventInstance_IsValid(event_instance))
            {
                FMOD_Studio_EventInstance_Release(event_instance);
            }
            break;
        default:
            break;
        }
    }
    void FmodEventEmitter2D::_bind_methods()
    {
        BIND_PROPERTY_WITH_HINT(event, Variant::VECTOR4I, PROPERTY_HINT_NONE, "FmodEvent");
        BIND_BOOL_PROPERTY(one_shot)
        BIND_BOOL_PROPERTY(trigger_once);
        BIND_BOOL_PROPERTY(non_rigid_body_velocity);
        BIND_BOOL_PROPERTY(preload_sample_data);
        BIND_BOOL_PROPERTY(override_attenuation);
        BIND_PROPERTY(attenuation_min, Variant::Type::INT);
        BIND_PROPERTY(attenuation_max, Variant::Type::INT);
        BIND_BOOL_PROPERTY(allow_fadeout);
        BIND_METHOD(start);
        BIND_METHOD(stop);
        BIND_METHOD(set_parameter, "name", "value");
        BIND_METHOD(set_parameter_by_id, "id", "value");
        BIND_METHOD(get_parameter, "name");
        BIND_METHOD(get_parameter_by_id, "id");
    }

    void FmodEventEmitter2D::refresh_parameters()
    {
        if (!FMOD_Studio_EventDescription_IsValid(description))
        {
            return;
        }
        int count;
        FMOD_Studio_EventDescription_GetParameterDescriptionCount(description, &count);
        parameters.resize(count);
        for (int i = 0; i < count; i++)
        {
            FMOD_STUDIO_PARAMETER_DESCRIPTION param;
            FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(description, i, &param);
            parameters.write[i] = {param.name, cast_to_vector2i(param.id), param.defaultvalue};
        }
    }

    bool FmodEventEmitter2D::_set(const StringName &p_name, const Variant &p_property)
    {
        if (!p_name.begins_with("param:"))
        {
            return false;
        }
        auto str = p_name.substr(6);
        for (int i = 0; i < parameters.size(); i++)
        {
            if (str.casecmp_to(parameters[i].name) == 0)
            {
                parameters.write[i].value = (float)p_property;
                return true;
            }
        }
        parameters.push_back({str.utf8().ptr(), p_property});
        return p_name.begins_with("param:");
    }

    bool FmodEventEmitter2D::_get(const StringName &p_name, Variant &r_property) const
    {
        if (!p_name.begins_with("param:"))
        {
            return false;
        }
        float temp = get_parameter(p_name.substr(6));
        if (temp == NAN)
        {
            return false;
        }
        else
        {
            r_property = temp;
            return true;
        }
    }

    bool FmodEventEmitter2D::_property_can_revert(const StringName &p_name) const
    {
        return p_name.begins_with("param:");
    }

    bool FmodEventEmitter2D::_property_get_revert(const StringName &p_name, Variant &r_property)
    {
        if (!p_name.begins_with("param:"))
        {
            return false;
        }
        FMOD_STUDIO_PARAMETER_DESCRIPTION param;
        FMOD_Studio_EventDescription_GetParameterDescriptionByName(description, p_name.substr(6).utf8(), &param);
        r_property = param.defaultvalue;
        return true;
    }

    void FmodEventEmitter2D::_get_property_list(List<PropertyInfo> *p_list) const
    {
        if (!FMOD_Studio_EventDescription_IsValid(description))
        {
            print_error("event description was not valid defaulting to cache");
            for (auto param : parameters)
            {
                PropertyInfo info;
                info.name = "param:" + String(param.name);
                info.hint = PROPERTY_HINT_NONE;
                info.type = Variant::FLOAT;
                info.usage = PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;
                p_list->push_back(info);
            }
            return;
        }
        int count;
        p_list->push_back(PropertyInfo(Variant::Type::STRING, "Parameters", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SUBGROUP));
        FMOD_Studio_EventDescription_GetParameterDescriptionCount(description, &count);
        for (int i = 0; i < count; i++)
        {
            FMOD_STUDIO_PARAMETER_DESCRIPTION param;
            FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(description, i, &param);
            if (param.type != FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
            {
                continue;
            }
            PropertyInfo info;
            info.name = "param:" + String(param.name);
            info.hint = PROPERTY_HINT_RANGE;
            info.type = Variant::FLOAT;
            info.usage = PropertyUsageFlags::PROPERTY_USAGE_DEFAULT;
            if (param.flags & FMOD_STUDIO_PARAMETER_DISCRETE)
            {
                info.hint_string = String(",").join({Variant(param.minimum), Variant(param.maximum), Variant(1)});
            }
            else
            {
                info.hint_string = String(",").join({Variant(param.minimum), Variant(param.maximum)});
            }
            p_list->push_back(info);
        }
    }

    void FmodEventEmitter2D::set_parameters()
    {
        if (FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            for (auto param : parameters)
            {
                FMOD_Studio_EventInstance_SetParameterByName(event_instance, param.name, param.value, false);
            }
        }
    }

    bool FmodEventEmitter2D::validate_event_description()
    {
        if (!FMOD_Studio_EventDescription_IsValid(description))
        {
            FMOD_GUID guid = cast_to_fmod_guid(event_guid);
            FMOD_RESULT result = FMOD_Studio_System_GetEventByID(FmodAudioServer::get_singleton()->get_studio(), &guid, &description);
            if (preload_sample_data)
            {
                FMOD_Studio_EventDescription_LoadSampleData(description);
            }
            return result == FMOD_OK;
        }
        else
        {
            return true;
        }
    }

    void FmodEventEmitter2D::start()
    {
        if (triggered && this->is_trigger_once())
        {
            return;
        }
        if (!this->validate_event_description())
        {
            return;
        }
        if (FMOD_Studio_EventInstance_IsValid(event_instance) && this->is_one_shot())
        {
            FMOD_Studio_EventInstance_Release(event_instance);
            event_instance = nullptr;
        }
        FMOD_BOOL is3D;
        FMOD_Studio_EventDescription_Is3D(description, &is3D);
        if (!FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            FMOD_Studio_EventDescription_CreateInstance(description, &event_instance);
            if (is3D)
            {
                FmodAudioServer::get_singleton()->attach_instance_to_node2D(this, event_instance, this->is_non_rigid_body_velocity());
            }
        }
        triggered = true;
        this->set_parameters();
        if (override_attenuation && is3D)
        {
            FMOD_Studio_EventInstance_SetProperty(event_instance, FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE, attenuation_min);
            FMOD_Studio_EventInstance_SetProperty(event_instance, FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE, attenuation_min);
        }
        FMOD_Studio_EventInstance_Start(event_instance);
    }

    void FmodEventEmitter2D::stop()
    {
        if (!FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            return;
        }
        if (is_allow_fadeout())
        {
            FMOD_Studio_EventInstance_Stop(event_instance, FMOD_STUDIO_STOP_ALLOWFADEOUT);
        }
        else
        {
            FMOD_Studio_EventInstance_Stop(event_instance, FMOD_STUDIO_STOP_IMMEDIATE);
        }
        FMOD_Studio_EventInstance_Release(event_instance);
    }

    Vector4i FmodEventEmitter2D::get_event()
    {
        return event_guid;
    }

    void FmodEventEmitter2D::set_event(Vector4i p_event)
    {
        if (p_event == event_guid)
        {
            return;
        }
        event_guid = p_event;
        if (FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            FMOD_Studio_EventInstance_Release(event_instance);
        }

        FMOD_GUID guid = cast_to_fmod_guid(event_guid);
        FMOD_Studio_System_GetEventByID(FmodAudioServer::get_singleton()->get_studio(), &guid, &description);
        if (preload_sample_data)
        {
            FMOD_Studio_EventDescription_LoadSampleData(description);
        }
        if (!validate_event_description())
        {
            return;
        }
        refresh_parameters();
        this->notify_property_list_changed();
    }

    bool FmodEventEmitter2D::is_one_shot() const
    {
        return one_shot;
    }

    void FmodEventEmitter2D::set_one_shot(bool p_one_shot)
    {
        one_shot = p_one_shot;
    }

    bool FmodEventEmitter2D::is_preload_sample_data() const
    {
        return preload_sample_data;
    }

    void FmodEventEmitter2D::set_preload_sample_data(bool p_preload_sample_data)
    {
        preload_sample_data = p_preload_sample_data;
    }

    bool FmodEventEmitter2D::is_trigger_once() const
    {
        return trigger_once;
    }

    void FmodEventEmitter2D::set_trigger_once(bool p_trigger_once)
    {
        trigger_once = p_trigger_once;
    }

    bool FmodEventEmitter2D::is_non_rigid_body_velocity() const
    {
        return non_rigid_body_velocity;
    }

    void FmodEventEmitter2D::set_non_rigid_body_velocity(bool p_non_rigid_body_velocity)
    {
        non_rigid_body_velocity = p_non_rigid_body_velocity;
    }

    bool FmodEventEmitter2D::is_override_attenuation() const
    {
        return override_attenuation;
    }

    void FmodEventEmitter2D::set_override_attenuation(bool p_override_attenuation)
    {
        override_attenuation = p_override_attenuation;
    }

    float FmodEventEmitter2D::get_attenuation_min() const
    {
        return attenuation_min;
    }

    void FmodEventEmitter2D::set_attenuation_min(float p_attenuation_min)
    {
        attenuation_min = p_attenuation_min;
    }

    float FmodEventEmitter2D::get_attenuation_max() const
    {
        return attenuation_max;
    }
    void FmodEventEmitter2D::set_attenuation_max(float p_attenuation_max)
    {
        attenuation_max = p_attenuation_max;
    }
    bool FmodEventEmitter2D::is_allow_fadeout() const
    {
        return allow_fadeout;
    }
    void FmodEventEmitter2D::set_allow_fadeout(bool p_allow_fadeout)
    {
        allow_fadeout = p_allow_fadeout;
    }
    void FmodEventEmitter2D::set_parameter(const String &name, float value)
    {
        for (int i = 0; i < parameters.size(); i++)
        {
            if (name.casecmp_to(parameters[i].name) == 0)
            {
                parameters.write[i].value = value;
                return;
            }
        }
        print_error("No parameter of name: ", name, " in ", get_name());
    }
    void FmodEventEmitter2D::set_parameter_by_id(const Vector2i &id, float value)
    {
        for (int i = 0; i < parameters.size(); i++)
        {
            if (id == parameters[i].id)
            {
                parameters.write[i].value = value;
                return;
            }
        }
        print_error("No parameter with id: ", id);
    }
    float FmodEventEmitter2D::get_parameter(const String &p_name) const
    {
        for (int i = 0; i < parameters.size(); i++)
        {
            if (p_name.casecmp_to(parameters[i].name) == 0)
            {
                return parameters[i].value;
            }
        }
        print_error("No parameter of name: ", p_name, " in ", get_name());
        return NAN;
    }
    float FmodEventEmitter2D::get_parameter_by_id(const Vector2i &p_id) const
    {
        for (int i = 0; i < parameters.size(); i++)
        {
            if (p_id == parameters[i].id)
            {
                return parameters[i].value;
            }
        }
        print_error("No parameter with id: ", p_id);
        return NAN;
    }
}