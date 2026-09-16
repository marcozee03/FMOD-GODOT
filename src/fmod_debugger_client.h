#pragma once

#ifdef DEBUG_ENABLED
#include "classes/object.hpp"
#include "classes/wrapped.hpp"
#include "core/binder_common.hpp"
#include "variant/string.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodDebuggerClient : public Object
{
  public:
    GDCLASS(FmodDebuggerClient, Object)
  public:
    enum CPUUsage
    {
        USAGE_STUDIO_UPDATE,
        USAGE_DSP,
        USAGE_STREAM,
        USAGE_GEOMETRY,
        USAGE_UPDATE,
        USAGE_CONVULUTION1,
        USAGE_CONVULUTION2,
    };

  private:
    int track_memory();
    float track_cpu(CPUUsage p_usage);
    bool debugger_capture(const String &p_message, const Array &p_data);
    void update_live_update(bool p_is_live_update_connected);

  protected:
    static void _bind_methods();

  public:
    FmodDebuggerClient() = default;
    ~FmodDebuggerClient() = default;
    void init(ModuleInitializationLevel p_init_level);
};
} // namespace FmodGodot
VARIANT_ENUM_CAST(FmodGodot::FmodDebuggerClient::CPUUsage);
#endif
