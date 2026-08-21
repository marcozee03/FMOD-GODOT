#include "fmod.h"
#include "fmod_common.h"
#include "fmod_channel_control.h"
class System;
class Sound;
class Channel;
class ChannelGroup;
class SoundGroup;
class DSP;
class DSPConnection;
class Geometry;
class Reverb3D;

class ChannelGroup : public ChannelControl
{
  private:
    // Constructor made private so user cannot statically instance a ChannelGroup class.  Appropriate ChannelGroup
    // creation or retrieval function must be used.
    ChannelGroup();
    ChannelGroup(const ChannelGroup &);

  public:
    FMOD_RESULT  release();

    // Nested channel groups.
    FMOD_RESULT  addGroup(ChannelGroup *group, bool propagatedspclock = true, DSPConnection **connection = 0);
    FMOD_RESULT  getNumGroups(int *numgroups);
    FMOD_RESULT  getGroup(int index, ChannelGroup **group);
    FMOD_RESULT  getParentGroup(ChannelGroup **group);

    // Information only functions.
    FMOD_RESULT  getName(char *name, int namelen);
    FMOD_RESULT  getNumChannels(int *numchannels);
    FMOD_RESULT  getChannel(int index, Channel **channel);
};
