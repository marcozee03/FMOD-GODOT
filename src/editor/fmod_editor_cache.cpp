#include "binding/studio/event_description.h"
#include "binding/studio/vca.h"
#ifdef TOOLS_ENABLED
#include "fmod_editor_cache.h"
#include <godot_cpp/variant/packed_string_array.hpp>
#include <variant/utility_functions.hpp>
namespace FmodGodot
{
FmodEditorCache::FmodEditorCache()
{
}
FmodEditorCache::~FmodEditorCache()
{
}

void FmodEditorCache::add(const Studio::EventDescription::Cache &p_event)
{
    event_cache.add_data(p_event.full_path, p_event);
}

void FmodEditorCache::add(const FmodBank::Cache &p_bank)
{
    bank_cache.add_data(p_bank.full_path, p_bank);
}

void FmodEditorCache::add(const ParameterCache &p_parameter)
{
    parameter_cache.add_data(p_parameter.full_path, p_parameter);
}

void FmodEditorCache::add(const Studio::VCA::Cache &p_vca)
{
    vca_cache.add_data(p_vca.full_path, p_vca);
}

void FmodEditorCache::clear()
{
    bank_cache.clear();
    event_cache.clear();
    vca_cache.clear();
    parameter_cache.clear();
}
const PathTree<Studio::EventDescription::Cache> FmodEditorCache::get_event_cache() const
{
    return event_cache;
}
const PathTree<FmodBank::Cache> FmodEditorCache::get_bank_cache() const
{
    return bank_cache;
}
const PathTree<ParameterCache> FmodEditorCache::get_parameter_cache() const
{
    return parameter_cache;
}
const PathTree<Studio::VCA::Cache> FmodEditorCache::get_vca_cache()
{
    return vca_cache;
}

Studio::EventDescription::Cache FmodEditorCache::get_event(const String &p_path) const
{
    return event_cache[p_path];
}

FmodBank::Cache FmodEditorCache::get_bank(const String &p_path) const
{
    return bank_cache[p_path];
}

ParameterCache FmodEditorCache::get_parameter(const String &p_path) const
{
    return parameter_cache[p_path];
}
Studio::VCA::Cache FmodEditorCache::get_vca(const String &p_path) const
{
    return vca_cache[p_path];
}

PackedStringArray FmodEditorCache::get_contents(const String &p_path) const
{
    if (p_path.begins_with("event:"))
    {
        return event_cache.get_contents(p_path);
    }
    else if (p_path.begins_with("vca:"))
    {
        return vca_cache.get_contents(p_path);
    }
    else if (p_path.begins_with("bank:"))
    {
        return bank_cache.get_contents(p_path);
    }
    else if (p_path.begins_with("param:"))
    {
        return parameter_cache.get_contents(p_path);
    }
    else
    {
        return {};
    }
}
} // namespace FmodGodot
#endif
