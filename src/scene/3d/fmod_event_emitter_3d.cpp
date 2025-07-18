#include "globals.h"
#include "fmod_event_emitter_3d.h"
namespace FmodGodot
{

    FmodEventEmitter3D::FmodEventEmitter3D()
    {
    }
    FmodEventEmitter3D::~FmodEventEmitter3D()
    {
    }
    void FmodEventEmitter3D::_notification(int p_what)
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

    void FmodEventEmitter3D::refresh_parameters()
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
            parameters.write[i] = {param.name, param.defaultvalue};
        }
    }
    void FmodEventEmitter3D::_bind_methods()
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
        ClassDB::bind_method(D_METHOD("start"), &FmodEventEmitter3D::start);
        ClassDB::bind_method(D_METHOD("stop"), &FmodEventEmitter3D::stop);
    }

    bool FmodEventEmitter3D::_set(const StringName &p_name, const Variant &p_property)
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

    bool FmodEventEmitter3D::_get(const StringName &p_name, Variant &r_property) const
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
                r_property = parameters[i].value;
                return true;
            }
        }
        return false;
    }

    bool FmodEventEmitter3D::_property_can_revert(const StringName &p_name) const
    {
        return p_name.begins_with("param:");
    }

    bool FmodEventEmitter3D::_property_get_revert(const StringName &p_name, Variant &r_property)
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

    void FmodEventEmitter3D::_get_property_list(List<PropertyInfo> *p_list) const
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

    void FmodEventEmitter3D::set_parameters()
    {
        if (FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            for (auto param : parameters)
            {
                FMOD_Studio_EventInstance_SetParameterByName(event_instance, param.name, param.value, false);
            }
        }
    }

    bool FmodEventEmitter3D::validate_event_description()
    {
        if (!FMOD_Studio_EventDescription_IsValid(description))
        {
            FMOD_GUID guid = cast_to_FMOD_GUID(event_guid);
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

    void FmodEventEmitter3D::start()
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
                FmodAudioServer::get_singleton()->attach_instance_to_node3d(this, event_instance, this->is_non_rigid_body_velocity());
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

    void FmodEventEmitter3D::stop()
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

    Vector4i FmodEventEmitter3D::get_event()
    {
        return event_guid;
    }

    void FmodEventEmitter3D::set_event(Vector4i p_event)
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

        FMOD_GUID guid = cast_to_FMOD_GUID(event_guid);
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

    bool FmodEventEmitter3D::is_one_shot() const
    {
        return one_shot;
    }

    void FmodEventEmitter3D::set_one_shot(bool p_one_shot)
    {
        one_shot = p_one_shot;
    }

    bool FmodEventEmitter3D::is_preload_sample_data() const
    {
        return preload_sample_data;
    }

    void FmodEventEmitter3D::set_preload_sample_data(bool p_preload_sample_data)
    {
        preload_sample_data = p_preload_sample_data;
    }

    bool FmodEventEmitter3D::is_trigger_once() const
    {
        return trigger_once;
    }

    void FmodEventEmitter3D::set_trigger_once(bool p_trigger_once)
    {
        trigger_once = p_trigger_once;
    }

    bool FmodEventEmitter3D::is_non_rigid_body_velocity() const
    {
        return non_rigid_body_velocity;
    }

    void FmodEventEmitter3D::set_non_rigid_body_velocity(bool p_non_rigid_body_velocity)
    {
        non_rigid_body_velocity = p_non_rigid_body_velocity;
    }

    bool FmodEventEmitter3D::is_override_attenuation() const
    {
        return override_attenuation;
    }

    void FmodEventEmitter3D::set_override_attenuation(bool p_override_attenuation)
    {
        override_attenuation = p_override_attenuation;
    }

    float FmodEventEmitter3D::get_attenuation_min() const
    {
        return attenuation_min;
    }

    void FmodEventEmitter3D::set_attenuation_min(float p_attenuation_min)
    {
        attenuation_min = p_attenuation_min;
    }

    float FmodEventEmitter3D::get_attenuation_max() const
    {
        return attenuation_max;
    }
    void FmodEventEmitter3D::set_attenuation_max(float p_attenuation_max)
    {
        attenuation_max = p_attenuation_max;
    }
    bool FmodEventEmitter3D::is_allow_fadeout() const
    {
        return allow_fadeout;
    }
    void FmodEventEmitter3D::set_allow_fadeout(bool p_allow_fadeout)
    {
        allow_fadeout = p_allow_fadeout;
    }
}