#include "classes/ref_counted.hpp"
#include "fmod_common.h"
#include "fmod_studio_common.h"
#include <godot_cpp/variant/vector4i.hpp>

using namespace godot;
namespace FmodGodot
{
class VCA : public RefCounted
{
    GDCLASS(VCA, RefCounted)
  private:
    // Constructor made private so user cannot statically instance the class.
    VCA();
    VCA(FMOD_STUDIO_VCA *p_handle);
    FMOD_STUDIO_VCA *handle;

  protected:
    static void _bind_methods();

  public:
    // Handle validity
    bool is_valid() const;

    // Property access
    Vector4i get_id() const;
    String get_path() const;

    // Playback control
    float get_volume() const;
    float get_final_volume() const;
    FMOD_RESULT set_volume(float volume);
};
} // namespace FmodGodot
