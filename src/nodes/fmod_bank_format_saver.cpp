#include "fmod_bank_format_saver.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/dir_access.hpp>
FmodGodot::FmodBankFormatSaver::FmodBankFormatSaver()
{
}

FmodGodot::FmodBankFormatSaver::~FmodBankFormatSaver()
{
}

void FmodGodot::FmodBankFormatSaver::_bind_methods()
{
}

PackedStringArray FmodGodot::FmodBankFormatSaver::_get_recognized_extensions(const Ref<Resource> &p_resource) const
{
    return {"bank"};
}

bool FmodGodot::FmodBankFormatSaver::_recognize(const Ref<Resource> &p_resource) const
{
    return Object::cast_to<FmodBank>(*p_resource) != nullptr;
}

// bool FmodGodot::FmodBankFormatSaver::_recognize_path(const Ref<Resource> &p_resource, const String &path) const
// {
//     return false;
// }

Error FmodGodot::FmodBankFormatSaver::_save(const Ref<Resource> &p_resource, const String &path, const uint32_t p_flags)
{
    return DirAccess::copy_absolute(p_resource->get_path(), path);
}
