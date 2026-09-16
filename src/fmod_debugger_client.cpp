#if DEBUG_ENABLED
#include "fmod_debugger_client.h"
#include "classes/engine.hpp"
#include "classes/engine_debugger.hpp"
#include "classes/performance.hpp"
#include "fmod.h"
#include "fmod_audio_server.h"
#include "fmod_studio_common.h"
#include "godot.hpp"
#include "variant/callable_method_pointer.hpp"
namespace FmodGodot
{
int FmodDebuggerClient::track_memory()
{
    int current;
    FMOD_Memory_GetStats(&current, nullptr, false);
    return current;
}
float FmodDebuggerClient::track_cpu(CPUUsage p_usage)
{

    FMOD_STUDIO_CPU_USAGE studio_usage;
    FMOD_CPU_USAGE usage;
    FMOD_Studio_System_GetCPUUsage(FmodAudioServer::get_singleton()->get_studio(), &studio_usage, &usage);
    switch (p_usage)
    {
    case USAGE_STUDIO_UPDATE:
        return studio_usage.update;
    case USAGE_DSP:
        return usage.dsp;
    case USAGE_STREAM:
        return usage.stream;
    case USAGE_GEOMETRY:
        return usage.geometry;
    case USAGE_UPDATE:
        return usage.update;
    case USAGE_CONVULUTION1:
        return usage.convolution1;
    case USAGE_CONVULUTION2:
        return usage.convolution2;
        break;
    }
    return 0;
}
bool FmodDebuggerClient::debugger_capture(const String &p_message, const Array &p_data)
{
    if (p_message == "mute_audio")
    {
        FmodAudioServer::get_singleton()->set_muted(static_cast<bool>(p_data[0]));
    }
    return false;
}

void FmodDebuggerClient::update_live_update(bool p_is_live_update_connected)
{
    if (!Engine::get_singleton()->is_editor_hint())
    {
        EngineDebugger *debugger = EngineDebugger::get_singleton();
        if (debugger != nullptr)
        {
            debugger->send_message("fmod:live_update", {p_is_live_update_connected});
        }
    }
}

void FmodDebuggerClient::_bind_methods()
{
}
void FmodDebuggerClient::init(ModuleInitializationLevel p_init_level)
{
    switch (p_init_level)
    {
    case godot::MODULE_INITIALIZATION_LEVEL_SERVERS: {
        Callable cpu = callable_mp(this, &FmodDebuggerClient::track_cpu);
        Performance::get_singleton()->add_custom_monitor("fmod/memory_usage",
                                                         callable_mp(this, &FmodDebuggerClient::track_memory), {},
                                                         Performance::MONITOR_TYPE_MEMORY);
        Performance::get_singleton()->add_custom_monitor("fmod/dsp_cpu_usage", cpu, {USAGE_DSP},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
        Performance::get_singleton()->add_custom_monitor("fmod/convulution1_cpu_usage", cpu, {USAGE_CONVULUTION1},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
        Performance::get_singleton()->add_custom_monitor("fmod/convulution2_cpu_usage", cpu, {USAGE_CONVULUTION1},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
        Performance::get_singleton()->add_custom_monitor("fmod/core_update_cpu_usage", cpu, {USAGE_UPDATE},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
        Performance::get_singleton()->add_custom_monitor("fmod/studio_update_cpu_usage", cpu, {USAGE_STUDIO_UPDATE},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
        Performance::get_singleton()->add_custom_monitor("fmod/stream_cpu_usage", cpu, {USAGE_STREAM},
                                                         Performance::MONITOR_TYPE_PERCENTAGE);
    }
    break;
    case godot::MODULE_INITIALIZATION_LEVEL_SCENE: {
        if (!Engine::get_singleton()->is_editor_hint())
        {
            EngineDebugger::get_singleton()->register_message_capture(
                "fmod", callable_mp(this, &FmodDebuggerClient::debugger_capture));
        }
        FmodAudioServer::get_singleton()->connect(
            "live_update_connected", callable_mp(this, &FmodDebuggerClient::update_live_update).bind(true));
        FmodAudioServer::get_singleton()->connect(
            "live_update_disconnected", callable_mp(this, &FmodDebuggerClient::update_live_update).bind(false));

        update_live_update(FmodAudioServer::get_singleton()->is_live_update_connected());
    }
    break;
    default:
        break;
    }
}
} // namespace FmodGodot
#endif
