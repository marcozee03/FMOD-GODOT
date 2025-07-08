#include "fmod_bank.h"
#include <godot_cpp/classes/resource_format_saver.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodBankFormatSaver : public ResourceFormatSaver
    {
        GDCLASS(FmodBankFormatSaver, ResourceFormatSaver)
    private:
        /* data */
    protected:
        static void _bind_methods();
    public:
        FmodBankFormatSaver(/* args */);
        ~FmodBankFormatSaver();
        PackedStringArray _get_recognized_extensions(const Ref<Resource> &p_resource) const override;
        bool _recognize(const Ref<Resource> &p_resource) const override;
        // bool _recognize_path(const Ref<Resource> &p_resource, const String &path) const override;
        Error _save(const Ref<Resource> &p_resource, const String& path,const uint32_t p_flags) override;
    };
}