#pragma once
#ifdef TOOLS_ENABLED
#include "binding/studio/parameter_cache.h"
#include "fmod_bank.h"
#include "binding/studio/event_description.h"
#include "binding/studio/vca.h"
#include "fmod_bank.h"
#include "path_tree.h"
namespace FmodGodot
{
class FmodEditorCache
{

  private:
    PathTree<Studio::EventDescription::Cache> event_cache;
    PathTree<FmodBank::Cache> bank_cache;
    PathTree<ParameterCache> parameter_cache;
    PathTree<Studio::VCA::Cache> vca_cache;

  public:
    FmodEditorCache(/* args */);
    ~FmodEditorCache();
    void add(const Studio::EventDescription::Cache &p_event);
    void add(const FmodBank::Cache &p_bank);
    void add(const ParameterCache &p_parameter);
    void add(const Studio::VCA::Cache &p_vca);
    void clear();
    void refresh();
    const PathTree<Studio::EventDescription::Cache> get_event_cache() const;
    const PathTree<FmodBank::Cache> get_bank_cache() const;
    const PathTree<ParameterCache> get_parameter_cache() const;
    const PathTree<Studio::VCA::Cache> get_vca_cache();
    Studio::EventDescription::Cache get_event(const String &p_path) const;
    FmodBank::Cache get_bank(const String &p_path) const;
    ParameterCache get_parameter(const String &p_path) const;
    Studio::VCA::Cache get_vca(const String &p_path) const;
    PackedStringArray get_contents(const String &p_path) const;
};
} // namespace FmodGodot
#endif
