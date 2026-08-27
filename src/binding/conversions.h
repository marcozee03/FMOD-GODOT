#pragma once

#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/vector4i.hpp"
#include <bit>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>

using namespace godot;
namespace FmodGodot
{
namespace
{
constexpr Vector3 get_column(const Basis &p_basis, int p_index)
{
    // Get actual basis axis column (we store transposed as rows for performance).
    return Vector3(p_basis.rows[0][p_index], p_basis.rows[1][p_index], p_basis.rows[2][p_index]);
}
constexpr real_t length_squared(const Vector3 &p_a)
{
    real_t x2 = p_a.x * p_a.x;
    real_t y2 = p_a.y * p_a.y;
    real_t z2 = p_a.z * p_a.z;

    return x2 + y2 + z2;
}

constexpr Vector3 cross(const Vector3 &p_a, const Vector3 &p_b)
{
    Vector3 ret((p_a.y * p_b.z) - (p_a.z * p_b.y), (p_a.z * p_b.x) - (p_a.x * p_b.z),
                (p_a.x * p_b.y) - (p_a.y * p_b.x));
    return ret;
}
constexpr Vector3 normalized(const Vector3 &p_a)
{
    if (!(Math::is_finite(p_a.x) && Math::is_finite(p_a.y) && Math::is_finite(p_a.z)))
    {
#ifdef MATH_CHECKS
        WARN_PRINT("Vector3 cannot be normalized, the elements must be finite. Making (0, 0, 0) as a fallback.");
#endif // MATH_CHECKS
        return Vector3(0, 0, 0);
    }

    real_t l = length_squared(p_a);
    if (l == 0)
    {
        return Vector3(0, 0, 0);
    }
    else
    {
        l = std::sqrt(l);
        return Vector3(p_a.x / l, p_a.y / l, p_a.z / l);
    }
}
} // namespace

constexpr Vector4i cast_to_vector4i(const FMOD_GUID &p_guid)
{
    return std::bit_cast<Vector4i>(p_guid);
}
constexpr FMOD_GUID cast_to_fmod_guid(const Vector4i &p_guid)
{
    return std::bit_cast<FMOD_GUID>(p_guid);
}
constexpr FMOD_STUDIO_PARAMETER_ID cast_to_parameter_id(GD_PARAMETER_ID p_id)
{
    return std::bit_cast<FMOD_STUDIO_PARAMETER_ID>(p_id);
}
constexpr GD_PARAMETER_ID cast_to_gd_parameter_id(const FMOD_STUDIO_PARAMETER_ID &p_id)
{
    return std::bit_cast<GD_PARAMETER_ID>(p_id);
}
FMOD_GUID string_to_fmod_guid(const char *p_guid);
String fmod_guid_to_string(const FMOD_GUID &p_guid);
String fmod_guid_to_string(const Vector4i &p_guid);
constexpr FMOD_VECTOR to_fmod_vector(godot::Vector3 p_vec)
{
    return {p_vec.x, p_vec.y, p_vec.z};
}

constexpr FMOD_VECTOR to_fmod_vector(godot::Vector2 p_vec)
{
    return {p_vec.x, p_vec.y, 0};
}
constexpr godot::Vector3 to_godot_vector(FMOD_VECTOR p_vec)
{
    return godot::Vector3(p_vec.x, p_vec.y, p_vec.z);
}
Variant to_variant(FMOD_STUDIO_USER_PROPERTY p_property);

constexpr FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform)
{
    FMOD_3D_ATTRIBUTES attr;
    attr.position = to_fmod_vector(p_transform.origin);
    attr.forward = to_fmod_vector(normalized(get_column(p_transform.basis, 2)));
    attr.up = to_fmod_vector(normalized(get_column(p_transform.basis, 1)));
    return attr;
}
constexpr FMOD_3D_ATTRIBUTES to_3d_attributes(const Transform3D &p_transform, const Vector3 &p_velocity)
{
    FMOD_3D_ATTRIBUTES attr = to_3d_attributes(p_transform);
    attr.velocity = to_fmod_vector(p_velocity);
    return attr;
}
constexpr FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector3 p_pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(p_pos);

    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node3D *p_node);
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody3D *p_rigidbody);
constexpr FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector2 p_pos)
{
    FMOD_3D_ATTRIBUTES attributes;
    attributes.forward = {0, 0, -1};
    attributes.up = {0, 1, 0};
    attributes.position = to_fmod_vector(p_pos);
    return attributes;
}
FMOD_3D_ATTRIBUTES to_3d_attributes(Node2D *p_node);
FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody2D *p_rigidbody);

constexpr Transform3D to_transform3d(const FMOD_3D_ATTRIBUTES &p_attr)
{
    const Vector3 position = FmodGodot::to_godot_vector(p_attr.position);
    const Vector3 up = FmodGodot::to_godot_vector(p_attr.up);
    const Vector3 forward = FmodGodot::to_godot_vector(p_attr.forward);
    return Transform3D(cross(up, forward), up, forward, position);
}

} // namespace FmodGodot
