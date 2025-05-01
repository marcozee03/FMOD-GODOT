#include "bankloader.h"
#include "fmodengine.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
using namespace godot;
using namespace FMODGodot;
void BankLoader::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_banks"), &BankLoader::get_banks);
    ClassDB::bind_method(D_METHOD("set_banks"), &BankLoader::set_banks);
    ClassDB::add_property("BankLoader", PropertyInfo(Variant::PACKED_STRING_ARRAY, "banks", PROPERTY_HINT_NONE, "FMODBanks"), "set_banks", "get_banks");
}

BankLoader::BankLoader()
{
    banks = PackedStringArray();
}
BankLoader::~BankLoader()
{
}
void BankLoader::_enter_tree()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    for (int i = 0; i < banks.size(); i++)
    {
        FMODEngine::load_global_bank(banks[i].utf8().ptr());
    }
}
void BankLoader::_exit_tree()
{
    if (Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    for (int i = 0; i < banks.size(); i++)
    {
        FMODEngine::unload_global_bank(banks[i].utf8().ptr());
    }
}
PackedStringArray BankLoader::get_banks()
{
    return banks;
}
void BankLoader::set_banks(PackedStringArray banks)
{
    this->banks = banks;
}
