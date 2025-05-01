#include <fmod.h>
#include <fmod_studio.h>
#include <godot_cpp/variant/vector4i.hpp>
#include <stdio.h>
#include <iostream>
using namespace godot;
namespace FMODGodot
{
    static Vector4i cast_to_Vector4i(const FMOD_GUID &guid)
    {
        Vector4i v;
        // int *p = (int *)&guid;
        // v.x = p[0];
        // v.y = p[1];
        // v.z = p[2];
        // v.w = p[3];
        v.x = guid.Data1;
        // For some reason unclear in fmod api docs must be reversed
        // now that i think about it probably something to do with endianess
        v.y = guid.Data3 << 16 | guid.Data2;
        unsigned int *ptr = (unsigned int *)guid.Data4;
        v.z = ptr[0];
        v.w = ptr[1];
        return v;
    }

    static FMOD_GUID cast_to_FMOD_GUID(const Vector4i &v_guid)
    {
        FMOD_GUID eventguid;
        eventguid.Data1 = v_guid.x;
        eventguid.Data2 = (unsigned short) v_guid.y;
        eventguid.Data3 = (unsigned short) (v_guid.y >> 16);
        unsigned int *ptr = (unsigned int *)eventguid.Data4;
        ptr[0] = v_guid.z;
        ptr[1] = v_guid.w;
        return eventguid;
    }

    static inline FMOD_GUID string_to_fmod_guid(const char *guid)
    {
        FMOD_GUID result;
        sscanf(guid,
               "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
               &result.Data1, &result.Data2, &result.Data3,
               &result.Data4[0], &result.Data4[1], &result.Data4[2], &result.Data4[3],
               &result.Data4[4], &result.Data4[5], &result.Data4[6], &result.Data4[7]);
        return result;
    }

    static inline String fmod_guid_to_string(const FMOD_GUID &guid)
    {
        char result[39];
        snprintf(result, sizeof(result), "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                 guid.Data1, guid.Data2, guid.Data3,
                 guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                 guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        return result;
    }
    static inline String fmod_guid_to_string(const Vector4i &guid)
    {
        return fmod_guid_to_string(cast_to_FMOD_GUID(guid));
    }
}