#include "fmod_event_tree.h"
#include "classes/editor_interface.hpp"
#include "classes/h_box_container.hpp"
#include "classes/label.hpp"
#include "classes/texture_rect.hpp"
#include "classes/tree_item.hpp"
#include "fmod_editor_index.h"
#include "fmod_editor_interface.h"
#include "globals.h"
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
#include <classes/theme.hpp>
#include <classes/window.hpp>
#include <fmod.h>
#include <fmod_studio.h>
using namespace godot;
namespace FmodGodot
{
void EventTree::on_item_activated()
{
    TreeItem *current = get_selected();
    if (current->get_child_count() > 0 || current->get_parent() == get_root())
    {
        return;
    }
    emit_signal("fmod_object_activated", get_item_path(get_selected()));
}

void EventTree::on_item_selected()
{
    TreeItem *current = get_selected();
    if (current->get_child_count() > 0 || current->get_parent() == get_root())
    {
        return;
    }
    emit_signal("fmod_object_selected", get_item_path(get_selected()));
}

void EventTree::_bind_methods()
{
    BIND_BITFIELD_FLAG(BANKS);
    BIND_BITFIELD_FLAG(EVENTS);
    BIND_BITFIELD_FLAG(VCAS);
    BIND_BITFIELD_FLAG(GLOBAL_PARAMETERS);
    BIND_PROPERTY_WITH_HINT(display_flags, Variant::Type::INT, PROPERTY_HINT_FLAGS,
                            "BANKS:1,EVENTS:2,VCAS:4,GLOBAL_PARAMETERS:8")
    ADD_SIGNAL(MethodInfo("fmod_object_activated", PropertyInfo(Variant::STRING, "fmod_object_path")));
    ADD_SIGNAL(MethodInfo("fmod_object_selected", PropertyInfo(Variant::STRING, "fmod_object_path")));
}
EventTree::EventTree()
{
    set_columns(1);
    set_hide_root(true);
    connect("item_activated", callable_mp(this, &EventTree::on_item_activated));
    connect("item_selected", callable_mp(this, &EventTree::on_item_selected));
    set_h_scroll_enabled(false);
    set_scroll_hint_mode(Tree::SCROLL_HINT_MODE_TOP);
}
EventTree::~EventTree()
{
}
namespace
{
void recurOverEvents(TreeItem *p_root, const FmodEditorIndex *p_cache, const PackedStringArray &p_contents,
                     const String &p_current_path)
{
    for (auto str : p_contents)
    {
        auto child = p_root->create_child();
        child->set_text(0, str);
        PackedStringArray subcontents = p_cache->get_contents(p_current_path + String("/") + str);
        if (!subcontents.is_empty())
        {
            child->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
            recurOverEvents(child, p_cache, subcontents, p_current_path + String("/") + str);
        }
    }
}
void initRecur(TreeItem *p_root, const FmodEditorIndex *p_cache, const String &p_current_path)
{
    TreeItem *item = p_root->create_child(-1);
    item->set_text(0, p_current_path);
    item->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
    auto event_contents = p_cache->get_contents(p_current_path);
    for (const String &str : event_contents)
    {
        auto child = item->create_child();
        child->set_text(0, str);
        PackedStringArray subcontents = p_cache->get_contents(p_current_path + String("/") + str);
        if (!subcontents.is_empty())
        {
            child->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
            recurOverEvents(child, p_cache, subcontents, p_current_path + String("/") + str);
        }
    }
}

} // namespace
void EventTree::set_display_flags(int p_flags)
{
    if (display_flags != p_flags)
    {
        display_flags = static_cast<DisplayFlags>(p_flags);
        LoadEvents();
    }
}
int EventTree::get_display_flags() const
{
    return static_cast<DisplayFlags>(display_flags);
}
void EventTree::LoadEvents()
{
    this->clear();
    create_item();
    const FmodEditorIndex *cache = FmodEditorInterface::get_singleton()->get_cache();
    String current_path;
    if (display_flags & EVENTS)
    {
        initRecur(get_root(), cache, "event:");
    }
    if (display_flags & BANKS)
    {
        initRecur(get_root(), cache, "bank:");
    }

    if (display_flags & VCAS)
    {
        initRecur(get_root(), cache, "vca:");
    }
    if (display_flags & GLOBAL_PARAMETERS)
    {
        initRecur(get_root(), cache, "param:");
    }
}
String EventTree::get_item_path(TreeItem *p_item)
{
    TreeItem *current = p_item;
    if (!current)
    {
        return "";
    }
    Vector<TreeItem *> items;
    while (current != get_root())
    {
        items.push_back(current);
        current = current->get_parent();
    }
    String str = items[items.size() - 1]->get_text(0);
    for (int i = items.size() - 2; i >= 0; i--)
    {
        str += '/' + items[i]->get_text(0);
    }
    return str;
}
Variant EventTree::_get_drag_data(const Vector2 &p_vec2)
{
    TreeItem *item = get_item_at_position(p_vec2);
    Ref<Texture2D> event_icon = get_theme_icon("FmodEvent", "EditorIcons");
    if (item)
    {
        HBoxContainer *box = memnew(HBoxContainer);
        TextureRect *text = memnew(TextureRect);
        text->set_stretch_mode(godot::TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
        if (get_item_path(item).begins_with("event:/"))
        {
            text->set_texture(get_theme_icon("FmodEvent", "EditorIcons"));
        }
        else if (get_item_path(item).begins_with("bank:/"))
        {
            text->set_texture(get_theme_icon("FmodBank", "EditorIcons"));
        }
        Label *label = memnew(Label());
        label->set_text(get_item_path(item).substr(get_item_path(item).find(":/")));
        box->add_child(label);
        box->add_child(text);
        set_drag_preview(box);
        return get_item_path(item);
    }
    return Variant();
}
} // namespace FmodGodot
