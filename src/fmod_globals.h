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
#define FMOD_GET_OUT_CSTRING(function, object, variable_name)                                                          \
    int retrieved = 0;                                                                                                 \
    if (function(object, nullptr, 0, &retrieved) == FMOD_ERR_TRUNCATED)                                                \
    {                                                                                                                  \
        int size = retrieved;                                                                                          \
        variable_name = memnew_arr(char, size);                                                                        \
        function(object, variable_name, size, &retrieved);                                                             \
    }

#define FMOD_GET_CSTRING(function, object, variable_name)                                                              \
    char *variable_name;                                                                                               \
    FMOD_GET_OUT_CSTRING(function, object, variable_name);

#define FMOD_GET_OUT_STRING(function, object, variable_name)                                                           \
    {                                                                                                                  \
        FMOD_GET_CSTRING(function, object, str##variable_name)                                                         \
        variable_name = String::utf8(str##variable_name, retrieved);                                                   \
        memdelete_arr(str##variable_name);                                                                             \
    }

#define FMOD_GET_STRING(function, object, variable_name)                                                               \
    String variable_name = "";                                                                                         \
    FMOD_GET_OUT_STRING(function, object, variable_name)

#define FMOD_LOOKUP_CSTRING(function, object, variable_name, ...)                                                      \
    char *variable_name;                                                                                               \
    int retrieved = 0;                                                                                                 \
    if (function(object __VA_OPT__(, ) __VA_ARGS__, nullptr, 0, &retrieved) == FMOD_ERR_TRUNCATED)                     \
    {                                                                                                                  \
        int size = retrieved;                                                                                          \
        variable_name = memnew_arr(char, size);                                                                        \
        function(object __VA_OPT__(, ) __VA_ARGS__, variable_name, size, &retrieved);                                  \
    }
#define FMOD_LOOKUP_STRING(function, object, variable_name, ...)                                                       \
    String variable_name = "";                                                                                         \
    {                                                                                                                  \
        FMOD_LOOKUP_CSTRING(function, object, str##variable_name __VA_OPT__(, ) __VA_ARGS__)                           \
        variable_name = String::utf8(str##variable_name, retrieved);                                                   \
        memdelete_arr(str##variable_name);                                                                             \
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
                    bool p_basic = false, bool p_internal = false);

Variant _GLOBAL_DEF(const PropertyInfo &p_info, const Variant &p_default, bool p_restart_if_changed = false,
                    bool p_basic = false, bool p_internal = false);
