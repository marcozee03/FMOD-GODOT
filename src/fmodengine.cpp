#include "fmodengine.h"
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <fmod_errors.h>
using namespace std;
using namespace godot;
namespace FMODGodot
{
    FMODEngine *FMODEngine::instance = NULL;

    FMODEngine::FMODEngine()
    {
        studio = NULL;
        core = NULL;
        Master = NULL;
        Masterstrings = NULL;
    }
    FMODEngine::~FMODEngine()
    {
        // if (banks)
        // {
        //     delete (banks);
        // }
    }

    void FMODEngine::_notification(int p_what)
    {
        FMOD_RESULT result;
        switch (p_what)
        {
        case NOTIFICATION_ENTER_TREE:
            if (!instance)
            {
                provide(this);
            }

            result = FMOD::Studio::System::create(&studio);
            if (result != FMOD_OK)
            {
                std::cout << "\ncreate" << FMOD_ErrorString(result);
            }
            result = studio->initialize(ProjectSettings::get_singleton()->get_setting("Fmod/General/channel_count"), FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
            if (result != FMOD_OK)
            {
                std::cout << "\ninit" << FMOD_ErrorString(result);
            }
            result = studio->getCoreSystem(&core);
            if (result != FMOD_OK)
            {
                std::cout << "\nget core" << FMOD_ErrorString(result);
            }
            // result = core->setDSPBufferSize(ProjectSettings::get_singleton()->get_setting("Fmod/DSP/dsp_buffer_size", 512), ProjectSettings::get_singleton()->get_setting("Fmod/DSP/dsp_buffer_count", 4));
            if (result != FMOD_OK)
            {
                std::cout << "\ndsp" << FMOD_ErrorString(result);
            }
            break;
        case NOTIFICATION_READY:
            LoadMasterBanks();
            break;
        case NOTIFICATION_EXIT_TREE:
            if (instance == this)
            {
                instance = NULL;
            }
            studio->release();
            break;
        case NOTIFICATION_PHYSICS_PROCESS:
            studio->update();
            break;
        default:
            break;
        }
    }
    void FMODEngine::_bind_methods()
    {
        ClassDB::bind_method(D_METHOD("unload_banks"), &FMODEngine::unload_banks);
    }
    void FMODEngine::LoadMasterBanks()
    {
        load_bank(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Bank_Path", "").stringify().utf8().ptr());
        load_bank(ProjectSettings::get_singleton()->get_setting("Fmod/Banks/Master_Strings_Bank_Path", "").stringify().utf8().ptr());
    }
    FMOD_RESULT FMODEngine::load_bank(String path)
    {
        PackedByteArray BankMemory = FileAccess::get_file_as_bytes(path);
        Studio::Bank *bank;
        FMOD_RESULT result = studio->loadBankMemory((char *)BankMemory.ptr(), BankMemory.size(), FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
        if (result == FMOD_OK)
        {
            banks.insert(path, 1);
        }
        else if (result == FMOD_ERR_EVENT_ALREADY_LOADED)
        {
            banks[path] = banks[path] + 1;
        }
        else
        {
            godot::_err_print_error("load_bank", "fmodengine.cpp", 103, "Couldn't load bank:" + path + " error:" + result, true, true);
        }
        return result;
    }
    void FMODEngine::unload_bank(String path)
    {
        if (!studio->isValid())
        {
            return;
        }
        banks[path] = banks[path] - 1;
        if (banks[path] <= 0)
        {
            banks.erase(path);
            Studio::Bank *bank;
            studio->getBank(path.utf8().ptr(), &bank);
            bank->unload();
        }
    }
    void FMODEngine::unload_banks()
    {
        if (!studio->isValid())
        {
            return;
        }
        studio->unloadAll();
    }
    void FMODEngine::get_core_ref(System **core)
    {
        *core = this->core;
    }
    void FMODEngine::get_studio_ref(Studio::System **studio)
    {
        *studio = this->studio;
    }
    System *FMODEngine::get_core()
    {
        return core;
    }
    Studio::System *FMODEngine::get_studio()
    {
        return studio;
    }

    System *FMODEngine::get_global_core()
    {
        return instance->get_core();
    }
    Studio::System *FMODEngine::get_global_studio()
    {
        return instance->get_studio();
    }
    void FMODEngine::get_global_core_ref(System **core)
    {
        instance->get_core_ref(core);
    }
    void FMODEngine::get_global_studio_ref(Studio::System **studio)
    {
        instance->get_studio_ref(studio);
    }
    FMOD_RESULT FMODEngine::load_global_bank(String path)
    {

        return instance->load_bank(path);
    }
    void FMODEngine::unload_global_bank(String path)
    {
        instance->unload_bank(path);
    }
    void FMODEngine::unload_global_banks()
    {
        instance->unload_banks();
    }
    void FMODEngine::provide(FMODEngine *engine)
    {
        instance = engine;
    }
    extern "C"
    {
        GDE_EXPORT int get_fmod_core(System **core)
        {
            FMODEngine::get_global_core_ref(core);
            if (core == NULL)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        GDE_EXPORT int get_fmod_studio(Studio::System **studio)
        {
            FMODEngine::get_global_studio_ref(studio);
            if (studio == NULL)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}