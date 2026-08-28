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

#define FMOD_GET_OUT_CSTRING(function, object, variable_name)                                                          \
    int retrieved = 0;                                                                                                 \
    function(object, nullptr, 0, &retrieved);                                                                          \
    if (retrieved > 0)                                                                                                 \
    {                                                                                                                  \
        int size = retrieved;                                                                                          \
        variable_name = memnew_arr(char, size);                                                                        \
        function(object, variable_name, size, &retrieved);                                                             \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        variable_name = nullptr;                                                                                       \
    }

#define FMOD_GET_CSTRING(function, object, variable_name)                                                              \
    char *variable_name = nullptr;                                                                                     \
    FMOD_GET_OUT_CSTRING(function, object, variable_name);

#define FMOD_GET_OUT_CHARSTRING(function, object, variable_name)                                                       \
    int retrieved = 0;                                                                                                 \
    function(object, nullptr, 0, &retrieved);                                                                          \
    if (retrieved > 0)                                                                                                 \
    {                                                                                                                  \
        int size = retrieved;                                                                                          \
        variable_name.resize_uninitialized(size);                                                                      \
        function(object, variable_name.ptrw(), size, &retrieved);                                                      \
    }

#define FMOD_GET_CHARSTRING(function, object, variable_name)                                                           \
    CharString variable_name;                                                                                          \
    FMOD_GET_OUT_CHARSTRING(function, object, variable_name);

#define FMOD_GET_OUT_STRING(function, object, variable_name)                                                           \
    {                                                                                                                  \
        FMOD_GET_CHARSTRING(function, object, str##variable_name);                                                     \
        variable_name = String(str##variable_name.ptr());                                                              \
    }

#define FMOD_GET_STRING(function, object, variable_name)                                                               \
    String variable_name = "";                                                                                         \
    FMOD_GET_OUT_STRING(function, object, variable_name)

#define FMOD_LOOKUP_CSTRING(function, object, variable_name, ...)                                                      \
    char *variable_name = nullptr;                                                                                     \
    int retrieved = 0;                                                                                                 \
    function(object __VA_OPT__(, ) __VA_ARGS__, nullptr, 0, &retrieved);                                               \
    if (retrieved > 0)                                                                                                 \
    {                                                                                                                  \
        int size = retrieved;                                                                                          \
        variable_name = memnew_arr(char, size);                                                                        \
        function(object __VA_OPT__(, ) __VA_ARGS__, variable_name, size, &retrieved);                                  \
    }
#define FMOD_LOOKUP_STRING(function, object, variable_name, ...)                                                       \
    String variable_name = "";                                                                                         \
    {                                                                                                                  \
        FMOD_LOOKUP_CSTRING(function, object, str##variable_name __VA_OPT__(, ) __VA_ARGS__)                           \
        if (str##variable_name != nullptr)                                                                             \
        {                                                                                                              \
            variable_name = String::utf8(str##variable_name, retrieved);                                               \
            memdelete_arr(str##variable_name);                                                                         \
        }                                                                                                              \
    }

#define FMOD_ERR_FAIL_MSG(m_result, m_msg)                                                                             \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_MSG(err != FMOD_OK, vformat("Result: %s %s", FMOD_ErrorString(err), m_msg));                     \
    }
#define FMOD_ERR_FAIL_RETERR_MSG(m_result, m_msg)                                                                      \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_V_MSG(m_result != FMOD_OK, err, vformat("Result: %s %s", FMOD_ErrorString(err), m_msg));         \
    }

#define FMOD_ERR_FAIL(m_result)                                                                                        \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_MSG(err != FMOD_OK, FMOD_ErrorString(err));                                                      \
    }
#define FMOD_ERR_FAIL_V(m_result, m_retval)                                                                            \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_V_MSG(err != FMOD_OK, m_retval, FMOD_ErrorString(err));                                          \
    }
#define FMOD_ERR_FAIL_VE(m_result)                                                                                     \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_V_MSG(err != FMOD_OK, err, FMOD_ErrorString(err));                                               \
    }

#define FMOD_ERR_FAIL_V_MSG(m_result, m_retval, m_msg)                                                                 \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_V_MSG(err != FMOD_OK, m_retval, vformat("Result: %s %s", FMOD_ErrorString(err), m_msg));         \
    }
#define FMOD_ERR_FAIL_VE_MSG(m_result, m_msg)                                                                          \
    {                                                                                                                  \
        FMOD_RESULT err = m_result;                                                                                    \
        ERR_FAIL_COND_V_MSG(err != FMOD_OK, err, vformat("Result: %s %s", FMOD_ErrorString(err), m_msg));              \
    }

typedef size_t Handle;
typedef uint64_t GD_PARAMETER_ID;
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
