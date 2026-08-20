#pragma once
#include "classes/ref_counted.hpp"
#include "fmod_common.h"
#include "fmod_studio_common.h"
#include <godot_cpp/variant/vector4i.hpp>

using namespace godot;
namespace FmodGodot
{
namespace Studio
{
class VCA : public Object
{
    GDCLASS(VCA, Object)
  protected:
    static void _bind_methods();

  public:
#ifdef TOOLS_ENABLED
    struct Cache
    {
        String full_path;
        Vector4i guid;
        Cache() = default;
        Cache(FMOD_STUDIO_VCA *p_vca);
    };
#endif

    VCA();
    // Handle validity
    static bool is_valid(size_t p_handle);

    // Property access
    static Vector4i get_id(size_t p_handle);
    static String get_path(size_t p_handle);

    // Playback control
    static float get_volume(size_t p_handle);
    static float get_final_volume(size_t p_handle);
    static FMOD_RESULT set_volume(size_t p_handle, float p_volume);
};
} // namespace Studio
} // namespace FmodGodot
