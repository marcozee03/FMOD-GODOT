#include "fmod_editor_index.h"
#include "bank.h"
#include "binding/studio/event_description.h"
#include "binding/studio/vca.h"
#include "core/error_macros.hpp"
#include "fmod_audio_server.h"
#include "fmod_studio_common.h"
#include "parameter_cache.h"
#include "variant/string.hpp"
#include "variant/variant.hpp"
#include "variant/vector4i.hpp"
#include <godot_cpp/variant/packed_string_array.hpp>
#include <variant/utility_functions.hpp>
namespace FmodGodot
{
FmodEditorIndex::FmodEditorIndex()
{
}
FmodEditorIndex::~FmodEditorIndex()
{
}

void FmodEditorIndex::add(const Studio::StudioEventDescription::Cache &p_event)
{
    ERR_FAIL_COND_MSG(p_event.full_path.is_empty(),
                      vformat("Tried caching event with GUID: %s but could not find String path. The parent master "
                              "strings banks might not be loaded.",
                              fmod_guid_to_string(p_event.guid)));
    tree.add_data(p_event.full_path, p_event.guid);
    event_cache[p_event.guid] = p_event;
}

void FmodEditorIndex::add(const Studio::StudioBank::Cache &p_bank)
{
    ERR_FAIL_COND_MSG(p_bank.full_path.is_empty(),
                      vformat("Tried caching bank with GUID: %s but could not find String path. The parent master "
                              "strings banks might not be loaded.",
                              fmod_guid_to_string(p_bank.guid)));

    tree.add_data(p_bank.full_path, p_bank.guid);
    bank_cache[p_bank.guid] = p_bank;
}

void FmodEditorIndex::add(const ParameterCache &p_parameter)
{
    ERR_FAIL_COND_MSG(p_parameter.name.is_empty(),
                      vformat("Tried caching parameter with GUID: %s but could not find String path. The parent master "
                              "strings banks might not be loaded.",
                              fmod_guid_to_string(p_parameter.guid)));
    tree.add_data("parameter:/" + p_parameter.name, p_parameter.guid);
    parameter_cache[p_parameter.guid] = p_parameter;
}

void FmodEditorIndex::add(const Studio::StudioVCA::Cache &p_vca)
{
    ERR_FAIL_COND_MSG(p_vca.full_path.is_empty(),
                      vformat("Tried caching VCA with GUID: %s but could not find String path. The parent master "
                              "strings banks might not be loaded.",
                              fmod_guid_to_string(p_vca.guid)));
    tree.add_data(p_vca.full_path, p_vca.guid);
    vca_cache[p_vca.guid] = p_vca;
}

void FmodEditorIndex::clear()
{
    bank_cache.clear();
    event_cache.clear();
    vca_cache.clear();
    parameter_cache.clear();
    tree.clear();
}
void FmodEditorIndex::refresh(FMOD_STUDIO_SYSTEM *p_studio_system)
{
    clear();
    if (!FMOD_Studio_System_IsValid(p_studio_system))
    {
        return;
    }
    int bank_count;
    FMOD_Studio_System_GetBankCount(p_studio_system, &bank_count);
    if (bank_count <= 0)
    {
        return;
    }
    FMOD_STUDIO_BANK **banks = memnew_arr(FMOD_STUDIO_BANK *, bank_count);
    FMOD_Studio_System_GetBankList(p_studio_system, banks, bank_count, &bank_count);
    for (int i = 0; i < bank_count; i++)
    {

        Studio::StudioBank::Cache bank(banks[i]);
        add(bank);
        {
            int event_count;
            FMOD_Studio_Bank_GetEventCount(banks[i], &event_count);
            if (event_count > 0)
            {
                FMOD_STUDIO_EVENTDESCRIPTION **descriptions = memnew_arr(FMOD_STUDIO_EVENTDESCRIPTION *, event_count);
                FMOD_Studio_Bank_GetEventList(banks[i], descriptions, event_count, &event_count);
                for (int j = 0; j < event_count; j++)
                {
                    FmodGodot::Studio::StudioEventDescription::Cache event(descriptions[j]);
                    add(event);
                }
                memdelete_arr(descriptions);
            }
        }
        {
            int vca_count;
            FMOD_Studio_Bank_GetVCACount(banks[i], &vca_count);
            if (vca_count > 0)
            {
                FMOD_STUDIO_VCA **vcas = memnew_arr(FMOD_STUDIO_VCA *, vca_count);
                FMOD_Studio_Bank_GetVCAList(banks[i], vcas, vca_count, &vca_count);
                for (int j = 0; j < vca_count; j++)
                {
                    FmodGodot::Studio::StudioVCA::Cache vca(vcas[j]);
                    add(vca);
                }
                memdelete_arr(vcas);
            }
        }
    }

    int p_count;
    FMOD_Studio_System_GetParameterDescriptionCount(p_studio_system, &p_count);
    if (p_count > 0)
    {
        FMOD_STUDIO_PARAMETER_DESCRIPTION *params = memnew_arr(FMOD_STUDIO_PARAMETER_DESCRIPTION, p_count);
        FMOD_Studio_System_GetParameterDescriptionList(p_studio_system, params, p_count, &p_count);
        for (int i = 0; i < p_count; i++)
        {
            add(ParameterCache(params[i]));
        }
        memdelete_arr(params);
    }
    memdelete_arr(banks);
}

Studio::StudioEventDescription::Cache FmodEditorIndex::get_event(const String &p_path) const
{
    if (!tree.has(p_path))
    {
        return Studio::StudioEventDescription::Cache();
    }
    return get_event(tree[p_path]);
}
Studio::StudioEventDescription::Cache FmodEditorIndex::get_event(const Vector4i &p_guid) const
{
    if (!event_cache.has(p_guid))
    {
        return Studio::StudioEventDescription::Cache();
    }
    return event_cache[p_guid];
}

Studio::StudioBank::Cache FmodEditorIndex::get_bank(const String &p_path) const
{
    if (!tree.has(p_path))
    {
        return Studio::StudioBank::Cache();
    }
    return get_bank(tree[p_path]);
}
Studio::StudioBank::Cache FmodEditorIndex::get_bank(const Vector4i &p_guid) const
{
    if (!bank_cache.has(p_guid))
    {
        return Studio::StudioBank::Cache();
    }
    return bank_cache[p_guid];
}

ParameterCache FmodEditorIndex::get_parameter(const String &p_path) const
{
    if (!tree.has(p_path))
    {
        return ParameterCache();
    }
    return get_parameter(tree[p_path]);
}
ParameterCache FmodEditorIndex::get_parameter(const Vector4i &p_guid) const
{
    if (!parameter_cache.has(p_guid))
    {
        return ParameterCache();
    }
    return parameter_cache[p_guid];
}

Studio::StudioVCA::Cache FmodEditorIndex::get_vca(const String &p_path) const
{
    if (!tree.has(p_path))
    {
        return Studio::StudioVCA::Cache();
    }
    return vca_cache[tree[p_path]];
}
Studio::StudioVCA::Cache FmodEditorIndex::get_vca(const Vector4i &p_guid) const
{
    if (!vca_cache.has(p_guid))
    {
        return Studio::StudioVCA::Cache();
    }
    return vca_cache[p_guid];
}

PackedStringArray FmodEditorIndex::get_contents(const String &p_path) const
{
    return tree.get_contents(p_path);
}
FmodEditorIndex::FmodObject FmodEditorIndex::get_object(const String &p_path) const
{
    FmodObject ret;
    Vector4i guid = tree[p_path];
    if (p_path.begins_with("event:/") || p_path.begins_with("snapshot:/"))
    {
        ret.object_type = FMOD_OBJECT_EVENT;
        if (event_cache.has(guid))
        {
            ret.event = &event_cache[tree[p_path]];
        }
    }
    else if (p_path.begins_with("bank:/"))
    {
        ret.object_type = FMOD_OBJECT_BANK;

        if (bank_cache.has(guid))
        {
            ret.bank = &bank_cache[tree[p_path]];
        }
    }
    else if (p_path.begins_with("vca:/"))
    {
        ret.object_type = FMOD_OBJECT_VCA;

        if (vca_cache.has(guid))
        {
            ret.vca = &vca_cache[tree[p_path]];
        }
    }
    else if (p_path.begins_with("parameter:/"))
    {
        ret.object_type = FMOD_OBJECT_PARAMETER;
        if (parameter_cache.has(guid))
        {
            ret.parameter = &parameter_cache[tree[p_path]];
        }
    }
    return ret;
}
godot::Vector4i FmodEditorIndex::lookup_id(const String &p_path) const
{
    FMOD_GUID guid;
    if (FMOD_OK !=
        FMOD_Studio_System_LookupID(FmodAudioServer::get_singleton()->get_studio(), p_path.utf8().ptr(), &guid))
    {
        if (tree.has(p_path))
        {
            return tree[p_path];
        }
        return Vector4i();
    }
    return cast_to_vector4i(guid);
}
godot::String FmodEditorIndex::lookup_path(const Vector4i &p_guid) const
{
    String path = "";
    {
        char *strpath = nullptr;
        int retrieved = 0;
        FMOD_Studio_System_LookupPath(FmodAudioServer ::get_singleton()->get_studio(),
                                      reinterpret_cast<const FMOD_GUID *>(&p_guid), nullptr, 0, &retrieved);
        if (retrieved > 0)
        {
            int size = retrieved;
            strpath = ::godot ::memnew_arr_template<char>(size);
            if (FMOD_Studio_System_LookupPath(FmodAudioServer ::get_singleton()->get_studio(),
                                              reinterpret_cast<const FMOD_GUID *>(&p_guid), strpath, size,
                                              &retrieved) == FMOD_OK)
            {
                path = String ::utf8(strpath, retrieved);
                memdelete_arr(strpath);
                return path;
            }
        }
    };
    if (bank_cache.has(p_guid))
    {
        return bank_cache[p_guid].full_path;
    }
    if (event_cache.has(p_guid))
    {
        return event_cache[p_guid].full_path;
    }
    if (parameter_cache.has(p_guid))
    {
        return "parameter:/" + parameter_cache[p_guid].name;
    }
    if (vca_cache.has(p_guid))
    {
        return vca_cache[p_guid].full_path;
    }
    return "";
}

} // namespace FmodGodot
