#include "fmod.h"
#include "fmod_common.h"
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
    FMOD_RESULT F_API addPolygon(float p_directocclusion, float p_reverbocclusion, bool p_doublesided,
                                 int p_numvertices, const FMOD_VECTOR *p_vertices, int *p_polygonindex);
    FMOD_RESULT F_API getNumPolygons(int *p_numpolygons);
    FMOD_RESULT F_API getMaxPolygons(int *p_maxpolygons, int *p_maxvertices);
    FMOD_RESULT F_API getPolygonNumVertices(int p_index, int *p_numvertices);
    FMOD_RESULT F_API setPolygonVertex(int p_index, int p_vertexindex, const FMOD_VECTOR *p_vertex);
    FMOD_RESULT F_API getPolygonVertex(int p_index, int p_vertexindex, FMOD_VECTOR *p_vertex);
    FMOD_RESULT F_API setPolygonAttributes(int p_index, float p_directocclusion, float p_reverbocclusion,
                                           bool p_doublesided);
    FMOD_RESULT F_API getPolygonAttributes(int p_index, float *p_directocclusion, float *p_reverbocclusion,
                                           bool *p_doublesided);

    // Object manipulation.
    FMOD_RESULT F_API setActive(bool p_active);
    FMOD_RESULT F_API getActive(bool *p_active);
    FMOD_RESULT F_API setRotation(const FMOD_VECTOR *p_forward, const FMOD_VECTOR *p_up);
    FMOD_RESULT F_API getRotation(FMOD_VECTOR *p_forward, FMOD_VECTOR *p_up);
    FMOD_RESULT F_API setPosition(const FMOD_VECTOR *p_position);
    FMOD_RESULT F_API getPosition(FMOD_VECTOR *p_position);
    FMOD_RESULT F_API setScale(const FMOD_VECTOR *p_scale);
    FMOD_RESULT F_API getScale(FMOD_VECTOR *p_scale);
    FMOD_RESULT F_API save(void *p_data, int *p_datasize);

    // Userdata set/get.
    FMOD_RESULT F_API setUserData(void *p_userdata);
    FMOD_RESULT F_API getUserData(void **p_userdata);
};
