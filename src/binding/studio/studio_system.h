#pragma once
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "variant/packed_float32_array.hpp"
#include "variant/transform2d.hpp"
#include "variant/vector4i.hpp"
namespace FmodGodot
{
namespace Studio
{
using namespace godot;
class StudioSystem : public Object
{
    GDCLASS(StudioSystem, Object)
  protected:
    static void _bind_methods();

  public:
    StudioSystem() = default;
    ~StudioSystem() = default;
    static Handle create(unsigned int p_headerversion = FMOD_VERSION);
    // TODO:
    FMOD_RESULT setAdvancedSettings(Handle p_handle, FMOD_STUDIO_ADVANCEDSETTINGS *p_settings);
    FMOD_RESULT getAdvancedSettings(Handle p_handle, FMOD_STUDIO_ADVANCEDSETTINGS *p_settings);
    // TODO: bind at some point
    // FMOD_RESULT initialize(Handle p_handle, int p_maxchannels, FMOD_STUDIO_INITFLAGS p_studioflags,
    // FMOD_INITFLAGS p_flags, void *p_extradriverdata);

    static FMOD_RESULT initialize(Handle p_handle, int p_maxchannels, FMOD_STUDIO_INITFLAGS p_studioflags,
                                  FMOD_INITFLAGS p_flags);
    static FMOD_RESULT release(Handle p_handle);

    // (FMOD_STUDIO_SYSTEM*)p_handle validity
    static bool is_valid(Handle p_handle);

    // Update processing
    static FMOD_RESULT update(Handle p_handle);
    static FMOD_RESULT flush_commands(Handle p_handle);
    static FMOD_RESULT flush_sample_loading(Handle p_handle);

    // Low-level API access
    // TODO: bind low-level-api
    // FMOD_RESULT getCoreSystem(FMOD::System **system) const;

    // Asset retrieval
    static Handle get_event(Handle p_handle, const String &p_path);
    static Handle get_bus(Handle p_handle, const String &p_path);
    static Handle get_vca(Handle p_handle, const String &p_path);
    static Handle get_bank(Handle p_handle, const String &p_path);
    static Handle get_event_by_id(Handle p_handle, const Vector4i &p_id);
    static Handle get_bus_by_id(Handle p_handle, const Vector4i &p_id);
    static Handle get_vca_by_id(Handle p_handle, const Vector4i &p_id);
    static Handle get_bank_by_id(Handle p_handle, const Vector4i &p_id);
    // TODO: FMOD_RESULT getSoundInfo(const char *p_key, FMOD_STUDIO_SOUND_INFO *p_info) const;
    // TODO: FMOD_RESULT getParameterDescriptionByName(const char *p_name, FMOD_STUDIO_PARAMETER_DESCRIPTION
    // *p_parameter) const;
    // TODO: FMOD_RESULT getParameterDescriptionByID(FMOD_STUDIO_PARAMETER_ID p_id, FMOD_STUDIO_PARAMETER_DESCRIPTION
    // *p_parameter) const;
    static String get_parameter_label_by_name(Handle p_handle, const String &p_name, int p_labelindex);
    static String get_parameter_label_by_id(Handle p_handle, GD_PARAMETER_ID p_id, int p_labelindex);

    // Global parameter control
    static float get_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id);
    static float get_final_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id);
    static FMOD_RESULT set_parameter_by_id(Handle p_handle, GD_PARAMETER_ID p_id, float p_value,
                                           bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameter_by_id_with_label(Handle p_handle, GD_PARAMETER_ID p_id, const String &p_label,
                                                      bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameters_by_ids(Handle p_handle, const PackedInt64Array &p_ids,
                                             PackedFloat32Array p_values, bool p_ignoreseekspeed = false);
    static float get_parameter_by_name(Handle p_handle, const String &p_name);
    static float get_final_parameter_by_name(Handle p_handle, const String &p_name);
    static FMOD_RESULT set_parameter_by_name(Handle p_handle, const String &p_name, float p_value,
                                             bool p_ignoreseekspeed = false);
    static FMOD_RESULT set_parameter_by_name_with_label(Handle p_handle, const String &p_name, const String &p_label,
                                                        bool p_ignoreseekspeed = false);

    // Path lookup
    static Vector4i lookup_id(Handle p_handle, const String &p_path);
    static String lookup_path(Handle p_handle, const Vector4i &p_id);

    // Listener control
    static int get_num_listeners(Handle p_handle);
    static FMOD_RESULT set_num_listeners(Handle p_handle, int p_numlisteners);
    static Transform3D get_listener_transform(Handle p_handle, int p_listener);
    static Vector3 get_listener_velocity(Handle p_handle, int p_listener);
    static Vector3 get_listener_attenuation_position(Handle p_handle, int p_listener);
    static FMOD_RESULT set_listener_attributes(Handle p_handle, int p_listener, const Transform3D &p_transform,
                                               const Vector3 &p_velocity);
    static FMOD_RESULT set_listener_attributes_with_attenuation(Handle p_handle, int p_listener,
                                                                const Transform3D &p_transform,
                                                                const Vector3 &p_velocity,
                                                                const Vector3 &p_attenuationposition);
    static float get_listener_weight(Handle p_handle, int p_listener);
    static FMOD_RESULT set_listener_weight(Handle p_handle, int p_listener, float p_weight);

    // Bank control
    static Handle load_bank_file(Handle p_handle, const String &p_filename, FMOD_STUDIO_LOAD_BANK_FLAGS p_flags);
    static Handle load_bank_memory(Handle p_handle, const PackedByteArray &p_buffer,
                                   FMOD_STUDIO_LOAD_MEMORY_MODE p_mode, FMOD_STUDIO_LOAD_BANK_FLAGS p_flags);
    // TODO: maybe exclude
    //  FMOD_RESULT loadBankCustom(const FMOD_STUDIO_BANK_INFO *p_info, FMOD_STUDIO_LOAD_BANK_FLAGS p_flags, Bank
    //  **p_bank);
    static FMOD_RESULT unload_all(Handle p_handle);

    // TODO: will maybe add include in api
    //  FMOD_RESULT getBufferUsage(FMOD_STUDIO_BUFFER_USAGE *p_usage) const;
    //  FMOD_RESULT resetBufferUsage();
    //  FMOD_RESULT registerPlugin(const FMOD_DSP_DESCRIPTION *p_description);
    //  FMOD_RESULT unregister_plugin(Handle p_handle, const String &p_name);

    // Enumeration
    static int get_bank_count(Handle p_handle);
    static PackedInt64Array get_bank_list(Handle p_handle);
    static int get_parameter_description_count(Handle p_handle);
    static LocalVector<FMOD_STUDIO_PARAMETER_DESCRIPTION> get_parameter_description_list(Handle p_handle);

    // Command capture and replay
    static FMOD_RESULT start_command_capture(Handle p_handle, const String &p_filename,
                                             FMOD_STUDIO_COMMANDCAPTURE_FLAGS p_flags);
    static FMOD_RESULT stop_command_capture(Handle p_handle);
    static Handle load_command_replay(Handle p_handle, const String &p_filename,
                                      FMOD_STUDIO_COMMANDREPLAY_FLAGS p_flags);

    // Callbacks
    FMOD_RESULT setCallback(FMOD_STUDIO_SYSTEM_CALLBACK p_callback,
                            FMOD_STUDIO_SYSTEM_CALLBACK_TYPE p_callbackmask = FMOD_STUDIO_SYSTEM_CALLBACK_ALL);
    static void *getUserData(Handle p_handle);
    static FMOD_RESULT setUserData(Handle p_handle, void *p_userdata);

    // Monitoring
    // TODO:
    // FMOD_RESULT getCpuUsage(FMOD_STUDIO_CPU_USAGE *p_usage, FMOD_CPU_USAGE *p_usage_core) const;
    // TODO:
    // FMOD_RESULT getMemoryUsage(FMOD_STUDIO_MEMORY_USAGE *p_memoryusage) const;
};
} // namespace Studio
} // namespace FmodGodot
