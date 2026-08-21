
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

class Channel : public ChannelControl
{
  private:
    // Constructor made private so user cannot statically instance a Channel class.  Appropriate Channel creation or
    // retrieval function must be used.
    Channel();
    Channel(const Channel &);

  public:
    // Channel specific control functionality.
    FMOD_RESULT F_API setFrequency(float frequency);
    FMOD_RESULT F_API getFrequency(float *frequency);
    FMOD_RESULT F_API setPriority(int priority);
    FMOD_RESULT F_API getPriority(int *priority);
    FMOD_RESULT F_API setPosition(unsigned int position, FMOD_TIMEUNIT postype);
    FMOD_RESULT F_API getPosition(unsigned int *position, FMOD_TIMEUNIT postype);
    FMOD_RESULT F_API setChannelGroup(ChannelGroup *channelgroup);
    FMOD_RESULT F_API getChannelGroup(ChannelGroup **channelgroup);
    FMOD_RESULT F_API setLoopCount(int loopcount);
    FMOD_RESULT F_API getLoopCount(int *loopcount);
    FMOD_RESULT F_API setLoopPoints(unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend,
                                    FMOD_TIMEUNIT loopendtype);
    FMOD_RESULT F_API getLoopPoints(unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend,
                                    FMOD_TIMEUNIT loopendtype);

    // Information only functions.
    FMOD_RESULT F_API isVirtual(bool *isvirtual);
    FMOD_RESULT F_API getCurrentSound(Sound **sound);
    FMOD_RESULT F_API getIndex(int *index);
};
