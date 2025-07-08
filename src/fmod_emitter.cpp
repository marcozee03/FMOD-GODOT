// #include "fmod_emitter.h"
// #include "fmod_audio_server.h"
// #include "fmod_globals.h"
// using namespace FmodGodot;
// bool FMODEmitter::_set(const StringName &p_name, const Variant &p_property) {
//     set
// }
// bool FMODEmitter::_get(const StringName &p_name, Variant &r_property) const
// {
// }
// bool FMODEmitter::_property_can_revert(const StringName &p_name) const
// {
// }
// bool FMODEmitter::_property_get_revert(const StringName &p_name, Variant &r_property) const{

// }
// void FMODEmitter::_get_property_list(List<PropertyInfo> *p_list) const{
    
// }
//     FMODEmitter::FMODEmitter(/* args */){

//     }
//     FMODEmitter::~FMODEmitter(){

//     }

//     void FMODEmitter::emit(){
//         auto studio = FmodAudioServer::get_global_studio();
//     }
//     Vector4i FMODEmitter::get_event(){
        
//         return event;
//     }
//     void FMODEmitter::set_event(Vector4i event){
//         this->event = event;
//         if (description->isValid()){
//             description->releaseAllInstances();
//         }
        
//         auto studio = FmodAudioServer::get_global_studio();
//         studio.eve
//         FMOD_GUID guid = cast_to_FMOD_GUID(event);
//         studio->getEventByID(&guid, &description);
//         this->event = event;
//     }