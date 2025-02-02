#pragma once
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <classes/project_settings.hpp>
#include <classes/resource_loader.hpp>
#include <classes/window.hpp>
#include "fmod_event_tree.h"
#include "fmod_globals.h"
using namespace godot;
using namespace FMOD;
namespace FMODGodot
{
    void EventTree::_bind_methods()
    {
    }
    EventTree::EventTree()
    {
        set_columns(2);
        set_select_mode(SelectMode::SELECT_ROW);
    }
    EventTree::~EventTree()
    {
    }
    bool EventTree::LoadEvents()
    {
        Studio::System *studio;
        FMOD::Studio::System::create(&studio);
        studio->initialize(1, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
        ProjectSettings *pj = ProjectSettings::get_singleton();
        Studio::Bank *MasterStrings;
        FMOD_RESULT result = studio->loadBankFile(pj->globalize_path(pj->get_setting("Fmod/Banks/Master_Strings_Bank_Path", "").stringify()).utf8().ptr(), FMOD_STUDIO_LOAD_BANK_NORMAL, &MasterStrings);
        if (result == FMOD_OK)
        {
            int count = 0;
            FMOD_GUID guid;
            int retrieved = 128;
            char *eventPath = new char[retrieved];

            MasterStrings->getStringCount(&count);
            for (int i = 0; i < count; i++)
            {
                FMOD_RESULT result = MasterStrings->getStringInfo(i, &guid, eventPath, retrieved, &retrieved);
                if (result == FMOD_ERR_TRUNCATED)
                {
                    delete[] eventPath;
                    eventPath = new char[retrieved];
                    MasterStrings->getStringInfo(i, &guid, eventPath, retrieved, &retrieved);
                }
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
            studio->release();
            delete[] eventPath;
            return true;
        }
        else
        {
            Window *window = (Window *)get_parent();
            window->hide();
            MasterStrings->unload();
            studio->release();
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
