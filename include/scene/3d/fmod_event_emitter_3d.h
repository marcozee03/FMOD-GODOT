#pragma once
#include <classes/node3d.hpp>
#include "fmod_audio_server.h"
#include <variant/vector2i.hpp>
namespace FmodGodot
{
    class FmodEventEmitter3D : public Node3D
    {
        GDCLASS(FmodEventEmitter3D, Node3D);
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
        bool allow_fadeout;

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
        void _notification(int p_what);

    public:
        FmodEventEmitter3D(/* args */);
        ~FmodEventEmitter3D();
        void start();
        void stop();

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
    };

}