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
    FMOD_RESULT F_API set3DAttributes(const FMOD_VECTOR *p_position, float p_mindistance, float p_maxdistance);
    FMOD_RESULT F_API get3DAttributes(FMOD_VECTOR *p_position, float *p_mindistance, float *p_maxdistance);
    FMOD_RESULT F_API setProperties(const FMOD_REVERB_PROPERTIES *p_properties);
    FMOD_RESULT F_API getProperties(FMOD_REVERB_PROPERTIES *p_properties);
    FMOD_RESULT F_API setActive(bool p_active);
    FMOD_RESULT F_API getActive(bool *p_active);

    // Userdata set/get.
    FMOD_RESULT F_API setUserData(void *p_userdata);
    FMOD_RESULT F_API getUserData(void **p_userdata);
};
