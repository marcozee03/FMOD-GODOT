#include "classes/control.hpp"
#include "fmod_event_previewer.h"
#include "variant/callable_method_pointer.hpp"
#include "variant/packed_string_array.hpp"
#ifdef TOOLS_ENABLED
#include "fmod_event_tree.h"
#include "fmod_project_explorer.h"
#include <classes/button.hpp>
#include <classes/label.hpp>
#include <classes/project_settings.hpp>
#include <classes/spin_box.hpp>
using namespace godot;
namespace FmodGodot
{
FmodGodot::FmodProjectExplorer::FmodProjectExplorer()
{
    set_v_size_flags(SIZE_EXPAND_FILL);
    tree = memnew(EventTree());
    tree->set_display_flags(EventTree::DisplayFlags::BANKS | EventTree::EVENTS | EventTree::VCAS |
                            EventTree::GLOBAL_PARAMETERS);
    tree->set_stretch_ratio(2.0);
    add_child(tree);

    previewer = memnew(FmodEventPreviewer);
    previewer->set_h_size_flags(SIZE_EXPAND_FILL);
    previewer->hide();
    tree->connect("fmod_object_selected", callable_mp(previewer, &FmodEventPreviewer::set_event_path), CONNECT_PERSIST);

    details = memnew(FmodObjectDetails());
    tree->set_h_size_flags(SIZE_EXPAND_FILL);
    add_child(details);
    tree->connect("fmod_object_selected", Callable(details, "display_fmod_object"), CONNECT_PERSIST);
    tree->connect("fmod_object_selected", callable_mp(this, &FmodProjectExplorer::emit_object_selected));
    tree->connect("fmod_object_activated", callable_mp(this, &FmodProjectExplorer::emit_object_activated));
    add_child(previewer);
}

void FmodProjectExplorer::emit_object_selected(const String &fmod_object_path)
{
    emit_signal("fmod_object_selected", fmod_object_path);
}
void FmodProjectExplorer::emit_object_activated(const String &fmod_object_path)
{
    emit_signal("fmod_object_activated", fmod_object_path);
}
void FmodProjectExplorer::set_display_flags(EventTree::DisplayFlags flags)
{
    tree->set_display_flags(flags);
}
void FmodGodot::FmodProjectExplorer::refresh()
{
    tree->LoadEvents();
}
void FmodGodot::FmodProjectExplorer::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("fmod_object_activated", PropertyInfo(Variant::STRING, "fmod_object_path")));
    ADD_SIGNAL(MethodInfo("fmod_object_selected", PropertyInfo(Variant::STRING, "fmod_object_path")));
}
} // namespace FmodGodot
#endif
