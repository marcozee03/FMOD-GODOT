#pragma once
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/variant.hpp>
using namespace godot;

#define BIND_PROPERTY(property, variantType)                                                                           \
    ClassDB::bind_method(D_METHOD("get_" #property), &self_type ::get_##property);                                     \
    ClassDB::bind_method(D_METHOD("set_" #property, #property), &self_type ::set_##property);                          \
    ClassDB::add_property(get_class_static(), PropertyInfo(variantType, #property, PROPERTY_HINT_NONE),                \
                          "set_" #property, "get_" #property);

#define BIND_PROPERTY_WITH_HINT(property, variantType, property_hint, hint_string)                                     \
    ClassDB::bind_method(D_METHOD("get_" #property), &self_type ::get_##property);                                     \
    ClassDB::bind_method(D_METHOD("set_" #property, #property), &self_type ::set_##property);                          \
    ClassDB::add_property(get_class_static(), PropertyInfo(variantType, #property, property_hint, hint_string),        \
                          "set_" #property, "get_" #property);

#define BIND_BOOL_PROPERTY(property)                                                                                   \
    ClassDB::bind_method(D_METHOD("is_" #property), &self_type ::is_##property);                                       \
    ClassDB::bind_method(D_METHOD("set_" #property, #property), &self_type ::set_##property);                          \
    ClassDB::add_property(get_class_static(), PropertyInfo(godot::Variant::Type::BOOL, #property, PROPERTY_HINT_NONE), \
                          "set_" #property, "is_" #property);

#define BIND_BOOL_PROPERTY_WITH_HINT(property, property_hint, hint_string)                                             \
    ClassDB::bind_method(D_METHOD("is_" #property), &self_type ::is_##property);                                       \
    ClassDB::bind_method(D_METHOD("set_" #property, #property), &self_type ::set_##property);                          \
    ClassDB::add_property(get_class_static(),                                                                          \
                          PropertyInfo(godot::Variant::Type::BOOL, #property, property_hint, hint_string),             \
                          "set_" #property, "is_" #property);
#define BIND_METHOD(method, ...) ClassDB::bind_method(D_METHOD(#method, ##__VA_ARGS__), &self_type::method);
