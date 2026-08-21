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

class System
{
  private:
    // Constructor made private so user cannot statically instance a System class.  System_Create must be used.
    System();
    System(const System &);

  public:
    FMOD_RESULT release();

    // Setup functions.
    FMOD_RESULT setOutput(FMOD_OUTPUTTYPE p_output);
    FMOD_RESULT getOutput(FMOD_OUTPUTTYPE *p_output);
    FMOD_RESULT getNumDrivers(int *p_numdrivers);
    FMOD_RESULT getDriverInfo(int p_id, char *p_name, int p_namelen, FMOD_GUID *p_guid, int *p_systemrate,
                              FMOD_SPEAKERMODE *p_speakermode, int *p_speakermodechannels);
    FMOD_RESULT setDriver(int p_driver);
    FMOD_RESULT getDriver(int *p_driver);
    FMOD_RESULT setSoftwareChannels(int p_numsoftwarechannels);
    FMOD_RESULT getSoftwareChannels(int *p_numsoftwarechannels);
    FMOD_RESULT setSoftwareFormat(int p_samplerate, FMOD_SPEAKERMODE p_speakermode, int p_numrawspeakers);
    FMOD_RESULT getSoftwareFormat(int *p_samplerate, FMOD_SPEAKERMODE *p_speakermode, int *p_numrawspeakers);
    FMOD_RESULT setDSPBufferSize(unsigned int p_bufferlength, int p_numbuffers);
    FMOD_RESULT getDSPBufferSize(unsigned int *p_bufferlength, int *p_numbuffers);
    FMOD_RESULT setFileSystem(FMOD_FILE_OPEN_CALLBACK p_useropen, FMOD_FILE_CLOSE_CALLBACK p_userclose,
                              FMOD_FILE_READ_CALLBACK p_userread, FMOD_FILE_SEEK_CALLBACK p_userseek,
                              FMOD_FILE_ASYNCREAD_CALLBACK p_userasyncread,
                              FMOD_FILE_ASYNCCANCEL_CALLBACK p_userasynccancel, int p_blockalign);
    FMOD_RESULT attachFileSystem(FMOD_FILE_OPEN_CALLBACK p_useropen, FMOD_FILE_CLOSE_CALLBACK p_userclose,
                                 FMOD_FILE_READ_CALLBACK p_userread, FMOD_FILE_SEEK_CALLBACK p_userseek);
    FMOD_RESULT setAdvancedSettings(FMOD_ADVANCEDSETTINGS *p_settings);
    FMOD_RESULT getAdvancedSettings(FMOD_ADVANCEDSETTINGS *p_settings);
    FMOD_RESULT setCallback(FMOD_SYSTEM_CALLBACK p_callback,
                            FMOD_SYSTEM_CALLBACK_TYPE p_callbackmask = FMOD_SYSTEM_CALLBACK_ALL);

    // Plug-in support.
    FMOD_RESULT setPluginPath(const char *p_path);
    FMOD_RESULT loadPlugin(const char *p_filename, unsigned int *p_handle, unsigned int p_priority = 0);
    FMOD_RESULT unloadPlugin(unsigned int p_handle);
    FMOD_RESULT getNumNestedPlugins(unsigned int p_handle, int *p_count);
    FMOD_RESULT getNestedPlugin(unsigned int p_handle, int p_index, unsigned int *p_nestedhandle);
    FMOD_RESULT getNumPlugins(FMOD_PLUGINTYPE p_plugintype, int *p_numplugins);
    FMOD_RESULT getPluginHandle(FMOD_PLUGINTYPE p_plugintype, int p_index, unsigned int *p_handle);
    FMOD_RESULT getPluginInfo(unsigned int p_handle, FMOD_PLUGINTYPE *p_plugintype, char *p_name, int p_namelen,
                              unsigned int *p_version);
    FMOD_RESULT setOutputByPlugin(unsigned int p_handle);
    FMOD_RESULT getOutputByPlugin(unsigned int *p_handle);
    FMOD_RESULT createDSPByPlugin(unsigned int p_handle, DSP **p_dsp);
    FMOD_RESULT getDSPInfoByPlugin(unsigned int p_handle, const FMOD_DSP_DESCRIPTION **p_description);
    FMOD_RESULT registerCodec(FMOD_CODEC_DESCRIPTION *p_description, unsigned int *p_handle,
                              unsigned int p_priority = 0);
    FMOD_RESULT registerDSP(const FMOD_DSP_DESCRIPTION *p_description, unsigned int *p_handle);
    FMOD_RESULT registerOutput(const FMOD_OUTPUT_DESCRIPTION *p_description, unsigned int *p_handle);

    // Init/Close.
    FMOD_RESULT init(int p_maxchannels, FMOD_INITFLAGS p_flags, void *p_extradriverdata);
    FMOD_RESULT close();

    // General post-init system functions.
    FMOD_RESULT update(); /* IMPORTANT! CALL THIS ONCE PER FRAME! */

    FMOD_RESULT setSpeakerPosition(FMOD_SPEAKER p_speaker, float p_x, float p_y, bool p_active);
    FMOD_RESULT getSpeakerPosition(FMOD_SPEAKER p_speaker, float *p_x, float *p_y, bool *p_active);
    FMOD_RESULT setStreamBufferSize(unsigned int p_filebuffersize, FMOD_TIMEUNIT p_filebuffersizetype);
    FMOD_RESULT getStreamBufferSize(unsigned int *p_filebuffersize, FMOD_TIMEUNIT *p_filebuffersizetype);
    FMOD_RESULT set3DSettings(float p_dopplerscale, float p_distancefactor, float p_rolloffscale);
    FMOD_RESULT get3DSettings(float *p_dopplerscale, float *p_distancefactor, float *p_rolloffscale);
    FMOD_RESULT set3DNumListeners(int p_numlisteners);
    FMOD_RESULT get3DNumListeners(int *p_numlisteners);
    FMOD_RESULT set3DListenerAttributes(int p_listener, const FMOD_VECTOR *p_pos, const FMOD_VECTOR *p_vel,
                                        const FMOD_VECTOR *p_forward, const FMOD_VECTOR *p_up);
    FMOD_RESULT get3DListenerAttributes(int p_listener, FMOD_VECTOR *p_pos, FMOD_VECTOR *p_vel, FMOD_VECTOR *p_forward,
                                        FMOD_VECTOR *p_up);
    FMOD_RESULT set3DRolloffCallback(FMOD_3D_ROLLOFF_CALLBACK p_callback);
    FMOD_RESULT mixerSuspend();
    FMOD_RESULT mixerResume();
    FMOD_RESULT getDefaultMixMatrix(FMOD_SPEAKERMODE p_sourcespeakermode, FMOD_SPEAKERMODE p_targetspeakermode,
                                    float *p_matrix, int p_matrixhop);
    FMOD_RESULT getSpeakerModeChannels(FMOD_SPEAKERMODE p_mode, int *p_channels);

    // System information functions.
    FMOD_RESULT getVersion(unsigned int *p_version, unsigned int *p_buildnumber = nullptr);
    FMOD_RESULT getOutputHandle(void **p_handle);
    FMOD_RESULT getChannelsPlaying(int *p_channels, int *p_realchannels = nullptr);
    FMOD_RESULT getCPUUsage(FMOD_CPU_USAGE *p_usage);
    FMOD_RESULT getFileUsage(long long *p_sample_bytes_read, long long *p_stream_bytes_read,
                             long long *p_other_bytes_read);

    // Sound/DSP/Channel/FX creation and retrieval.
    FMOD_RESULT createSound(const char *p_name_or_data, FMOD_MODE p_mode, FMOD_CREATESOUNDEXINFO *p_exinfo,
                            Sound **p_sound);
    FMOD_RESULT createStream(const char *p_name_or_data, FMOD_MODE p_mode, FMOD_CREATESOUNDEXINFO *p_exinfo,
                             Sound **p_sound);
    FMOD_RESULT createDSP(const FMOD_DSP_DESCRIPTION *p_description, DSP **p_dsp);
    FMOD_RESULT createDSPByType(FMOD_DSP_TYPE p_type, DSP **p_dsp);
    FMOD_RESULT createDSPConnection(FMOD_DSPCONNECTION_TYPE p_type, DSPConnection **p_connection);
    FMOD_RESULT createChannelGroup(const char *p_name, ChannelGroup **p_channelgroup);
    FMOD_RESULT createSoundGroup(const char *p_name, SoundGroup **p_soundgroup);
    FMOD_RESULT createReverb3D(Reverb3D **p_reverb);

    FMOD_RESULT playSound(Sound *p_sound, ChannelGroup *p_channelgroup, bool p_paused, Channel **p_channel);
    FMOD_RESULT playDSP(DSP *p_dsp, ChannelGroup *p_channelgroup, bool p_paused, Channel **p_channel);
    FMOD_RESULT getChannel(int p_channelid, Channel **p_channel);
    FMOD_RESULT getDSPInfoByType(FMOD_DSP_TYPE p_type, const FMOD_DSP_DESCRIPTION **p_description);
    FMOD_RESULT getMasterChannelGroup(ChannelGroup **p_channelgroup);
    FMOD_RESULT getMasterSoundGroup(SoundGroup **p_soundgroup);

    // Routing to ports.
    FMOD_RESULT attachChannelGroupToPort(FMOD_PORT_TYPE p_port_type, FMOD_PORT_INDEX p_port_index,
                                         ChannelGroup *p_channelgroup, bool p_pass_thru = false);
    FMOD_RESULT detachChannelGroupFromPort(ChannelGroup *p_channelgroup);

    // Reverb API.
    FMOD_RESULT setReverbProperties(int p_instance, const FMOD_REVERB_PROPERTIES *p_prop);
    FMOD_RESULT getReverbProperties(int p_instance, FMOD_REVERB_PROPERTIES *p_prop);

    // System level DSP functionality.
    FMOD_RESULT lockDSP();
    FMOD_RESULT unlockDSP();

    // Recording API.
    FMOD_RESULT getRecordNumDrivers(int *p_numdrivers, int *p_numconnected);
    FMOD_RESULT getRecordDriverInfo(int p_id, char *p_name, int p_namelen, FMOD_GUID *p_guid, int *p_systemrate,
                                    FMOD_SPEAKERMODE *p_speakermode, int *p_speakermodechannels,
                                    FMOD_DRIVER_STATE *p_state);
    FMOD_RESULT getRecordPosition(int p_id, unsigned int *p_position);
    FMOD_RESULT recordStart(int p_id, Sound *p_sound, bool p_loop);
    FMOD_RESULT recordStop(int p_id);
    FMOD_RESULT isRecording(int p_id, bool *p_recording);

    // Geometry API.
    FMOD_RESULT createGeometry(int p_maxpolygons, int p_maxvertices, Geometry **p_geometry);
    FMOD_RESULT setGeometrySettings(float p_maxworldsize);
    FMOD_RESULT getGeometrySettings(float *p_maxworldsize);
    FMOD_RESULT loadGeometry(const void *p_data, int p_datasize, Geometry **p_geometry);
    FMOD_RESULT getGeometryOcclusion(const FMOD_VECTOR *p_listener, const FMOD_VECTOR *p_source, float *p_direct,
                                     float *p_reverb);

    // Network functions.
    FMOD_RESULT setNetworkProxy(const char *p_proxy);
    FMOD_RESULT getNetworkProxy(char *p_proxy, int p_proxylen);
    FMOD_RESULT setNetworkTimeout(int p_timeout);
    FMOD_RESULT getNetworkTimeout(int *p_timeout);

    // Userdata set/get.
    FMOD_RESULT setUserData(void *p_userdata);
    FMOD_RESULT getUserData(void **p_userdata);
};
