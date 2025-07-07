#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/variant.hpp>
using namespace godot;

#define BIND_PROPERTY(property_name, class, variantType)                                                 \
    ClassDB::bind_method(D_METHOD("get_" #property_name), &class ::get_##property_name);                 \
    ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &class ::set_##property_name); \
    ClassDB::add_property(#class, PropertyInfo(variantType, #property_name, PROPERTY_HINT_NONE), "set_" #property_name, "get_" #property_name);

#define BIND_PROPERTY_WITH_HINT(property_name, class, variantType, property_hint, hint_string)           \
    ClassDB::bind_method(D_METHOD("get_" #property_name), &class ::get_##property_name);                 \
    ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &class ::set_##property_name); \
    ClassDB::add_property(#class, PropertyInfo(variantType, #property_name, property_hint, hint_string), "set_" #property_name, "get_" #property_name);

#define BIND_BOOL_PROPERTY(property_name, class)                                                         \
    ClassDB::bind_method(D_METHOD("is_" #property_name), &class ::is_##property_name);                   \
    ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &class ::set_##property_name); \
    ClassDB::add_property(#class, PropertyInfo(godot::Variant::Type::BOOL, #property_name, PROPERTY_HINT_NONE), "set_" #property_name, "is_" #property_name);

#define BIND_BOOL_PROPERTY_WITH_HINT(property_name, class, property_hint, hint_string)                   \
    ClassDB::bind_method(D_METHOD("is_" #property_name), &class ::is_##property_name);                   \
    ClassDB::bind_method(D_METHOD("set_" #property_name, #property_name), &class ::set_##property_name); \
    ClassDB::add_property(#class, PropertyInfo(godot::Variant::Type::BOOL, #property_name, property_hint, hint_string), "set_" #property_name, "is_" #property_name);
    