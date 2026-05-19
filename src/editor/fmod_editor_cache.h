#pragma once
#ifdef TOOLS_ENABLED
#include "fmod_object.h"
#include "path_tree.h"
namespace FmodGodot
{
class FmodEditorCache
{

  private:
    PathTree<Event> event_cache;
    PathTree<Bank> bank_cache;
    PathTree<Parameter> parameter_cache;
    PathTree<VCA> vca_cache;

  public:
    FmodEditorCache(/* args */);
    ~FmodEditorCache();
    void add(const Event &p_event);
    void add(const Bank &p_Bank);
    void add(const Parameter &p_parameter);
    void add(const VCA &p_vca);
    void clear();
    void refresh();
    void print();
    const PathTree<Event> get_event_cache() const;
    const PathTree<Bank> get_bank_cache() const;
    const PathTree<Parameter> get_parameter_cache() const;
    const PathTree<VCA> get_vca_cache();
    Event get_event(const String &path) const;
    Bank get_bank(const String &path) const;
    Parameter get_parameter(const String &path) const;
    VCA get_vca(const String &path) const;
    PackedStringArray get_contents(const String &p_path) const;
};
} // namespace FmodGodot
#endif
