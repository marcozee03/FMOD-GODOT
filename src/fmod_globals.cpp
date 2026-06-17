#include "fmod_globals.h"
#include "classes/project_settings.hpp"
#include "fmod_common.h"
#include "variant/dictionary.hpp"
#include "variant/vector2i.hpp"
#include <cstdio>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4i.hpp>
using namespace godot;
namespace FmodGodot
{

char *to_char_ptr(const String &str)
{
    return str.utf8().ptrw();
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"

Vector4i cast_to_vector4i(const FMOD_GUID &p_guid)
{
    static_assert(sizeof(Vector4i) == sizeof(FMOD_GUID),
                  "Vector4i and FMOD_GUID must be the same size for type punning");
    Vector4i v;
    memcpy(&v, &p_guid, sizeof(Vector4i));
    return v;
}

FMOD_GUID cast_to_fmod_guid(const Vector4i &p_guid)
{
    static_assert(sizeof(Vector4i) == sizeof(FMOD_GUID),
                  "Vector4i and FMOD_GUID must be the same size for type punning");
    FMOD_GUID eventguid;
    memcpy(&eventguid, &p_guid, sizeof(Vector4i));
    return eventguid;
}

static_assert(sizeof(Vector2i) == sizeof(FMOD_STUDIO_PARAMETER_ID));
FMOD_STUDIO_PARAMETER_ID cast_to_parameter_id(const Vector2i &p_id)
{
    static_assert(sizeof(FMOD_STUDIO_PARAMETER_ID) == sizeof(Vector2i),
                  "Vector2i and FMOD_STUDIO_PARAMETER_ID must be the same size for type punning");
    FMOD_STUDIO_PARAMETER_ID id;
    memcpy(&id, &p_id, sizeof(FMOD_STUDIO_PARAMETER_ID));
    return id;
}

Vector2i cast_to_vector2i(const FMOD_STUDIO_PARAMETER_ID &p_id)
{
    static_assert(sizeof(FMOD_STUDIO_PARAMETER_ID) == sizeof(Vector2i),
                  "Vector2i and FMOD_STUDIO_PARAMETER_ID must be the same size for type punning");
    Vector2i id;
    memcpy(&id, &p_id, sizeof(Vector2i));
    return id;
}
#pragma GCC diagnostic pop
FMOD_GUID string_to_fmod_guid(const char *guid)
{
    FMOD_GUID result;
    sscanf(guid, "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}", &result.Data1, &result.Data2,
           &result.Data3, &result.Data4[0], &result.Data4[1], &result.Data4[2], &result.Data4[3], &result.Data4[4],
           &result.Data4[5], &result.Data4[6], &result.Data4[7]);
    return result;
}
String fmod_guid_to_string(const FMOD_GUID &guid)
{
    return vformat("{%08ux-%04ux-%04ux-%02ux%02ux-%02ux%02ux%02ux%02ux%02ux%02ux}", guid.Data1, guid.Data2, guid.Data3,
                   guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
                   guid.Data4[6], guid.Data4[7]);
}
String fmod_guid_to_string(const Vector4i &guid)
{
    return fmod_guid_to_string(cast_to_fmod_guid(guid));
}
/// @brief converts the godot vector into an fmod vector.
/// /// @param vec the godot vector to convert
/// @return
FMOD_VECTOR to_fmod_vector(godot::Vector3 vec)
{
    FMOD_VECTOR temp;
    temp.x = vec.x;
    temp.y = vec.y;
    temp.z = vec.z;

    return temp;
}
/// @brief converts the godot vector into an fmod vector.// @param vec the godot vector to convert
/// @return
FMOD_VECTOR to_fmod_vector(godot::Vector2 vec)
{
    return {vec.x, vec.y, 0};
}

/// @brief converts the fmod vector into an godot vector.
/// @param vec the fmod vector to convert
/// @return
godot::Vector3 to_godot_vector(FMOD_VECTOR vec)
{
    return godot::Vector3(vec.x, vec.y, vec.z);
}

FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector3 pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(pos);

    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node3D *node)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(node->get_global_position());
    attributes.forward = to_fmod_vector(-node->get_global_transform().get_basis().get_column(2));
    attributes.up = to_fmod_vector(node->get_global_transform().get_basis().get_column(1));
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody3D *rigidbody)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(rigidbody->get_global_position());
    attributes.forward = to_fmod_vector(-rigidbody->get_global_transform().get_basis().get_column(2));
    attributes.up = to_fmod_vector(rigidbody->get_global_transform().get_basis().get_column(1));
    attributes.velocity = to_fmod_vector(rigidbody->get_linear_velocity());
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector2 pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(pos);
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node2D *node)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(node->get_global_position());
    attributes.forward = to_fmod_vector(node->get_global_transform().columns[0]);
    attributes.up = to_fmod_vector(node->get_global_transform().columns[1]);
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody2D *rigidbody)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(rigidbody->get_global_position());
    attributes.forward = to_fmod_vector(rigidbody->get_global_transform().columns[0]);
    attributes.up = to_fmod_vector(rigidbody->get_global_transform().columns[1]);
    attributes.velocity = to_fmod_vector(rigidbody->get_linear_velocity());
    return attributes;
}
} // namespace FmodGodot
Variant _GLOBAL_DEF(const String &p_var, const Variant &p_default, bool p_restart_if_changed,
                    bool p_ignore_value_in_docs, bool p_basic, bool p_internal)
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
