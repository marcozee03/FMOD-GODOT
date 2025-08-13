#pragma once
#include "fmod_audio_server.h"
#include "fmod_globals.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include "variant/packed_string_array.hpp"
#include "variant/string_name.hpp"
#include <fmod_studio.h>
namespace FmodGodot
{
template <class Derived, class NodeType, class RigidBody> class FmodEventEmitter : public NodeType
{
    struct parameter
    {
        const char *name;
        Vector2i id;
        float value;
    };

  private:
    Vector4i event_guid;
    bool one_shot = true;
    bool trigger_once = false;
    bool triggered = false;
    bool non_rigid_body_velocity = false;
    bool preload_sample_data = false;
    bool override_attenuation = false;
    float attenuation_min = 1;
    float attenuation_max = 100;
    bool allow_fadeout = true;
    bool attached_to_rigidbody;

  protected:
    FMOD_STUDIO_EVENTDESCRIPTION *description;
    FMOD_STUDIO_EVENTINSTANCE *event_instance;
    Vector<parameter> parameters;
    void refresh_parameters();
    void set_parameters();
    bool validate_event_description();
    bool _set(const StringName &p_name, const Variant &p_property);
    bool _get(const StringName &p_name, Variant &r_property) const;

    bool _property_can_revert(const StringName &p_name) const;
    bool _property_get_revert(const StringName &p_name, Variant &r_property);

    void _get_property_list(List<PropertyInfo> *p_list) const;

    static void _bind_methods();

  public:
    FmodEventEmitter<Derived, NodeType, RigidBody>() = default;
    ~FmodEventEmitter<Derived, NodeType, RigidBody>() = default;
    void start();
    void stop();
    bool is_attached_to_rigidbody() const;
    void set_attached_to_rigidbody(bool p_attached);
    Vector4i get_event();
    void set_event(Vector4i p_event);

    bool is_one_shot() const;
    void set_one_shot(bool p_one_shot);

    bool is_trigger_once() const;
    void set_trigger_once(bool p_trigger_once);

    bool is_non_rigid_body_velocity() const;
    void set_non_rigid_body_velocity(bool p_non_rigid_body_velocity);

    bool is_preload_sample_data() const;
    void set_preload_sample_data(bool p_preload_sample_data);

    bool is_override_attenuation() const;
    void set_override_attenuation(bool p_override_attenuation);

    float get_attenuation_min() const;
    void set_attenuation_min(float p_attenuation_min);

    float get_attenuation_max() const;
    void set_attenuation_max(float p_attenuation_max);

    bool is_allow_fadeout() const;
    void set_allow_fadeout(bool p_allow_fadeout);

    void set_parameter(const String &name, float value);
    void set_parameter_by_id(const Vector2i &id, float value);
    float get_parameter(const String &p_name) const;
    float get_parameter_by_id(const Vector2i &p_id) const;
    void _notification(int p_what);
    static const StringName &get_class_static();
};

template <class Derived, class NodeType, class RigidBody>
const StringName &FmodEventEmitter<Derived, NodeType, RigidBody>::get_class_static()
{
    return Derived::get_class_static();
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::_notification(int p_what)
{
    switch (p_what)
    {
    case godot::Node::NOTIFICATION_PREDELETE:
        /* code */
        break;
    case godot::Node::NOTIFICATION_EXIT_TREE:
        if (FMOD_Studio_EventInstance_IsValid(event_instance))
        {
            FMOD_Studio_EventInstance_Release(event_instance);
        }
        break;
    case godot::Node::NOTIFICATION_UNPARENTED:
    case godot::Node::NOTIFICATION_PARENTED:
        this->update_configuration_warnings();
        break;
    default:
        break;
    }
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::refresh_parameters()
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

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::_set(const StringName &p_name, const Variant &p_property)
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

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::_get(const StringName &p_name, Variant &r_property) const
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

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::_property_can_revert(const StringName &p_name) const
{
    return p_name.begins_with("param:");
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::_property_get_revert(const StringName &p_name, Variant &r_property)
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

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::_get_property_list(List<PropertyInfo> *p_list) const
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
    p_list->push_back(
        PropertyInfo(Variant::Type::STRING, "Parameters", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_SUBGROUP));
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

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_parameters()
{
    if (FMOD_Studio_EventInstance_IsValid(event_instance))
    {
        for (auto param : parameters)
        {
            FMOD_Studio_EventInstance_SetParameterByName(event_instance, param.name, param.value, false);
        }
    }
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::validate_event_description()
{
    if (!FMOD_Studio_EventDescription_IsValid(description))
    {
        FMOD_GUID guid = cast_to_fmod_guid(event_guid);
        FMOD_RESULT result =
            FMOD_Studio_System_GetEventByID(FmodAudioServer::get_singleton()->get_studio(), &guid, &description);
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

template <class Derived, class NodeType, class RigidBody> void FmodEventEmitter<Derived, NodeType, RigidBody>::start()
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
            if (attached_to_rigidbody)
            {
                auto ptr = Object::cast_to<RigidBody>(this->get_parent());
                if (ptr)
                {
                    static_cast<Derived *>(this)->attach_instance_to_rigid_body(ptr, event_instance);
                }
                else
                {
                    print_error("Event Emitters parent is not a rigidbody2D");
                    static_cast<Derived *>(this)->attach_instance_to_node(this, event_instance,
                                                                          this->is_non_rigid_body_velocity());
                }
            }
            else
            {
                static_cast<Derived *>(this)->attach_instance_to_node(this, event_instance,
                                                                      this->is_non_rigid_body_velocity());
            }
        }
    }
    triggered = true;
    this->set_parameters();
    if (override_attenuation && is3D)
    {
        FMOD_Studio_EventInstance_SetProperty(event_instance, FMOD_STUDIO_EVENT_PROPERTY_MINIMUM_DISTANCE,
                                              attenuation_min);
        FMOD_Studio_EventInstance_SetProperty(event_instance, FMOD_STUDIO_EVENT_PROPERTY_MAXIMUM_DISTANCE,
                                              attenuation_min);
    }
    FMOD_Studio_EventInstance_Start(event_instance);
}

template <class Derived, class NodeType, class RigidBody> void FmodEventEmitter<Derived, NodeType, RigidBody>::stop()
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

template <class Derived, class NodeType, class RigidBody>
Vector4i FmodEventEmitter<Derived, NodeType, RigidBody>::get_event()
{
    return event_guid;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_event(Vector4i p_event)
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
template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_one_shot() const
{
    return one_shot;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_one_shot(bool p_one_shot)
{
    one_shot = p_one_shot;
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_preload_sample_data() const
{
    return preload_sample_data;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_preload_sample_data(bool p_preload_sample_data)
{
    preload_sample_data = p_preload_sample_data;
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_trigger_once() const
{
    return trigger_once;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_trigger_once(bool p_trigger_once)
{
    trigger_once = p_trigger_once;
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_non_rigid_body_velocity() const
{
    return non_rigid_body_velocity;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_non_rigid_body_velocity(bool p_non_rigid_body_velocity)
{
    non_rigid_body_velocity = p_non_rigid_body_velocity;
}

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_override_attenuation() const
{
    return override_attenuation;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_override_attenuation(bool p_override_attenuation)
{
    override_attenuation = p_override_attenuation;
}

template <class Derived, class NodeType, class RigidBody>
float FmodEventEmitter<Derived, NodeType, RigidBody>::get_attenuation_min() const
{
    return attenuation_min;
}

template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_attenuation_min(float p_attenuation_min)
{
    attenuation_min = p_attenuation_min;
}

template <class Derived, class NodeType, class RigidBody>
float FmodEventEmitter<Derived, NodeType, RigidBody>::get_attenuation_max() const
{
    return attenuation_max;
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_attenuation_max(float p_attenuation_max)
{
    attenuation_max = p_attenuation_max;
}
template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_allow_fadeout() const
{
    return allow_fadeout;
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_allow_fadeout(bool p_allow_fadeout)
{
    allow_fadeout = p_allow_fadeout;
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_parameter(const String &name, float value)
{
    for (int i = 0; i < parameters.size(); i++)
    {
        if (name.casecmp_to(parameters[i].name) == 0)
        {
            parameters.write[i].value = value;
            return;
        }
    }
    print_error("No parameter of name: ", name, " in ", this->get_name());
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_parameter_by_id(const Vector2i &id, float value)
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
template <class Derived, class NodeType, class RigidBody>
float FmodEventEmitter<Derived, NodeType, RigidBody>::get_parameter(const String &p_name) const
{
    for (int i = 0; i < parameters.size(); i++)
    {
        if (p_name.casecmp_to(parameters[i].name) == 0)
        {
            return parameters[i].value;
        }
    }
    print_error("No parameter of name: ", p_name, " in ", this->get_name());
    return NAN;
}
template <class Derived, class NodeType, class RigidBody>
float FmodEventEmitter<Derived, NodeType, RigidBody>::get_parameter_by_id(const Vector2i &p_id) const
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

template <class Derived, class NodeType, class RigidBody>
bool FmodEventEmitter<Derived, NodeType, RigidBody>::is_attached_to_rigidbody() const
{
    return attached_to_rigidbody;
}
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::set_attached_to_rigidbody(bool p_attached)
{
    attached_to_rigidbody = p_attached;
    this->update_configuration_warnings();
};
template <class Derived, class NodeType, class RigidBody>
void FmodEventEmitter<Derived, NodeType, RigidBody>::_bind_methods()
{
    typedef typename Derived::self_type self_type;
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
    BIND_BOOL_PROPERTY(attached_to_rigidbody);
}
} // namespace FmodGodot
