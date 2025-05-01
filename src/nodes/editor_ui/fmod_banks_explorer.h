#pragma once
#include <classes/file_dialog.hpp>
#include <classes/v_box_container.hpp>
#include <classes/button.hpp>
#include <classes/item_list.hpp>
using namespace godot;
namespace FMODGodot
{
    class BanksExplorer : public Control
    {
        GDCLASS(BanksExplorer, Control);

    private:
        ItemList *list;
        Button *button;
        VBoxContainer *vbox;
        FileDialog *fileDialog;

        bool add_bank(String path);
        static bool contains(ItemList *list, String text);

    protected:
        static void _bind_methods();

    public:
        PackedStringArray path_list();
        BanksExplorer(/* args */);
        ~BanksExplorer();
        void on_item_clicked(int index, Vector2 atPosition, int mouseButtonIndex);
        void add_banks(PackedStringArray paths);
        void clear();
        virtual Vector2 _get_minimum_size() const override;
    };

}