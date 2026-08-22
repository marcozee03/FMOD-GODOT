#pragma once
#include "binding/studio/parameter_cache.h"
#include "classes/ref_counted.hpp"
#include "classes/wrapped.hpp"
#include "fmod_defs.h"
#include "variant/variant.hpp"
#include "variant/vector2i.hpp"
namespace FmodGodot
{
namespace Studio
{
class StudioEventDescription : public RefCounted
{
    GDCLASS(StudioEventDescription, RefCounted)
  protected:
    static void _bind_methods();

  public:
#ifdef TOOLS_ENABLED
    struct Cache
    {
        Cache() = default;
        Cache(FMOD_STUDIO_EVENTDESCRIPTION *p_description);
        String full_path;
        Vector4i guid;
        bool is3d;
        bool doppler_enabled;
        bool one_shot;
        bool stream;
        float min;
        float max;
        int lengthMS;
        Vector<FmodGodot::ParameterCache> parameters;
    };
#endif
    StudioEventDescription() = default;
    ~StudioEventDescription() = default;
    // Handle validity
    static bool is_valid(Handle p_handle);

    // Property access
    static Vector4i get_id(Handle p_handle);
    static String get_path(Handle p_handle);
    static int get_parameter_description_count(Handle p_handle);
    static FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByIndex(Handle p_handle, int p_index);
    static FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByName(Handle p_handle, const String &p_name);
    static FMOD_STUDIO_PARAMETER_DESCRIPTION getParameterDescriptionByID(Handle p_handle, uint64_t p_id);
    static String get_parameter_label_by_index(Handle p_handle, int p_index, int p_labelindex);
    static String get_parameter_label_by_name(Handle p_handle, const String &p_name, int p_labelindex);
    static String get_parameter_label_by_id(Handle p_handle, uint64_t p_id, int p_labelindex);
    static int get_user_property_count(Handle p_handle);

    static Variant get_user_property_by_index(Handle p_handle, int p_index);
    static Variant get_user_property(Handle p_handle, const String &p_name);
    static int get_length(Handle p_handle);
    static float get_min_distance(Handle p_handle);
    static float get_max_distance(Handle p_handle);
    static float get_sound_size(Handle p_handle);

    static bool is_snapshot(Handle p_handle);
    static bool is_oneshot(Handle p_handle);
    static bool is_stream(Handle p_handle);
    static bool is3D(Handle p_handle);
    static bool is_doppler_enabled(Handle p_handle);
    static bool has_sustain_point(Handle p_handle);

    static Handle create_instance(Handle p_handle);
    static int get_instance_count(Handle p_handle);
    static PackedInt64Array get_instance_list(Handle p_handle);

    // Sample data loading control
    static FMOD_RESULT load_sample_data(Handle p_handle);
    static FMOD_RESULT unload_sample_data(Handle p_handle);
    static FMOD_STUDIO_LOADING_STATE get_sample_loading_state(Handle p_handle);

    // Convenience functions
    static FMOD_RESULT release_all_instances(Handle p_handle);

    // TODO: bind to signals
    //  Callbacks
    //  FMOD_RESULT setCallback(FMOD_STUDIO_EVENT_CALLBACK callback,
    //                          FMOD_STUDIO_EVENT_CALLBACK_TYPE callbackmask = FMOD_STUDIO_EVENT_CALLBACK_ALL);
    static void *get_user_data(Handle p_handle);
    static FMOD_RESULT set_user_data(Handle p_handle, void *p_userdata);
};
} // namespace Studio
} // namespace FmodGodot
