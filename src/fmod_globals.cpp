#include <godot_cpp/variant/vector4i.hpp>
#include "fmod_globals.h"

#include <stdio.h>
#include <iostream>
#include <godot_cpp/variant/string.hpp>
#include <fmod.h>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
using namespace godot;
namespace FmodGodot
{

    char* to_char_ptr(const String& str){
        return str.utf8().ptrw();
    }

    Vector4i cast_to_Vector4i(const FMOD_GUID &guid)
    {
        Vector4i v;
        v.x = guid.Data1;
        // For some reason unclear in fmod api docs must be reversed
        // now that i think about it probably something to do with endianess
        v.y = guid.Data3 << 16 | guid.Data2;
        unsigned int *ptr = (unsigned int *)guid.Data4;
        v.z = ptr[0];
        v.w = ptr[1];
        return v;
    }

    FMOD_GUID cast_to_FMOD_GUID(const Vector4i &v_guid)
    {
        FMOD_GUID eventguid;
        eventguid.Data1 = v_guid.x;
        eventguid.Data2 = (unsigned short)v_guid.y;
        eventguid.Data3 = (unsigned short)(v_guid.y >> 16);
        unsigned int *ptr = (unsigned int *)eventguid.Data4;
        ptr[0] = v_guid.z;
        ptr[1] = v_guid.w;
        return eventguid;
    }
    FMOD_GUID string_to_fmod_guid(const char *guid)
    {
        FMOD_GUID result;
        sscanf(guid,
               "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
               &result.Data1, &result.Data2, &result.Data3,
               &result.Data4[0], &result.Data4[1], &result.Data4[2], &result.Data4[3],
               &result.Data4[4], &result.Data4[5], &result.Data4[6], &result.Data4[7]);
        return result;
    }
    String fmod_guid_to_string(const FMOD_GUID &guid)
    {
        char result[39];
        snprintf(result, sizeof(result), "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                 guid.Data1, guid.Data2, guid.Data3,
                 guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                 guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        return result;
    }
    String fmod_guid_to_string(const Vector4i &guid)
    {
        return fmod_guid_to_string(cast_to_FMOD_GUID(guid));
    }
    /// @brief converts the godot vector into an fmod vector. Transforming it into the fmod vector space (negates the Y coordinate)
    /// @param vec the godot vector to convert
    /// @return 
    FMOD_VECTOR to_fmod_vector(godot::Vector3 vec)
    {
        FMOD_VECTOR temp;
        temp.x = vec.x;
        temp.y = -vec.y;
        temp.z = vec.z;

        return temp;
    }
    /// @brief converts the godot vector into an fmod vector. Transforming it into the fmod vector space (negates the Y coordinate)
    /// @param vec the godot vector to convert
    /// @return
    FMOD_VECTOR to_fmod_vector(godot::Vector2 vec)
    {
        return {vec.x, -vec.y, 0};
    }

    /// @brief converts the fmod vector into an godot vector. Transforming it into the godot vector space (negates the Y coordinate)
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
}