
#ifndef FMOD_ENGINE
#define FMOD_ENGINE
#include <classes/node.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot_cpp/templates/hash_map.hpp>
using namespace std;
using namespace godot;
using namespace FMOD;
namespace FMODGodot
{
    // struct GstringHasher
    // {
    //     size_t operator()(const string &s){
    //         return s.hash();
    //     }
    // };
    // struct GstringEquals
    // {
    //     bool operator()(const string &s1 , const string &s2){
    //         return s1 == s2;
    //     }
    // };
    class FMODEngine : public Node
    {
        GDCLASS(FMODEngine, Node)
    private:
        static void provide(FMODEngine *engine);
        static FMODEngine *instance;
        Studio::System *studio;
        System *core;
        Studio::Bank *Master;
        Studio::Bank *Masterstrings;
        HashMap<String, size_t> banks;
        void LoadMasterBanks();

    protected:
        static void _bind_methods();
        void _notification(int p_what);

    public:
        FMODEngine();
        ~FMODEngine();
        static System *get_global_core();
        static Studio::System *get_global_studio();
        static void get_global_core_ref(System **core);
        static void get_global_studio_ref(Studio::System **studio);
        static FMOD_RESULT load_global_bank(String path);
        static void unload_global_bank(String path);
        static void unload_global_banks();
        System *get_core();
        Studio::System *get_studio();
        void get_core_ref(System **core);
        void get_studio_ref(Studio::System **studio);
        FMOD_RESULT load_bank(String path);
        void unload_bank(String path);
        void unload_banks();
    };

}
#endif
