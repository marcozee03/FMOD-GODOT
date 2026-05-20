#pragma once

#include "classes/stream_peer_tcp.hpp"
#include "classes/wrapped.hpp"
using namespace godot;
namespace FmodGodot
{
class FmodScriptClient : public godot::StreamPeerTCP
{
    GDCLASS(FmodScriptClient, StreamPeerTCP)
  private:
    const int StudioScriptPort = 3663;

  protected:
    static void _bind_methods();

  public:
    bool is_connected_to_studio();
    void connect_to_fmod();
    bool send_script_command(const String &command);
    String get_script_output(const String &command);
};
} // namespace FmodGodot
