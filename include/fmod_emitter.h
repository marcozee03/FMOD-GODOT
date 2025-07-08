// #include <fmod_studio.hpp>
// #include <godot.hpp>
// #include <object.hpp>
// #include <godot_cpp/variant/vector4i.hpp>
// using namespace godot;
// namespace FmodGodot
// {
//     class FMODEmitter : public Object
//     {
//         GDCLASS(FMODEmitter, Object);

//     private:
//         Vector4i event;
//         bool one_shot;
//         FMOD::Studio::EventDescription* description;
//     protected:
//         bool _set(const StringName &p_name, const Variant &p_property);
//         bool _get(const StringName &p_name, Variant &r_property) const;
//         bool _property_can_revert(const StringName &p_name) const;
//         bool _property_get_revert(const StringName &p_name, Variant &r_property) const;
//         void _get_property_list(List<PropertyInfo> *p_list) const;
//         static void _bind_methods();

//     public:
//         FMODEmitter(/* args */);
//         ~FMODEmitter();
//         void emit();
//         Vector4i get_event();
//         void set_event(Vector4i event);
//         bool is_one_shot();
//         void set_one_shot()
//     };
// }