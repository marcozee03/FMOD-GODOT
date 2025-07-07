#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "fmod_event_selector.h"
#include "fmod_event_tree.h"
#include <godot_cpp/classes/button.hpp>
// #include <godot_cpp/classes/event_s
using namespace godot;
namespace FmodGodot
{
    class EventPathSelectorProperty : public EditorProperty
    {
        GDCLASS(EventPathSelectorProperty,EditorProperty)
    private:
        FmodEventPathSelector *eventSelector;
        String currentValue;
        bool updating = false;
        void on_text_changed(String newText);

    protected:
       static void _bind_methods();

    public:
        EventPathSelectorProperty();
        ~EventPathSelectorProperty();
        virtual void _update_property() override;
    };
}
#endif