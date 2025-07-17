#pragma once
#include <godot_cpp/classes/node.hpp>
#include "fmod_bank.h"
using namespace godot;
namespace FmodGodot
{
    class FmodBankLoader : public Node
    {
        GDCLASS(FmodBankLoader, Node)
        bool preload_sample_data;

    protected:
        static void _bind_methods();

    public:
        TypedArray<Ref<FmodBank>> banks;
        FmodBankLoader();
        ~FmodBankLoader();
        void _enter_tree() override;
        void set_banks(TypedArray<Ref<FmodBank>> banks);
        TypedArray<Ref<FmodBank>> get_banks();
        void set_preload_sample_data(bool p_preload);
        bool is_preload_sample_data();
    };
}