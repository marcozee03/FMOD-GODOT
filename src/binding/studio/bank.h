#pragma once
#include "binding/conversions.h"
#include "fmod_common.h"
#include "fmod_defs.h"
#include "fmod_studio_common.h"
#include "globals.h"
#include <fmod_studio.h>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/defs.hpp>
using namespace godot;
namespace FmodGodot
{
namespace Studio
{
class FmodBankFormatLoader;
class Bank : public Object
{

    GDCLASS(Bank, Object);

  private:
  protected:
    static void _bind_methods()
    {
        BIND_STATIC_METHOD(is_valid, "handle");
        BIND_STATIC_METHOD(get_id, "handle");
        BIND_STATIC_METHOD(get_path, "handle");
        BIND_STATIC_METHOD(unload, "handle");
        BIND_STATIC_METHOD(load_sample_data, "handle");
        BIND_STATIC_METHOD(unload_sample_data, "handle");
        BIND_STATIC_METHOD(get_loading_state, "handle");
        BIND_STATIC_METHOD(get_sample_loading_state, "handle");
        BIND_STATIC_METHOD(get_string_count, "handle");
        BIND_STATIC_METHOD(get_string_info, "handle", "index", "id");
        BIND_STATIC_METHOD(get_event_count, "handle");
        BIND_STATIC_METHOD(get_event_list, "handle");
        BIND_STATIC_METHOD(get_bus_count, "handle");
        BIND_STATIC_METHOD(get_bus_list, "handle");
        BIND_STATIC_METHOD(get_vca_count, "handle");
        BIND_STATIC_METHOD(get_vca_list, "handle");
    }

  public:
#ifdef TOOLS_ENABLED
    struct Cache
    {
        Cache() = default;
        Cache(FMOD_STUDIO_BANK *p_bank);
        String full_path;
        Vector4i guid;
        Vector<String> children;
    };
#endif
    Bank() = default;
    ~Bank() = default;

    static bool is_valid(Handle p_handle);

    static Vector4i get_id(Handle p_handle);

    static String get_path(Handle p_handle);
    static FMOD_RESULT unload(Handle p_handle);
    static FMOD_RESULT load_sample_data(Handle p_handle);
    static FMOD_RESULT unload_sample_data(Handle p_handle);

    static FMOD_STUDIO_LOADING_STATE get_loading_state(Handle p_handle);
    static FMOD_STUDIO_LOADING_STATE get_sample_loading_state(Handle p_handle);

    static int get_string_count(Handle p_handle);
    static String get_string_info(Handle p_handle, int p_index, const Vector4i &p_id);
    static int get_event_count(Handle p_handle);
    static PackedInt64Array get_event_list(Handle p_handle);
    static int get_bus_count(Handle p_handle);
    static PackedInt64Array get_bus_list(Handle p_handle);
    static int get_vca_count(Handle p_handle);
    static PackedInt64Array get_vca_list(Handle p_handle);

    static void *get_user_data(Handle p_handle);
    static FMOD_RESULT set_user_data(Handle p_handle, void *p_userdata);
};

} // namespace Studio
} // namespace FmodGodot
