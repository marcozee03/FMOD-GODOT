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

class DSP
{
  private:
    // Constructor made private so user cannot statically instance a DSP class.  Appropriate DSP creation or retrieval
    // function must be used.
    DSP();
    DSP(const DSP &);

  public:
    FMOD_RESULT  release();
    FMOD_RESULT getSystemObject(System **p_system);

    // Connection / disconnection / input and output enumeration.
    FMOD_RESULT addInput(DSP *p_input, DSPConnection **p_connection = nullptr,
                         FMOD_DSPCONNECTION_TYPE p_type = FMOD_DSPCONNECTION_TYPE_STANDARD);
    FMOD_RESULT addInputPreallocated(DSP *p_input, DSPConnection **p_connection = nullptr);
    FMOD_RESULT disconnectFrom(DSP *p_target, DSPConnection *p_connection = nullptr);
    FMOD_RESULT disconnectAll(bool p_inputs, bool p_outputs);
    FMOD_RESULT getNumInputs(int *p_numinputs);
    FMOD_RESULT getNumOutputs(int *p_numoutputs);
    FMOD_RESULT getInput(int p_index, DSP **p_input, DSPConnection **p_inputconnection);
    FMOD_RESULT getOutput(int p_index, DSP **p_output, DSPConnection **p_outputconnection);

    // DSP unit control.
    FMOD_RESULT setActive(bool p_active);
    FMOD_RESULT getActive(bool *p_active);
    FMOD_RESULT setBypass(bool p_bypass);
    FMOD_RESULT getBypass(bool *p_bypass);
    FMOD_RESULT setWetDryMix(float p_prewet, float p_postwet, float p_dry);
    FMOD_RESULT getWetDryMix(float *p_prewet, float *p_postwet, float *p_dry);
    FMOD_RESULT setChannelFormat(FMOD_CHANNELMASK p_channelmask, int p_numchannels,
                                 FMOD_SPEAKERMODE p_source_speakermode);
    FMOD_RESULT getChannelFormat(FMOD_CHANNELMASK *p_channelmask, int *p_numchannels,
                                 FMOD_SPEAKERMODE *p_source_speakermode);
    FMOD_RESULT getOutputChannelFormat(FMOD_CHANNELMASK p_inmask, int p_inchannels, FMOD_SPEAKERMODE p_inspeakermode,
                                       FMOD_CHANNELMASK *p_outmask, int *p_outchannels,
                                       FMOD_SPEAKERMODE *p_outspeakermode);
    FMOD_RESULT  reset();
    FMOD_RESULT setCallback(FMOD_DSP_CALLBACK p_callback);

    // DSP parameter control.
    FMOD_RESULT setParameterFloat(int p_index, float p_value);
    FMOD_RESULT setParameterInt(int p_index, int p_value);
    FMOD_RESULT setParameterBool(int p_index, bool p_value);
    FMOD_RESULT setParameterData(int p_index, void *p_data, unsigned int p_length);
    FMOD_RESULT getParameterFloat(int p_index, float *p_value, char *p_valuestr, int p_valuestrlen);
    FMOD_RESULT getParameterInt(int p_index, int *p_value, char *p_valuestr, int p_valuestrlen);
    FMOD_RESULT getParameterBool(int p_index, bool *p_value, char *p_valuestr, int p_valuestrlen);
    FMOD_RESULT getParameterData(int p_index, void **p_data, unsigned int *p_length, char *p_valuestr,
                                 int p_valuestrlen);
    FMOD_RESULT getNumParameters(int *p_numparams);
    FMOD_RESULT getParameterInfo(int p_index, FMOD_DSP_PARAMETER_DESC **p_desc);
    FMOD_RESULT getDataParameterIndex(int p_datatype, int *p_index);
    FMOD_RESULT showConfigDialog(void *p_hwnd, bool p_show);

    // DSP attributes.
    FMOD_RESULT getInfo(char *p_name, unsigned int *p_version, int *p_channels, int *p_configwidth,
                        int *p_configheight);
    FMOD_RESULT getType(FMOD_DSP_TYPE *p_type);
    FMOD_RESULT getIdle(bool *p_idle);

    // Userdata set/get.
    FMOD_RESULT setUserData(void *p_userdata);
    FMOD_RESULT getUserData(void **p_userdata);

    // Metering.
    FMOD_RESULT setMeteringEnabled(bool p_input_enabled, bool p_output_enabled);
    FMOD_RESULT getMeteringEnabled(bool *p_input_enabled, bool *p_output_enabled);
    FMOD_RESULT getMeteringInfo(FMOD_DSP_METERING_INFO *p_input_info, FMOD_DSP_METERING_INFO *p_output_info);
    FMOD_RESULT getCPUUsage(unsigned int *p_exclusive, unsigned int *p_inclusive);
};
