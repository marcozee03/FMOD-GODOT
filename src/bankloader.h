#ifndef BANK_LOADER
#define BANK_LOADER
#include <godot_cpp/classes/node.hpp>
using namespace godot;
namespace FMODGodot
{
    class BankLoader : public Node
    {
        GDCLASS(BankLoader, Node)
    protected:
        static void _bind_methods();
    public:
        PackedStringArray banks;
        BankLoader();
        ~BankLoader();
        void _enter_tree() override;
        void _exit_tree() override;
        void set_banks(PackedStringArray banks);
        PackedStringArray get_banks();
    };
}
#endif