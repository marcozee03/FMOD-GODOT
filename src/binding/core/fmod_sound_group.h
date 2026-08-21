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

class SoundGroup
{
  private:
    // Constructor made private so user cannot statically instance a SoundGroup class.  Appropriate SoundGroup creation
    // or retrieval function must be used.
    SoundGroup();
    SoundGroup(const SoundGroup &);

  public:
    FMOD_RESULT  release();
    FMOD_RESULT getSystemObject(System **p_system);

    // SoundGroup control functions.
    FMOD_RESULT setMaxAudible(int p_maxaudible);
    FMOD_RESULT getMaxAudible(int *p_maxaudible);
    FMOD_RESULT setMaxAudibleBehavior(FMOD_SOUNDGROUP_BEHAVIOR p_behavior);
    FMOD_RESULT getMaxAudibleBehavior(FMOD_SOUNDGROUP_BEHAVIOR *p_behavior);
    FMOD_RESULT setMuteFadeSpeed(float p_speed);
    FMOD_RESULT getMuteFadeSpeed(float *p_speed);
    FMOD_RESULT setVolume(float p_volume);
    FMOD_RESULT getVolume(float *p_volume);
    FMOD_RESULT  stop();

    // Information only functions.
    FMOD_RESULT getName(char *p_name, int p_namelen);
    FMOD_RESULT getNumSounds(int *p_numsounds);
    FMOD_RESULT getSound(int p_index, Sound **p_sound);
    FMOD_RESULT getNumPlaying(int *p_numplaying);

    // Userdata set/get.
    FMOD_RESULT setUserData(void *p_userdata);
    FMOD_RESULT getUserData(void **p_userdata);
};
