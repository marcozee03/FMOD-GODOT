
#include "fmod_banks_explorer.h"
#include <classes/resource_loader.hpp>
using namespace godot;
namespace FMODGodot
{
    bool BanksExplorer::add_bank(String path)
    {
        if (!BanksExplorer::contains(list, path))
        {
            list->add_item(path);
            return true;
        }
        return false;
    }

    bool BanksExplorer::contains(ItemList *list, String text)
    {
        for (int i = 0; i < list->get_item_count(); i++)
        {
            if (list->get_item_text(i) == (text))
            {
                return true;
            }
        }
        return false;
    }

    void BanksExplorer::_bind_methods()
    {
        ADD_SIGNAL(MethodInfo("paths_changed"));
        ClassDB::bind_method(D_METHOD("add_banks", "banks_paths"), &BanksExplorer::add_banks);
        ClassDB::bind_method(D_METHOD("path_list"), &BanksExplorer::path_list);
        ClassDB::bind_method(D_METHOD("on_item_clicked"), &BanksExplorer::on_item_clicked);
        ClassDB::bind_method(D_METHOD("clear"), &BanksExplorer::clear);
    }

    PackedStringArray BanksExplorer::path_list()
    {
        PackedStringArray arr;
        for (int i = 0; i < list->get_item_count(); i++)
        {
            arr.push_back(list->get_item_text(i));
        }
        return arr;
    }

    BanksExplorer::BanksExplorer()
    {
        fileDialog = memnew(FileDialog);
        this->set_custom_minimum_size({200, 200});
        fileDialog->add_filter("*.bank");
        fileDialog->set_file_mode(FileDialog::FileMode::FILE_MODE_OPEN_FILES);
        fileDialog->set_initial_position(godot::Window::WindowInitialPosition::WINDOW_INITIAL_POSITION_CENTER_PRIMARY_SCREEN);
        button = memnew(Button);
        vbox = memnew(VBoxContainer);
        list = memnew(ItemList);
        list->set_auto_height(true);
        vbox->set_anchors_and_offsets_preset(LayoutPreset::PRESET_FULL_RECT);
        add_child(fileDialog);
        fileDialog->hide();
        add_child(vbox);
        Ref<Texture2D> icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_icon.svg");
        button->set_button_icon(icon);
        button->set_text("Add banks");
        list->set_tooltip_text("right click to remove item from list");
        vbox->add_child(button);
        list->connect("item_clicked", Callable(this, "on_item_clicked"));
        vbox->add_child(list);
        list->set_v_size_flags(SizeFlags::SIZE_EXPAND_FILL);
        button->connect("pressed", Callable(fileDialog, "show"));
        fileDialog->connect("files_selected", Callable(this, "add_banks"));
    }
    BanksExplorer::~BanksExplorer()
    {
    }
    void BanksExplorer::on_item_clicked(int index, Vector2 atPosition, int mouseButtonIndex)
    {
        if (mouseButtonIndex == MOUSE_BUTTON_RIGHT)
        {
            list->remove_item(index);
            emit_signal("paths_changed");
        }
    }
    void BanksExplorer::add_banks(PackedStringArray paths)
    {
        bool emit = false;
        for (int i = 0; i < paths.size(); i++)
        {
            emit = add_bank(paths[i]) || emit;
        }
        if (emit)
        {
            emit_signal("paths_changed");
        }
    }
    void BanksExplorer::clear()
    {
        list->clear();
    }
    Vector2 BanksExplorer::_get_minimum_size() const
    {
        return vbox->get_minimum_size();
    }
}