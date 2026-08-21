#pragma once
#include "fmod_event_selector.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
using namespace godot;
namespace FmodGodot
{
class EventPathSelectorProperty : public EditorProperty
{
    GDCLASS(EventPathSelectorProperty, EditorProperty)
  private:
    FmodEventSelector *eventSelector;
    String currentValue;
    bool updating = false;
    void on_text_changed(String p_new_text);
    void on_editing_toggled(bool p_toggled_on);

  protected:
    static void _bind_methods();

  public:
    EventPathSelectorProperty();
    ~EventPathSelectorProperty();
    virtual void _update_property() override;
};
} // namespace FmodGodot
