#pragma once
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/h_flow_container.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/label.hpp>
using namespace godot;
namespace FmodGodot
{
    class FmodObjectDetails : public VBoxContainer
    {
        GDCLASS(FmodObjectDetails, VBoxContainer);

    private:
        HBoxContainer *box;
        TextureRect *icon;
        Label *header;
        HFlowContainer *flowlayout;

    protected:
        static void _bind_methods();

    public:
        FmodObjectDetails(/* args */);
        ~FmodObjectDetails();

        void display_fmod_object(const String &p_path);
    };
}
#endif