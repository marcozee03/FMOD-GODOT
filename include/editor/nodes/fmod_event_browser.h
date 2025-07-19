#pragma once
#define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include "classes/v_box_container.hpp"
#include "classes/h_box_container.hpp"
#include "classes/h_split_container.hpp"
#include "fmod_event_tree.h"
#include "fmod_object_details.h"
using namespace godot;
namespace FmodGodot
{
    class FmodEventBrowser : public VBoxContainer
    {
        GDCLASS(FmodEventBrowser, VBoxContainer)
    private:
        HBoxContainer *top_buttons;
        HBoxContainer *split;
        EventTree *tree;
        FmodObjectDetails *details;

    protected:
        static void _bind_methods();
    public:
        FmodEventBrowser(/* args */);
        ~FmodEventBrowser() = default;
    };
}
#endif