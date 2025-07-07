#pragma once
#include <godot_cpp/variant/vector4i.hpp>

#include <stdio.h>
#include <iostream>
#include <godot_cpp/variant/string.hpp>
#include <fmod.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
using namespace godot;
namespace FmodGodot
{
    Vector4i cast_to_Vector4i(const FMOD_GUID &guid);

    FMOD_GUID cast_to_FMOD_GUID(const Vector4i &v_guid);
    FMOD_GUID string_to_fmod_guid(const char *guid);
    String fmod_guid_to_string(const FMOD_GUID &guid);
    String fmod_guid_to_string(const Vector4i &guid);
    FMOD_VECTOR to_fmod_vector(godot::Vector3 vec);

    FMOD_VECTOR to_fmod_vector(godot::Vector2 vec);
    godot::Vector3 to_godot_vector(FMOD_VECTOR vec);

    FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector3 pos);

    FMOD_3D_ATTRIBUTES to_3d_attributes(Node3D *node);
    FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody3D *rigidbody);
    FMOD_3D_ATTRIBUTES to_3d_attributes(godot::Vector2 pos);
    FMOD_3D_ATTRIBUTES to_3d_attributes(Node2D *node);
    FMOD_3D_ATTRIBUTES to_3d_attributes(RigidBody2D *rigidbody);
}