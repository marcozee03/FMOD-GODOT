#pragma once

#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/vector4i.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>

using namespace godot;
namespace FmodGodot
{
Vector4i cast_to_vector4i(const FMOD_GUID &p_guid);
FMOD_GUID cast_to_fmod_guid(const Vector4i &p_guid);
FMOD_STUDIO_PARAMETER_ID cast_to_parameter_id(GD_PARAMETER_ID p_id);
GD_PARAMETER_ID cast_to_gd_parameter_id(const FMOD_STUDIO_PARAMETER_ID &p_id);
FMOD_GUID string_to_fmod_guid(const char *p_guid);
String fmod_guid_to_string(const FMOD_GUID &p_guid);
String fmod_guid_to_string(const Vector4i &p_guid);
FMOD_VECTOR to_fmod_vector(godot::Vector3 p_vec);

FMOD_VECTOR to_fmod_vector(godot::Vector2 p_vec);
godot::Vector3 to_godot_vector(FMOD_VECTOR p_vec);
Variant to_variant(FMOD_STUDIO_USER_PROPERTY p_property);

FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform);
FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform, const Vector3 &p_velocity);
FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector3 p_pos);

FMOD_3D_ATTRIBUTES to_3d_attributes(Node3D *p_node);
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody3D *p_rigidbody);
FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector2 p_pos);
FMOD_3D_ATTRIBUTES to_3d_attributes(Node2D *p_node);
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody2D *p_rigidbody);

Transform3D to_transform3d(const FMOD_3D_ATTRIBUTES &p_attr);

} // namespace FmodGodot
