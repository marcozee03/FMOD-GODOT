// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <fmod.h>
#include <fmod_studio.h>
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
#include <classes/window.hpp>
#include "fmod_event_tree.h"
#include "fmod_globals.h"
#include "fmod_audio_server.h"
#include "fmod_editor_interface.h"
#include "fmod_editor_cache.h"
#include "classes/tree_item.hpp"
#include "globals.h"
#include "classes/editor_interface.hpp"
#include <classes/theme.hpp>
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
        emit_signal("fmod_object_activated", get_selected_path());
    }

    void EventTree::on_item_selected()
    {
        TreeItem *current = get_selected();
        if (current->get_child_count() > 0 || current->get_parent() == get_root())
        {
            return;
        }
        emit_signal("fmod_object_selected", get_selected_path());
    }

    void EventTree::_bind_methods()
    {
        BIND_BITFIELD_FLAG(BANKS);
        BIND_BITFIELD_FLAG(EVENTS);
        BIND_BITFIELD_FLAG(VCAS);
        BIND_BITFIELD_FLAG(GLOBAL_PARAMETERS);
        BIND_PROPERTY_WITH_HINT(display_flags, Variant::Type::INT, PROPERTY_HINT_FLAGS, "BANKS:1,EVENTS:2,VCAS:4,GLOBAL_PARAMETERS:8")
        ADD_SIGNAL(MethodInfo("fmod_object_activated", PropertyInfo(Variant::PACKED_STRING_ARRAY, "fmod_object_path")));
        ADD_SIGNAL(MethodInfo("fmod_object_selected", PropertyInfo(Variant::PACKED_STRING_ARRAY, "fmod_object_path")));
    }
    EventTree::EventTree()
    {
        set_columns(1);
        set_hide_root(true);
        connect("item_activated", callable_mp(this, &EventTree::on_item_activated));
        connect("item_selected", callable_mp(this, &EventTree::on_item_selected));
        set_h_scroll_enabled(false);
    }
    EventTree::~EventTree()
    {
    }
    namespace
    {
        void recurOverEvents(TreeItem *root, const FmodEditorCache *cache, const PackedStringArray &contents, const String &current_path)
        {
            for (auto str : contents)
            {
                auto child = root->create_child();
                child->set_text(0, str);
                PackedStringArray subcontents = cache->get_contents(current_path + String("/") + str);
                if (!subcontents.is_empty())
                {
                    child->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
                    recurOverEvents(child, cache, subcontents, current_path + String("/") + str);
                }
            }
        }
        void initRecur(TreeItem *root, const FmodEditorCache *cache, const String &current_path)
        {
            TreeItem *item = root->create_child(-1);
            item->set_text(0, current_path);
            item->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
            auto event_contents = cache->get_contents(current_path);
            for (const String &str : event_contents)
            {
                auto child = item->create_child();
                child->set_text(0, str);
                PackedStringArray subcontents = cache->get_contents(current_path + String("/") + str);
                if (!subcontents.is_empty())
                {
                    child->set_icon(0, EditorInterface::get_singleton()->get_editor_theme()->get_icon("Folder", "EditorIcons"));
                    recurOverEvents(child, cache, subcontents, current_path + String("/") + str);
                }
            }
        }

    }
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
        const FmodEditorCache *cache = FmodEditorInterface::get_singleton()->get_cache();
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
    String EventTree::get_selected_path()
    {
        TreeItem *current = get_selected();
        if(!current){
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
}
#endif