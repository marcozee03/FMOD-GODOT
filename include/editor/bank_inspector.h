#ifdef TOOLS_ENABLED
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/tree_item.hpp>
#include <fmod_audio_server.h>
#include <fmod_studio.hpp>
#include "fmod_bank.h"
using namespace godot;
using namespace FMOD::Studio;
namespace FmodGodot
{
    class BankInspector : public Tree
    {
        GDCLASS(BankInspector, Tree);
        
    private:
        Ref<FmodBank> m_bank;
        Ref<Texture2D> event_icon;
        Ref<Texture2D> c_parameter_icon;
        Ref<Texture2D> d_parameter_icon;
    protected:
        static void _bind_methods();

    public:
        BankInspector();
        BankInspector(Ref<FmodBank> p_bank);
        ~BankInspector() = default;
        void _enter_tree() override;
    };
}
#endif