#pragma once
#include "fmod_object_selector.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
using namespace godot;
namespace FmodGodot
{
class FmodPathSelectorProperty : public EditorProperty
{
    GDCLASS(FmodPathSelectorProperty, EditorProperty)
  private:
    FmodObjectSelector *objectSelector;
    String currentValue;
    bool updating = false;
    void _fmod_guid_and_path_changed(const Vector4i &p_guid, const String &p_path);

  protected:
    static void _bind_methods();

  public:
    FmodPathSelectorProperty();
    FmodPathSelectorProperty(FmodObjectTree::DisplayFlags p_flags);
    ~FmodPathSelectorProperty();
    virtual void _update_property() override;
};
} // namespace FmodGodot
