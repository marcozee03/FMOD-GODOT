#include "fmod_listener.h"
#include "fmod_audio_server.h"
#include "fmod_globals.h"
#include "globals.h"
namespace FmodGodot
{
    void FmodListener::_physics_process(double delta)
    {
        FMOD_3D_ATTRIBUTES attributes = to_3d_attributes(this);
        FMOD_VECTOR vec = to_fmod_vector(this->get_global_position());
        FmodGodot::FmodAudioServer::get_singleton()->get_studio()->setListenerAttributes(listener_index,&attributes, &vec);
    }
    FmodListener::FmodListener()
    {
    }
    FmodListener::~FmodListener()
    {
    }
    int FmodListener::get_listener_index()
    {
        return listener_index;
    }
    void FmodListener::set_listener_index(int index)
    {
        listener_index = Math::clamp(index, 0, FMOD_MAX_LISTENERS - 1);
        // FmodAudioServer::get_singleton()->
    }

    void FmodListener::_bind_methods(){
        BIND_PROPERTY(listener_index, FmodListener, Variant::Type::INT);
    }
}