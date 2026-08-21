#include "fmod.h"
#include "fmod_common.h"
#include "variant/packed_float32_array.hpp"
#include "variant/packed_int64_array.hpp"
#include "variant/packed_vector3_array.hpp"
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

class ChannelControl
{
  private:
    FMOD_CHANNEL *handle;
    // Constructor made private so user cannot statically instance a Control class.
    ChannelControl();
    ChannelControl(const ChannelControl &);

  public:
    FMOD_RESULT getSystemObject(System **system);

    // General control functionality for Channels and ChannelGroups.
    FMOD_RESULT stop()
    {
        return FMOD_Channel_Stop(handle);
    }
    FMOD_RESULT setPaused(bool paused)
    {
        return FMOD_Channel_SetPaused(handle, paused);
    }
    bool getPaused()
    {
        FMOD_BOOL paused;
        FMOD_Channel_GetPaused(handle, &paused);
        return paused;
    }
    FMOD_RESULT setVolume(float volume)
    {
        return FMOD_Channel_SetVolume(handle, volume);
    }
    float getVolume()
    {
        float volume;
        FMOD_Channel_GetVolume(handle, &volume);
        return volume;
    }
    FMOD_RESULT setVolumeRamp(bool ramp)
    {
        return FMOD_Channel_SetVolumeRamp(handle, ramp);
    }
    bool getVolumeRamp()
    {
        FMOD_BOOL ramp;
        FMOD_Channel_GetVolumeRamp(handle, &ramp);
        return ramp;
    }
    float getAudibility()
    {
        float audibility;
        FMOD_Channel_GetAudibility(handle, &audibility);
        return audibility;
    }
    FMOD_RESULT setPitch(float pitch)
    {
        return FMOD_Channel_SetPitch(handle, pitch);
    }
    float getPitch()
    {
        float pitch;
        FMOD_Channel_GetPitch(handle, &pitch);
        return pitch;
    }
    FMOD_RESULT setMute(bool mute)
    {
        return FMOD_Channel_SetMute(handle, mute);
    }
    bool getMute()
    {
        FMOD_BOOL mute;
        FMOD_Channel_GetMute(handle, &mute);
        return mute;
    }
    FMOD_RESULT setReverbProperties(int instance, float wet)
    {
        return FMOD_Channel_SetReverbProperties(handle, instance, wet);
    }
    float getReverbProperties(int instance)
    {
        float wet;
        FMOD_Channel_GetReverbProperties(handle, instance, &wet);
        return wet;
    }
    FMOD_RESULT setLowPassGain(float gain)
    {
        return FMOD_Channel_SetLowPassGain(handle, gain);
    }
    float getLowPassGain()
    {
        float gain;
        FMOD_Channel_GetLowPassGain(handle, &gain);
        return gain;
    }
    FMOD_RESULT setMode(FMOD_MODE mode)
    {
        return FMOD_Channel_SetMode(handle, mode);
    }
    FMOD_MODE getMode()
    {
        FMOD_MODE mode;
        FMOD_Channel_GetMode(handle, &mode);
        return mode;
    }
    FMOD_RESULT setCallback(FMOD_CHANNELCONTROL_CALLBACK callback)
    {
        return FMOD_Channel_SetCallback(handle, callback);
    }
    bool isPlaying()
    {
        FMOD_BOOL playing;
        FMOD_Channel_IsPlaying(handle, &playing);
        return playing;
    }

    // Panning and level adjustment.
    // Note all 'set' functions alter a final matrix, this is why the only get function is getMixMatrix, to avoid other
    // get functions returning incorrect/obsolete values.
    FMOD_RESULT setPan(float pan)
    {
        return FMOD_Channel_SetPan(handle, pan);
    }
    FMOD_RESULT setMixLevelsOutput(float frontleft, float frontright, float center, float lfe, float surroundleft,
                                   float surroundright, float backleft, float backright)
    {
        return FMOD_Channel_SetMixLevelsOutput(handle, frontleft, frontright, center, lfe, surroundleft, surroundright,
                                               backleft, backright);
    }
    FMOD_RESULT setMixLevelsInput(godot::PackedFloat32Array &levels)
    {
        FMOD_Channel_SetMixLevelsInput(handle, levels.ptrw(), levels.size());
    }
    // TODO:
    // FMOD_RESULT setMixMatrix(float *matrix, int outchannels, int inchannels, int inchannel_hop = 0)
    // {
    //     FMOD_Channel_SetMixMatrix(handle);
    // }
    // TODO:
    // FMOD_RESULT getMixMatrix(float *matrix, int *outchannels, int *inchannels, int inchannel_hop = 0)
    // {
    //     FMOD_Channel_GetMixMatrix(handle);
    // }

    // Clock based functionality.
    unsigned long long getDSPClock()
    {
        unsigned long long dsp_clock;
        FMOD_Channel_GetDSPClock(handle, &dsp_clock, nullptr);
        return dsp_clock;
    }
    unsigned long long getDSPParentClock()
    {
        unsigned long long parent_clock;
        FMOD_Channel_GetDSPClock(handle, nullptr, &parent_clock);
        return parent_clock;
    }
    FMOD_RESULT setDelay(unsigned long long dspclock_start, unsigned long long dspclock_end, bool stopchannels = true)
    {
        FMOD_Channel_SetDelay(handle, dspclock_start, dspclock_end, stopchannels);
    }
    unsigned long long getDelayStart()
    {
        unsigned long long dspclock_start;
        FMOD_Channel_GetDelay(handle, &dspclock_start, nullptr, nullptr);
        return dspclock_start;
    }
    unsigned long long getDelayEnd()
    {
        unsigned long long dspclock_end;
        FMOD_Channel_GetDelay(handle, nullptr, &dspclock_end, nullptr);
        return dspclock_end;
    }
    bool getDelayStopChannels()
    {
        FMOD_BOOL stop_channels;
        FMOD_Channel_GetDelay(handle, nullptr, nullptr, &stop_channels);
        return stop_channels;
    }
    FMOD_RESULT addFadePoint(unsigned long long dspclock, float volume)
    {
        FMOD_Channel_AddFadePoint(handle, dspclock, volume);
    }
    FMOD_RESULT setFadePointRamp(unsigned long long dspclock, float volume)
    {
        FMOD_Channel_SetFadePointRamp(handle, dspclock, volume);
    }
    FMOD_RESULT removeFadePoints(unsigned long long dspclock_start, unsigned long long dspclock_end)
    {
        FMOD_Channel_RemoveFadePoints(handle, dspclock_start, dspclock_end);
    }
    godot::PackedInt64Array getFadePointsClock()
    {
        godot::PackedInt64Array point_dspclock;
        unsigned int numpoints;
        FMOD_Channel_GetFadePoints(handle, &numpoints, nullptr, nullptr);
        point_dspclock.resize(numpoints);

        FMOD_Channel_GetFadePoints(handle, &numpoints, (unsigned long long *)point_dspclock.ptrw(), nullptr);
        return point_dspclock;
    }
    godot::PackedFloat32Array getFadePointsVolume()
    {
        godot::PackedFloat32Array point_volume;
        unsigned int numpoints;
        FMOD_Channel_GetFadePoints(handle, &numpoints, nullptr, nullptr);
        point_volume.resize(numpoints);

        FMOD_Channel_GetFadePoints(handle, &numpoints, nullptr, point_volume.ptrw());
        return point_volume;
    }

    // DSP effects.
    // TODO:
    FMOD_RESULT getDSP(int index, DSP **dsp)
    {
        FMOD_Channel_GetDSP(handle);
    }
    // TODO:
    FMOD_RESULT addDSP(int index, DSP *dsp)
    {
        FMOD_Channel_AddDSP(handle);
    }
    // TODO:
    FMOD_RESULT removeDSP(DSP *dsp)
    {
        FMOD_Channel_RemoveDSP(handle);
    }
    // TODO:
    FMOD_RESULT getNumDSPs(int *numdsps)
    {
        FMOD_Channel_GetNumDSPs(handle);
    }
    // TODO:
    FMOD_RESULT setDSPIndex(DSP *dsp, int index)
    {
        FMOD_Channel_SetDSPIndex(handle);
    }
    // TODO:
    FMOD_RESULT getDSPIndex(DSP *dsp, int *index)
    {
        FMOD_Channel_GetDSPIndex(handle);
    }

    // 3D functionality.
    FMOD_RESULT set3DAttributes(const FMOD_VECTOR *pos, const FMOD_VECTOR *vel)
    {
        return FMOD_Channel_Set3DAttributes(handle, pos, vel);
    }
    FMOD_RESULT get3DAttributes(FMOD_VECTOR *pos, FMOD_VECTOR *vel)
    {
        FMOD_Channel_Get3DAttributes(handle);
    }
    FMOD_RESULT set3DMinMaxDistance(float mindistance, float maxdistance)
    {
        return FMOD_Channel_Set3DMinMaxDistance(handle, mindistance, maxdistance);
    }
    float get3DMaxDistance()
    {
        float max;
        FMOD_Channel_Get3DMinMaxDistance(handle, nullptr, &max);
        return max;
    }
    float get3DMinDistance()
    {
        float min;
        FMOD_Channel_Get3DMinMaxDistance(handle, nullptr, &min);
        return min;
    }
    FMOD_RESULT set3DConeSettings(float insideconeangle, float outsideconeangle, float outsidevolume)
    {
        FMOD_Channel_Set3DConeSettings(handle, insideconeangle, outsideconeangle, outsidevolume);
    }
    FMOD_RESULT get3DConeSettings(float *insideconeangle, float *outsideconeangle, float *outsidevolume)
    {
        FMOD_Channel_Get3DConeSettings(handle);
    }
    FMOD_RESULT set3DConeOrientation(FMOD_VECTOR *orientation)
    {
        FMOD_Channel_Set3DConeOrientation(handle);
    }
    FMOD_RESULT get3DConeOrientation(FMOD_VECTOR *orientation)
    {
        FMOD_Channel_Get3DConeOrientation(handle);
    }
    FMOD_RESULT set3DCustomRolloff(godot::PackedVector3Array &points)
    {
        FMOD_Channel_Set3DCustomRolloff(handle, (FMOD_VECTOR *)points.ptrw(), points.size());
    }
    FMOD_RESULT get3DCustomRolloff(FMOD_VECTOR **points, int *numpoints)
    {
        FMOD_Channel_Get3DCustomRolloff(handle);
    }
    FMOD_RESULT set3DOcclusion(float directocclusion, float reverbocclusion)
    {
        FMOD_Channel_Set3DOcclusion(handle);
    }
    FMOD_RESULT get3DOcclusion(float *directocclusion, float *reverbocclusion)
    {
        FMOD_Channel_Get3DOcclusion(handle);
    }
    FMOD_RESULT set3DSpread(float angle)
    {
        return FMOD_Channel_Set3DSpread(handle, angle);
    }
    float get3DSpread()
    {
        float angle;
        FMOD_Channel_Get3DSpread(handle, &angle);
        return angle;
    }
    FMOD_RESULT set3DLevel(float level)
    {
        FMOD_Channel_Set3DLevel(handle);
    }
    FMOD_RESULT get3DLevel(float *level)
    {
        FMOD_Channel_Get3DLevel(handle);
    }
    FMOD_RESULT set3DDopplerLevel(float level)
    {
        FMOD_Channel_Set3DDopplerLevel(handle);
    }
    FMOD_RESULT get3DDopplerLevel(float *level)
    {
        FMOD_Channel_Get3DDopplerLevel(handle);
    }
    FMOD_RESULT set3DDistanceFilter(bool custom, float customLevel, float centerFreq)
    {
        return FMOD_Channel_Set3DDistanceFilter(handle, customLevel, centerFreq);
    }
    FMOD_RESULT get3DDistanceFilter(bool *custom, float *customLevel, float *centerFreq)
    {
        FMOD_Channel_Get3DDistanceFilter(handle);
    }

    // Userdata set/get.
    FMOD_RESULT setUserData(void *userdata)
    {
        return FMOD_Channel_SetUserData(handle, userdata);
    }
    FMOD_RESULT getUserData(void **userdata)
    {
        FMOD_Channel_GetUserData(handle);
    }
};
