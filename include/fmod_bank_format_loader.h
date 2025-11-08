#include "fmod_bank.h"
#include <godot_cpp/classes/resource_format_loader.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodBankFormatLoader : public ResourceFormatLoader
{
    GDCLASS(FmodBankFormatLoader, ResourceFormatLoader)
  private:
    /* data */
  protected:
    static void _bind_methods();

  public:
    FmodBankFormatLoader(/* args */);
    ~FmodBankFormatLoader();

    bool _exists(const String &path) const override;
    PackedStringArray _get_recognized_extensions() const override;
    String _get_resource_script_class(const String &path) const override;
    String _get_resource_type(const String &path) const override;
    bool _handles_type(const StringName &p_type) const override;
    bool _recognize_path(const String &p_path, const StringName &p_type) const override;
    Variant _load(const String &path, const String &original_path, bool use_sub_threads,
                  int32_t cache_mode) const override;
};
} // namespace FmodGodot
