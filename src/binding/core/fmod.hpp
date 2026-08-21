/* ======================================================================================== */
/* FMOD Core API - C++ header file.                                                         */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2026.                               */
/*                                                                                          */
/* Use this header in conjunction with fmod_common.h (which contains all the constants /    */
/* callbacks) to develop using the C++ language.                                            */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.03/api/core-api.html                                             */
/* ======================================================================================== */
#ifndef _FMOD_HPP
#define _FMOD_HPP

#include "fmod.h"
#include "fmod_common.h"

/*
    FMOD Namespace
*/
namespace FMOD
{
class System;
class Sound;
class ChannelControl;
class Channel;
class ChannelGroup;
class SoundGroup;
class DSP;
class DSPConnection;
class Geometry;
class Reverb3D;

/*
    FMOD global system functions (optional).
*/
inline FMOD_RESULT Memory_Initialize(void *poolmem, int poollen, FMOD_MEMORY_ALLOC_CALLBACK useralloc,
                                     FMOD_MEMORY_REALLOC_CALLBACK userrealloc, FMOD_MEMORY_FREE_CALLBACK userfree,
                                     FMOD_MEMORY_TYPE memtypeflags = FMOD_MEMORY_ALL)
{
    return FMOD_Memory_Initialize(poolmem, poollen, useralloc, userrealloc, userfree, memtypeflags);
}
inline FMOD_RESULT Memory_GetStats(int *currentalloced, int *maxalloced, bool blocking = true)
{
    return FMOD_Memory_GetStats(currentalloced, maxalloced, blocking);
}
inline FMOD_RESULT Debug_Initialize(FMOD_DEBUG_FLAGS flags, FMOD_DEBUG_MODE mode = FMOD_DEBUG_MODE_TTY,
                                    FMOD_DEBUG_CALLBACK callback = 0, const char *filename = 0)
{
    return FMOD_Debug_Initialize(flags, mode, callback, filename);
}
inline FMOD_RESULT File_SetDiskBusy(int busy)
{
    return FMOD_File_SetDiskBusy(busy);
}
inline FMOD_RESULT File_GetDiskBusy(int *busy)
{
    return FMOD_File_GetDiskBusy(busy);
}
inline FMOD_RESULT Thread_SetAttributes(FMOD_THREAD_TYPE type,
                                        FMOD_THREAD_AFFINITY affinity = FMOD_THREAD_AFFINITY_GROUP_DEFAULT,
                                        FMOD_THREAD_PRIORITY priority = FMOD_THREAD_PRIORITY_DEFAULT,
                                        FMOD_THREAD_STACK_SIZE stacksize = FMOD_THREAD_STACK_SIZE_DEFAULT)
{
    return FMOD_Thread_SetAttributes(type, affinity, priority, stacksize);
}

/*
    FMOD System factory functions.
*/
inline FMOD_RESULT System_Create(System **system, unsigned int headerversion = FMOD_VERSION)
{
    return FMOD_System_Create((FMOD_SYSTEM **)system, headerversion);
}

/*
    'Sound' API
*/


/*
    'ChannelControl API'.   This is a base class for Channel and ChannelGroup so they can share the same functionality.
   This cannot be used or instansiated explicitly.
*/


/*
    'Channel' API.
*/


/*
    'ChannelGroup' API
*/


/*
    'SoundGroup' API
*/


/*
    'DSP' API
*/


/*
    'DSPConnection' API
*/


/*
    'Geometry' API
*/
class Geometry
{
  private:
    // Constructor made private so user cannot statically instance a Geometry class.  Appropriate Geometry creation or
    // retrieval function must be used.
    Geometry();
    Geometry(const Geometry &);

  public:
    FMOD_RESULT F_API release();

    // Polygon manipulation.
    FMOD_RESULT F_API addPolygon(float directocclusion, float reverbocclusion, bool doublesided, int numvertices,
                                 const FMOD_VECTOR *vertices, int *polygonindex);
    FMOD_RESULT F_API getNumPolygons(int *numpolygons);
    FMOD_RESULT F_API getMaxPolygons(int *maxpolygons, int *maxvertices);
    FMOD_RESULT F_API getPolygonNumVertices(int index, int *numvertices);
    FMOD_RESULT F_API setPolygonVertex(int index, int vertexindex, const FMOD_VECTOR *vertex);
    FMOD_RESULT F_API getPolygonVertex(int index, int vertexindex, FMOD_VECTOR *vertex);
    FMOD_RESULT F_API setPolygonAttributes(int index, float directocclusion, float reverbocclusion, bool doublesided);
    FMOD_RESULT F_API getPolygonAttributes(int index, float *directocclusion, float *reverbocclusion,
                                           bool *doublesided);

    // Object manipulation.
    FMOD_RESULT F_API setActive(bool active);
    FMOD_RESULT F_API getActive(bool *active);
    FMOD_RESULT F_API setRotation(const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
    FMOD_RESULT F_API getRotation(FMOD_VECTOR *forward, FMOD_VECTOR *up);
    FMOD_RESULT F_API setPosition(const FMOD_VECTOR *position);
    FMOD_RESULT F_API getPosition(FMOD_VECTOR *position);
    FMOD_RESULT F_API setScale(const FMOD_VECTOR *scale);
    FMOD_RESULT F_API getScale(FMOD_VECTOR *scale);
    FMOD_RESULT F_API save(void *data, int *datasize);

    // Userdata set/get.
    FMOD_RESULT F_API setUserData(void *userdata);
    FMOD_RESULT F_API getUserData(void **userdata);
};

/*
    'Reverb' API
*/
class Reverb3D
{
  private:
    // Constructor made private so user cannot statically instance a Reverb3D class.  Appropriate Reverb creation or
    // retrieval function must be used.
    Reverb3D();
    Reverb3D(const Reverb3D &);

  public:
    FMOD_RESULT F_API release();

    // Reverb manipulation.
    FMOD_RESULT F_API set3DAttributes(const FMOD_VECTOR *position, float mindistance, float maxdistance);
    FMOD_RESULT F_API get3DAttributes(FMOD_VECTOR *position, float *mindistance, float *maxdistance);
    FMOD_RESULT F_API setProperties(const FMOD_REVERB_PROPERTIES *properties);
    FMOD_RESULT F_API getProperties(FMOD_REVERB_PROPERTIES *properties);
    FMOD_RESULT F_API setActive(bool active);
    FMOD_RESULT F_API getActive(bool *active);

    // Userdata set/get.
    FMOD_RESULT F_API setUserData(void *userdata);
    FMOD_RESULT F_API getUserData(void **userdata);
};
} // namespace FMOD

#endif
