#include "conversions.h"
#include "fmod_common.h"
#include "fmod_studio.h"
#include "variant/variant.hpp"
#include "variant/vector2i.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4i.hpp>

namespace FmodGodot
{

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

} // namespace FmodGodot
