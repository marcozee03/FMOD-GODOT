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
        memdelete_arr(char_ptr);                                                                                       \
        size = retrieved;                                                                                              \
        char_ptr = memnew_arr(char, size);                                                                             \
        function(object, char_ptr, size, &retrieved);                                                                  \
    }
#define FMOD_LOOKUP_FULL_STRING(function, object, query, char_ptr, size, retrieved)                                    \
    if (function(object, query, char_ptr, size, &retrieved) == FMOD_ERR_TRUNCATED)                                     \
    {                                                                                                                  \
        memdelete_arr(char_ptr);                                                                                       \
        size = retrieved;                                                                                              \
        char_ptr = memnew_arr(char, size);                                                                             \
        function(object, query, char_ptr, size, &retrieved);                                                           \
    }

#define FMOD_GET_FULL_STRING_FUNC_CALL(function_call, char_ptr, size, retrieved)                                       \
    if (function_call == FMOD_ERR_TRUNCATED)                                                                           \
    {                                                                                                                  \
        memdelete_arr(char_ptr);                                                                                       \
        size = retrieved;                                                                                              \
        char_ptr = memnew_arr(char, size);                                                                             \
        function_call;                                                                                                 \
    }
#define GLOBAL_GET(m_var) ProjectSettings::get_singleton()->get_setting_with_override(m_var)

#define GLOBAL_DEF(m_var, m_value) _GLOBAL_DEF(m_var, m_value)
#define GLOBAL_DEF_RST(m_var, m_value) _GLOBAL_DEF(m_var, m_value, true)
#define GLOBAL_GET(m_var) ProjectSettings::get_singleton()->get_setting_with_override(m_var)

#define GLOBAL_DEF_BASIC(m_var, m_value) _GLOBAL_DEF(m_var, m_value, false, true)
#define GLOBAL_DEF_RST_BASIC(m_var, m_value) _GLOBAL_DEF(m_var, m_value, true, true)

#define GLOBAL_DEF_INTERNAL(m_var, m_value) _GLOBAL_DEF(m_var, m_value, false, false, true)
} // namespace FmodGodot
Variant _GLOBAL_DEF(const String &p_var, const Variant &p_default, bool p_restart_if_changed = false,
                    bool p_ignore_value_in_docs = false, bool p_basic = false, bool p_internal = false);


Variant _GLOBAL_DEF(const PropertyInfo &p_info, const Variant &p_default, bool p_restart_if_changed = false,
                    bool p_basic = false, bool p_internal = false);
