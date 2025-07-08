#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/v_box_container.hpp>
#include "bank_inspector.h"
#include <godot_cpp/classes/resource_loader.hpp>
void FmodGodot::BankInspector::_enter_tree()
{
    Studio::System *studio_system = FmodAudioServer::get_global_studio();
    FMOD::Studio::Bank *bank;
    studio_system->loadBankFile(m_bank->get_path().utf8().ptr(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    int count;
    bank->getEventCount(&count);
    FMOD::Studio::EventDescription **descriptions = new EventDescription *[count];
    bank->getEventList(descriptions, count, &count);
    int size = 128;
    int retrieved;
    char *str = new char[size];
    FMOD_RESULT err;
    auto root = create_item();
    for (int i = 0; i < count; i++)
    {
        err = descriptions[i]->getPath(str, size, &retrieved);
        if (err == FMOD_ERR_TRUNCATED)
        {
            size = retrieved;
            delete[] str;
            str = new char[size];
            descriptions[i]->getPath(str, size, &retrieved);
        }

        auto item = this->create_item(root);
        item->set_text(0, str + 7);
        item->set_icon(0, event_icon);
        int parameter_count;
        descriptions[i]->getParameterDescriptionCount(&parameter_count);
        for (int j = 0; j < parameter_count; j++)
        {
            FMOD_STUDIO_PARAMETER_DESCRIPTION param_description;
            descriptions[i]->getParameterDescriptionByIndex(j, &param_description);
            if (param_description.flags & FMOD_STUDIO_PARAMETER_GAME_CONTROLLED == 0)
            {
                continue;
            }
            auto param_item = create_item(item);
            if (param_description.flags & FMOD_STUDIO_PARAMETER_DISCRETE != 0)
            {
                param_item->set_icon(0, d_parameter_icon);
            }
            else
            {
                param_item->set_icon(0, c_parameter_icon);
            }
            param_item->set_text(0, param_description.name);
            Array values = {param_description.minimum, param_description.defaultvalue, param_description.maximum};
            param_item->set_text(1, String("[{0}...{1}...{2}]").format(values));
        }
    }
    VBoxContainer *parent = cast_to<VBoxContainer>(get_parent());
    if (parent)
    {
        parent->set_v_size_flags(SIZE_EXPAND_FILL);
        set_v_size_flags(SIZE_EXPAND_FILL);
        parent->get_parent()->print_tree_pretty();
    }
    bank->unload();
    delete[] descriptions;
    delete[] str;
}

FmodGodot::BankInspector::BankInspector() : m_bank(nullptr)
{
    set_columns(2);
    // set_custom_minimum_size({0, });
    set_v_scroll_enabled(false);
    set_v_size_flags(SIZE_EXPAND_FILL);
    set_hide_root(true);
}
FmodGodot::BankInspector::BankInspector(Ref<FmodBank> p_bank) : BankInspector()
{
    m_bank = p_bank;
    event_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
    c_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/c_parameter_icon.svg");
    d_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/d_parameter_icon.svg");
}
void FmodGodot::BankInspector::_bind_methods()
{
}
#endif