#include <fmod.h>
#include <fmod_studio.h>
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
#include <classes/window.hpp>
#include "fmod_event_tree.h"
#include "fmod_globals.h"
#include "fmod_audio_server.h"
#include "fmod_editor_cache.h"
#include "classes/tree_item.hpp"
using namespace godot;
namespace FmodGodot
{
    void EventTree::_bind_methods()
    {
    }
    EventTree::EventTree()
    {
        set_columns(2);
        set_select_mode(SelectMode::SELECT_ROW);
        // BIND_BITFIELD_FLAG(BANKS);
        // BIND_BITFIELD_FLAG(EVENTS);
        // BIND_BITFIELD_FLAG(VCAS);
        // BIND_BITFIELD_FLAG(GLOBAL_PARAMETERS);
    }
    EventTree::~EventTree()
    {
    }
    namespace
    {
        void recurOverEvents(TreeItem *root, const PackedStringArray &contents, const String &current_path)
        {
        }
    }
    void EventTree::set_display_flags(int p_flags)
    {
        if (display_flags != p_flags)
        {
            // display_flags = static_cast<DisplayFlags>(p_flags);
            // this->clear();
            // root = create_item();
            // FmodEditorCache cache;
            // if (display_flags & EVENTS)
            // {
            //     TreeItem *item = root->create_child(-1);
            //     item->set_text(0, "event:/");
            //     auto event_contents = cache.get_contents("event:/");
            //     String current_path = "event:/";
            //     for (auto str : event_contents)
            //     {
            //         auto child = item->create_child();
            //         child->set_text(0, str);
            //         PackedStringArray subcontents = cache.get_contents(current_path + str);
            //         if (!cache.get_contents(str).is_empty())
            //         {
            //             recurOverEvents(child, subcontents, current_path + str + '/');
            //         }
            //     }
            // }
            // if (display_flags & BANKS)
            // {
            // }

            // if (display_flags & VCAS)
            // {
            // }
            // if (display_flags & GLOBAL_PARAMETERS)
            // {
            // }
        }
    }
    int EventTree::get_display_flags() const
    {
        return static_cast<DisplayFlags>(display_flags);
    }
    bool EventTree::LoadEvents()
    {
        FMOD_STUDIO_SYSTEM *studio = FmodAudioServer::get_global_studio();
        ProjectSettings *pj = ProjectSettings::get_singleton();
        FMOD_STUDIO_BANK *MasterStrings;
        FMOD_RESULT result = FMOD_Studio_System_GetBank(studio, "bank:/Master.strings", &MasterStrings);
        // char path[128];
        // int r;
        // MasterStrings->getPath(path, 128, &r);
        // print_line(path);
        if (result == FMOD_OK)
        {
            int count = 0;
            FMOD_GUID guid;
            int size = 128;
            int retrieved = 0;
            char *eventPath = new char[size];
            FMOD_Studio_Bank_GetStringCount(MasterStrings, &count);
            for (int i = 0; i < count; i++)
            {
                FMOD_GET_FULL_STRING_FUNC_CALL(FMOD_Studio_Bank_GetStringInfo(MasterStrings, i, &guid, eventPath, size, &retrieved), eventPath, size, retrieved);
                String g_eventPath = String(eventPath);
                if (!g_eventPath.begins_with("event:/"))
                {
                    continue;
                }
                auto item = create_item(root);
                item->set_cell_mode(1, TreeItem::TreeCellMode::CELL_MODE_ICON);
                item->set_icon(1, ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg"));
                item->set_text(0, eventPath);
                item->set_metadata(0, cast_to_Vector4i(guid));
            }
            delete[] eventPath;
            return true;
        }
        else
        {
            Window *window = (Window *)get_parent();
            window->hide();
            return false;
        }
    }
    void EventTree::_ready()
    {
        root = create_item();
        root->set_text(0, "root");
        set_hide_root(true);
        LoadEvents();
    }
}
