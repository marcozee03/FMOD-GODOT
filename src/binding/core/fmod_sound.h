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

class Sound
{
  private:
    // Constructor made private so user cannot statically instance a Sound class.  Appropriate Sound creation or
    // retrieval function must be used.
    Sound();
    Sound(const Sound &);

  public:
    FMOD_RESULT release();
    FMOD_RESULT getSystemObject(System **p_system);

    // Standard sound manipulation functions.
    FMOD_RESULT lock(unsigned int p_offset, unsigned int p_length, void **p_ptr1, void **p_ptr2, unsigned int *p_len1,
                     unsigned int *p_len2);
    FMOD_RESULT unlock(void *p_ptr1, void *p_ptr2, unsigned int p_len1, unsigned int p_len2);
    FMOD_RESULT setDefaults(float p_frequency, int p_priority);
    FMOD_RESULT getDefaults(float *p_frequency, int *p_priority);
    FMOD_RESULT set3DMinMaxDistance(float p_min, float p_max);
    FMOD_RESULT get3DMinMaxDistance(float *p_min, float *p_max);
    FMOD_RESULT set3DConeSettings(float p_insideconeangle, float p_outsideconeangle, float p_outsidevolume);
    FMOD_RESULT get3DConeSettings(float *p_insideconeangle, float *p_outsideconeangle, float *p_outsidevolume);
    FMOD_RESULT set3DCustomRolloff(FMOD_VECTOR *p_points, int p_numpoints);
    FMOD_RESULT get3DCustomRolloff(FMOD_VECTOR **p_points, int *p_numpoints);
    FMOD_RESULT getSubSound(int p_index, Sound **p_subsound);
    FMOD_RESULT getSubSoundParent(Sound **p_parentsound);
    FMOD_RESULT getName(char *p_name, int p_namelen);
    FMOD_RESULT getLength(unsigned int *p_length, FMOD_TIMEUNIT p_lengthtype);
    FMOD_RESULT getFormat(FMOD_SOUND_TYPE *p_type, FMOD_SOUND_FORMAT *p_format, int *p_channels, int *p_bits);
    FMOD_RESULT getNumSubSounds(int *p_numsubsounds);
    FMOD_RESULT getNumTags(int *p_numtags, int *p_numtagsupdated);
    FMOD_RESULT getTag(const char *p_name, int p_index, FMOD_TAG *p_tag);
    FMOD_RESULT getOpenState(FMOD_OPENSTATE *p_openstate, unsigned int *p_percentbuffered, bool *p_starving,
                             bool *p_diskbusy);
    FMOD_RESULT readData(void *p_buffer, unsigned int p_length, unsigned int *p_read);
    FMOD_RESULT seekData(unsigned int p_pcm);

    FMOD_RESULT setSoundGroup(SoundGroup *p_soundgroup);
    FMOD_RESULT getSoundGroup(SoundGroup **p_soundgroup);

    // Synchronization point API.  These points can come from markers embedded in wav files, and can also generate
    // channel callbacks.
    FMOD_RESULT getNumSyncPoints(int *p_numsyncpoints);
    FMOD_RESULT getSyncPoint(int p_index, FMOD_SYNCPOINT **p_point);
    FMOD_RESULT getSyncPointInfo(FMOD_SYNCPOINT *p_point, char *p_name, int p_namelen, unsigned int *p_offset,
                                 FMOD_TIMEUNIT p_offsettype);
    FMOD_RESULT addSyncPoint(unsigned int p_offset, FMOD_TIMEUNIT p_offsettype, const char *p_name,
                             FMOD_SYNCPOINT **p_point);
    FMOD_RESULT deleteSyncPoint(FMOD_SYNCPOINT *p_point);

    // Functions also in Channel class but here they are the 'default' to save having to change it in Channel all the
    // time.
    FMOD_RESULT setMode(FMOD_MODE p_mode);
    FMOD_RESULT getMode(FMOD_MODE *p_mode);
    FMOD_RESULT setLoopCount(int p_loopcount);
    FMOD_RESULT getLoopCount(int *p_loopcount);
    FMOD_RESULT setLoopPoints(unsigned int p_loopstart, FMOD_TIMEUNIT p_loopstarttype, unsigned int p_loopend,
                              FMOD_TIMEUNIT p_loopendtype);
    FMOD_RESULT getLoopPoints(unsigned int *p_loopstart, FMOD_TIMEUNIT p_loopstarttype, unsigned int *p_loopend,
                              FMOD_TIMEUNIT p_loopendtype);

    // For MOD/S3M/XM/IT/MID sequenced formats only.
    FMOD_RESULT getMusicNumChannels(int *p_numchannels);
    FMOD_RESULT setMusicChannelVolume(int p_channel, float p_volume);
    FMOD_RESULT getMusicChannelVolume(int p_channel, float *p_volume);
    FMOD_RESULT setMusicSpeed(float p_speed);
    FMOD_RESULT getMusicSpeed(float *p_speed);

    // Userdata set/get.
    FMOD_RESULT setUserData(void *p_userdata);
    FMOD_RESULT getUserData(void **p_userdata);
};
