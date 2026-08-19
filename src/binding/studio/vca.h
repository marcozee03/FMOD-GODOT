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
class VCA : public RefCounted
{
    GDCLASS(VCA, RefCounted)
  private:
    // Constructor made private so user cannot statically instance the class.
    FMOD_STUDIO_VCA *handle;

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
    VCA(FMOD_STUDIO_VCA *p_handle);
    operator FMOD_STUDIO_VCA *() const
    {
        return handle;
    }
    // Handle validity
    bool is_valid() const;

    // Property access
    Vector4i get_id() const;
    String get_path() const;

    // Playback control
    float get_volume() const;
    float get_final_volume() const;
    FMOD_RESULT set_volume(float p_volume);
};
} // namespace Studio
} // namespace FmodGodot
