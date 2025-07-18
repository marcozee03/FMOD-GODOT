#include "fmod_editor_cache.h"
#include "fmod_object.h"
#include <godot_cpp/variant/packed_string_array.hpp>
#include <variant/utility_functions.hpp>
#include <iostream>
namespace FmodGodot
{
    FmodEditorCache::FmodEditorCache()
    {
    }
    FmodEditorCache::~FmodEditorCache()
    {
    }

    void FmodEditorCache::add(const Event &p_event)
    {
        event_cache.add_data(p_event.full_path, p_event);
    }

    void FmodEditorCache::add(const Bank &p_Bank)
    {
        bank_cache.add_data(p_Bank.full_path, p_Bank);
    }

    void FmodEditorCache::add(const Parameter &p_parameter)
    {
        parameter_cache.add_data(p_parameter.full_path, p_parameter);
    }

    void FmodEditorCache::add(const VCA &p_vca)
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
    void FmodEditorCache::print()
    {
        event_cache.print();
        bank_cache.print();
        vca_cache.print();
        parameter_cache.print();
    }

    const PathTree<Event> FmodEditorCache::get_event_cache() const
    {
        return event_cache;
    }
    const PathTree<Bank> FmodEditorCache::get_bank_cache() const
    {
        return bank_cache;
    }
    const PathTree<Parameter> FmodEditorCache::get_parameter_cache() const
    {
        return parameter_cache;
    }
    const PathTree<VCA> FmodEditorCache::get_vca_cache()
    {
        return vca_cache;
    }

    Event FmodEditorCache::get_event(const String &path) const
    {
        return event_cache[path];
    }

    Bank FmodEditorCache::get_bank(const String &path) const
    {
        return bank_cache[path];
    }

    Parameter FmodEditorCache::get_parameter(const String &path) const
    {
        return parameter_cache[path];
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
}