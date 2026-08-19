#include "fmod_globals.h"
#include "classes/project_settings.hpp"
#include "variant/dictionary.hpp"
#include "variant/vector2i.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4i.hpp>
using namespace godot;
Variant _GLOBAL_DEF(const String &p_var, const Variant &p_default, bool p_restart_if_changed, bool p_basic,
                    bool p_internal)
{
    Variant ret;
    if (!ProjectSettings::get_singleton()->has_setting(p_var))
    {
        ProjectSettings::get_singleton()->set(p_var, p_default);
    }
    ret = GLOBAL_GET(p_var);

    ProjectSettings::get_singleton()->set_initial_value(p_var, p_default);
    // ProjectSettings::get_singleton()->set_builtin_order(p_var);
    ProjectSettings::get_singleton()->set_as_basic(p_var, p_basic);
    ProjectSettings::get_singleton()->set_restart_if_changed(p_var, p_restart_if_changed);
    // ProjectSettings::get_singleton()->set_ignore_value_in_docs(p_var, p_ignore_value_in_docs);
    ProjectSettings::get_singleton()->set_as_internal(p_var, p_internal);
    return ret;
}

Variant _GLOBAL_DEF(const PropertyInfo &p_info, const Variant &p_default, bool p_restart_if_changed, bool p_basic,
                    bool p_internal)
{
    Variant ret = _GLOBAL_DEF(p_info.name, p_default, p_restart_if_changed, p_basic, p_internal);
    Dictionary prop = p_info;
    prop.erase("usage");
    ProjectSettings::get_singleton()->add_property_info(prop);
    return ret;
}
