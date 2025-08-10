#pragma once
#include <godot_cpp/variant/vector4i.hpp>

#include <fmod.h>
#include <fmod_studio.h>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <iostream>
#include <stdio.h>
using namespace godot;
namespace FmodGodot
{

char *to_char_ptr(const String &p_str);
Vector4i cast_to_vector4i(const FMOD_GUID &p_guid);
FMOD_GUID cast_to_fmod_guid(const Vector4i &p_guid);
FMOD_STUDIO_PARAMETER_ID cast_to_parameter_id(const Vector2i &p_id);
Vector2i cast_to_vector2i(const FMOD_STUDIO_PARAMETER_ID &p_id);
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

#define FMOD_GET_FULL_STRING(function, object, char_ptr, size, retrieved)                                              \
    if (function(object, char_ptr, size, &retrieved) == FMOD_ERR_TRUNCATED)                                            \
    {                                                                                                                  \
        delete[] char_ptr;                                                                                             \
        size = retrieved;                                                                                              \
        char_ptr = new char[size];                                                                                     \
        function(object, char_ptr, size, &retrieved);                                                                  \
    }
#define FMOD_LOOKUP_FULL_STRING(function, object, query, char_ptr, size, retrieved)                                    \
    if (function(object, query, char_ptr, size, &retrieved) == FMOD_ERR_TRUNCATED)                                     \
    {                                                                                                                  \
        delete[] char_ptr;                                                                                             \
        size = retrieved;                                                                                              \
        char_ptr = new char[size];                                                                                     \
        function(object, query, char_ptr, size, &retrieved);                                                           \
    }

#define FMOD_GET_FULL_STRING_FUNC_CALL(function_call, char_ptr, size, retrieved)                                       \
    if (function_call == FMOD_ERR_TRUNCATED)                                                                           \
    {                                                                                                                  \
        delete[] char_ptr;                                                                                             \
        size = retrieved;                                                                                              \
        char_ptr = new char[size];                                                                                     \
        function_call;                                                                                                 \
    }
} // namespace FmodGodot
