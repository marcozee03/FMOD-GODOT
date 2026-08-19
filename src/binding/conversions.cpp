#include "conversions.h"
#include "fmod_common.h"
#include "fmod_studio.h"
#include "variant/variant.hpp"
#include "variant/vector2i.hpp"
#include <cstring>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4i.hpp>

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
//

Vector4i parse_guid(const String &p_id)
{
    FMOD_GUID guid;
    FMOD_Studio_ParseID(p_id.utf8().ptr(), &guid);
    return cast_to_vector4i(guid);
}
String fmod_guid_to_string(const FMOD_GUID &p_guid)
{
    return vformat("{%08ux-%04ux-%04ux-%02ux%02ux-%02ux%02ux%02ux%02ux%02ux%02ux}", p_guid.Data1, p_guid.Data2,
                   p_guid.Data3, p_guid.Data4[0], p_guid.Data4[1], p_guid.Data4[2], p_guid.Data4[3], p_guid.Data4[4],
                   p_guid.Data4[5], p_guid.Data4[6], p_guid.Data4[7]);
}
String fmod_guid_to_string(const Vector4i &p_guid)
{
    return fmod_guid_to_string(cast_to_fmod_guid(p_guid));
}
/// @brief converts the godot vector into an fmod vector.
/// /// @param vec the godot vector to convert
/// @return
FMOD_VECTOR to_fmod_vector(godot::Vector3 p_vec)
{
    FMOD_VECTOR temp;
    temp.x = p_vec.x;
    temp.y = p_vec.y;
    temp.z = p_vec.z;

    return temp;
}
/// @brief converts the godot vector into an fmod vector.// @param vec the godot vector to convert
/// @return
FMOD_VECTOR to_fmod_vector(godot::Vector2 p_vec)
{
    return {p_vec.x, p_vec.y, 0};
}

/// @brief converts the fmod vector into an godot vector.
/// @param vec the fmod vector to convert
/// @return
godot::Vector3 to_godot_vector(FMOD_VECTOR p_vec)
{
    return godot::Vector3(p_vec.x, p_vec.y, p_vec.z);
}
FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform)
{
    FMOD_3D_ATTRIBUTES attr;
    attr.position = to_fmod_vector(p_transform.get_origin());
    attr.forward = to_fmod_vector(p_transform.basis.get_column(2).normalized());
    attr.up = to_fmod_vector(p_transform.basis.get_column(1).normalized());
    return attr;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform, const Vector3 &p_velocity)
{
    FMOD_3D_ATTRIBUTES attr = to_3d_attributes(p_transform);
    attr.velocity = to_fmod_vector(p_velocity);
    return attr;
}

FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector3 p_pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(p_pos);

    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node3D *p_node)
{
    return to_3d_attributes(p_node->get_global_transform());
}
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody3D *p_rigidbody)
{
    FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(p_rigidbody->get_global_transform());
    attributes.velocity = to_fmod_vector(p_rigidbody->get_linear_velocity());
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector2 p_pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(p_pos);
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node2D *p_node)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(p_node->get_global_position());
    attributes.forward = to_fmod_vector(p_node->get_global_transform().columns[0]);
    attributes.up = to_fmod_vector(p_node->get_global_transform().columns[1]);
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody2D *p_rigidbody)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.position = to_fmod_vector(p_rigidbody->get_global_position());
    attributes.forward = to_fmod_vector(p_rigidbody->get_global_transform().columns[0]);
    attributes.up = to_fmod_vector(p_rigidbody->get_global_transform().columns[1]);
    attributes.velocity = to_fmod_vector(p_rigidbody->get_linear_velocity());
    return attributes;
}
Variant to_variant(FMOD_STUDIO_USER_PROPERTY p_property)
{
    switch (p_property.type)
    {
    case FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER:
        return p_property.intvalue;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN:
        return p_property.stringvalue;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT:
        return p_property.floatvalue;
    case FMOD_STUDIO_USER_PROPERTY_TYPE_STRING:
        return p_property.stringvalue;
    default:
        return Variant();
        break;
    }
}
