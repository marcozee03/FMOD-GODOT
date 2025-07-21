
#ifdef TOOLS_ENABLED
#include "fmod_event_browser.h"
#include <classes/button.hpp>
#include "fmod_editor_interface.h"
#include <classes/spin_box.hpp>
#include <classes/label.hpp>
#include <classes/project_settings.hpp>
#include "fmod_string_names.h"
using namespace godot;
namespace FmodGodot
{
    FmodGodot::FmodEventBrowser::FmodEventBrowser()
    {
        top_buttons = memnew(HBoxContainer);
        add_child(top_buttons, false, INTERNAL_MODE_FRONT);
        Button *refresh = memnew(Button());
        refresh->connect("pressed", callable_mp(FmodEditorInterface::get_singleton(), &FmodEditorInterface::refresh));
        refresh->set_text("Refresh");
        top_buttons->add_child(refresh);

        Button *build = memnew(Button());
        build->set_text("Build Banks");
        build->set_tooltip_text("The FMOD Studio project build path must be set inside this godot project");
        top_buttons->add_child(build);
        build->connect("pressed", callable_mp(FmodEditorInterface::get_singleton(), &FmodEditorInterface::build_banks));
        build->set_disabled(true);
        Button *restart_server = memnew(Button());
        restart_server->set_text("Restart Server");
        restart_server->set_tooltip_text("restarts the fmod audio server");
        top_buttons->add_child(restart_server);
        restart_server->connect("pressed", callable_mp(FmodEditorInterface::get_singleton(), &FmodEditorInterface::restart_server));

        split = memnew(HBoxContainer());
        split->set_v_size_flags(SIZE_EXPAND_FILL);
        add_child(split);
        tree = memnew(EventTree());
        details = memnew(FmodObjectDetails());
        split->add_child(tree);
        tree->set_display_flags(EventTree::DisplayFlags::BANKS | EventTree::EVENTS | EventTree::VCAS | EventTree::GLOBAL_PARAMETERS);
        tree->set_h_size_flags(SIZE_EXPAND_FILL);
        split->add_child(details);
        tree->connect("fmod_object_selected", Callable(details, "display_fmod_object"), CONNECT_PERSIST);
    }
    void FmodGodot::FmodEventBrowser::_bind_methods()
    {
    }
}
#endif