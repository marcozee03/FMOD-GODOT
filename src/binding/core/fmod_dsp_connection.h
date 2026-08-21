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

class DSPConnection
{
  private:
    // Constructor made private so user cannot statically instance a DSPConnection class.  Appropriate DSPConnection
    // creation or retrieval function must be used.
    DSPConnection();
    DSPConnection(const DSPConnection &);

  public:
    FMOD_RESULT F_API getInput(DSP **p_input);
    FMOD_RESULT F_API getOutput(DSP **p_output);
    FMOD_RESULT F_API setMix(float p_volume);
    FMOD_RESULT F_API getMix(float *p_volume);
    FMOD_RESULT F_API setMixMatrix(float *p_matrix, int p_outchannels, int p_inchannels, int p_inchannel_hop = 0);
    FMOD_RESULT F_API getMixMatrix(float *p_matrix, int *p_outchannels, int *p_inchannels, int p_inchannel_hop = 0);
    FMOD_RESULT F_API getType(FMOD_DSPCONNECTION_TYPE *p_type);

    // Userdata set/get.
    FMOD_RESULT F_API setUserData(void *p_userdata);
    FMOD_RESULT F_API getUserData(void **p_userdata);
};
