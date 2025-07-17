// #define TOOLS_ENABLED
#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/v_box_container.hpp>
#include "bank_inspector.h"
#include <godot_cpp/classes/resource_loader.hpp>
void FmodGodot::BankInspector::_enter_tree()
{
    FMOD_STUDIO_SYSTEM *studio_system = FmodAudioServer::get_global_studio();
    FMOD_STUDIO_BANK *bank;
    if (FMOD_Studio_System_LoadBankFile(studio_system, m_bank->get_path().utf8(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank) == FMOD_ERR_EVENT_ALREADY_LOADED)
    {
        FMOD_Studio_System_GetBank(studio_system, (String("bank:/") + m_bank->get_path().get_file().get_basename()).utf8(), &bank);
    }
    int count;
    FMOD_Studio_Bank_GetEventCount(bank, &count);
    FMOD_STUDIO_EVENTDESCRIPTION **descriptions = new FMOD_STUDIO_EVENTDESCRIPTION *[count];
    FMOD_Studio_Bank_GetEventList(bank, descriptions, count, &count);
    int size = 128;
    int retrieved;
    char *str = new char[size];
    FMOD_RESULT err;
    auto root = create_item();
    FMOD_GET_FULL_STRING(FMOD_Studio_Bank_GetPath, bank, str, size, retrieved);
    root->set_icon(0, bank_icon);
    root->set_text(0, str);
    for (int i = 0; i < count; i++)
    {
        FMOD_GET_FULL_STRING(FMOD_Studio_EventDescription_GetPath, descriptions[i], str, size, retrieved);
        auto item = this->create_item(root);
        item->set_text(0, str + 7);
        item->set_icon(0, event_icon);
        int parameter_count;
        FMOD_Studio_EventDescription_GetParameterDescriptionCount(descriptions[i], &parameter_count);
        for (int j = 0; j < parameter_count; j++)
        {
            FMOD_STUDIO_PARAMETER_DESCRIPTION param_description;
            FMOD_Studio_EventDescription_GetParameterDescriptionByIndex(descriptions[i], j, &param_description);
            if (param_description.type != FMOD_STUDIO_PARAMETER_GAME_CONTROLLED)
            {
                continue;
            }
            auto param_item = create_item(item);
            if (param_description.flags & FMOD_STUDIO_PARAMETER_DISCRETE)
            {
                param_item->set_icon(0, d_parameter_icon);
            }
            else
            {
                param_item->set_icon(0, c_parameter_icon);
            }
            param_item->set_text(0, param_description.name);
            Array values = {param_description.minimum, param_description.maximum};
            param_item->set_text(1, String("[{0}...{1}]").format(values));
        }
    }
    VBoxContainer *parent = cast_to<VBoxContainer>(get_parent());
    if (parent)
    {
        parent->set_v_size_flags(SIZE_EXPAND_FILL);
        set_v_size_flags(SIZE_EXPAND_FILL);
    }
    delete[] descriptions;
    delete[] str;
}

FmodGodot::BankInspector::BankInspector() : m_bank(nullptr)
{
    set_columns(2);
    // set_custom_minimum_size({0, });
    set_v_scroll_enabled(false);
    set_v_size_flags(SIZE_EXPAND_FILL);
    bank_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/bank_icon.svg");
    event_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/event_icon.svg");
    c_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/c_parameter_icon.svg");
    d_parameter_icon = ResourceLoader::get_singleton()->load("res://addons/FmodGodot/icons/d_parameter_icon.svg");
}
FmodGodot::BankInspector::BankInspector(Ref<FmodBank> p_bank) : BankInspector()
{
    m_bank = p_bank;
}
void FmodGodot::BankInspector::_bind_methods()
{
}
#endif