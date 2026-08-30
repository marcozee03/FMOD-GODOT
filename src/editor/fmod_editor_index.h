#pragma once
#include "bank.h"
#include "binding/studio/event_description.h"
#include "binding/studio/parameter_cache.h"
#include "binding/studio/vca.h"
#include "path_tree.h"
#include "variant/vector4i.hpp"
namespace FmodGodot
{
class FmodEditorIndex
{

  private:
    PathTree<Vector4i> tree;

    AHashMap<Vector4i, Studio::StudioEventDescription::Cache> event_cache;
    AHashMap<Vector4i, Studio::StudioBank::Cache> bank_cache;
    AHashMap<Vector4i, ParameterCache> parameter_cache;
    AHashMap<Vector4i, Studio::StudioVCA::Cache> vca_cache;

    enum FmodObjectType
    {
        FMOD_OBJECT_EVENT,
        FMOD_OBJECT_BANK,
        FMOD_OBJECT_PARAMETER,
        FMOD_OBJECT_VCA,
        FMOD_OBJECT_UNKNOWN,
    };
    struct FmodObject
    {
        FmodObjectType object_type = FMOD_OBJECT_UNKNOWN;
        union {
          public:
            Studio::StudioEventDescription::Cache const *event;
            Studio::StudioBank::Cache const *bank;
            ParameterCache const *parameter;
            Studio::StudioVCA::Cache const *vca;
        };
    };

  public:
    FmodEditorIndex(/* args */);
    ~FmodEditorIndex();
    void add(const Studio::StudioEventDescription::Cache &p_event);
    void add(const Studio::StudioBank::Cache &p_bank);
    void add(const ParameterCache &p_parameter);
    void add(const Studio::StudioVCA::Cache &p_vca);
    void clear();
    void refresh(FMOD_STUDIO_SYSTEM *p_studio_system);
    Studio::StudioEventDescription::Cache get_event(const String &p_path) const;
    Studio::StudioEventDescription::Cache get_event(const Vector4i &p_guid) const;
    Studio::StudioBank::Cache get_bank(const String &p_path) const;
    Studio::StudioBank::Cache get_bank(const Vector4i &p_guid) const;
    ParameterCache get_parameter(const String &p_path) const;
    ParameterCache get_parameter(const Vector4i &p_guid) const;
    Studio::StudioVCA::Cache get_vca(const String &p_path) const;
    Studio::StudioVCA::Cache get_vca(const Vector4i &p_guid) const;
    PackedStringArray get_contents(const String &p_path) const;
    FmodObject get_object(const String &p_path) const;
    FmodObject get_object(const Vector4i &p_guid) const;
    Vector4i lookup_id(const String &p_path) const;
    String lookup_path(const Vector4i &p_guid) const;
};
} // namespace FmodGodot
