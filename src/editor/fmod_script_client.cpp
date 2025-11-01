#include "fmod_script_client.h"
#include "classes/global_constants.hpp"
#include "classes/stream_peer_tcp.hpp"
#include "core/error_macros.hpp"
#include "core/print_string.hpp"
#include "variant/array.hpp"
#include "variant/packed_byte_array.hpp"
#include "variant/utility_functions.hpp"
using namespace FmodGodot;
void FmodScriptClient::connect_to_fmod()
{
    if (!is_connected_to_studio())
    {
        if (Error::OK != connect_to_host("127.0.0.1", StudioScriptPort))
        {
            print_line_rich(
                "[color=red]FMOD Godot: Script Client failed to connect - Check FMOD Studio is running[/color]");
            return;
        }
        set_no_delay(true);
    }
    while (get_status() == STATUS_CONNECTING)
    {
        poll();
    }
    if (get_status() == STATUS_ERROR)
    {
        print_line_rich(
            "[color=red]FMOD Godot: Script Client failed to connect - Check FMOD Studio is running[/color]");
        return;
    }
    while (get_available_bytes() == 0)
    {
        poll();
    }
    Array arr = get_partial_data(128);
    String header = PackedByteArray(arr[1]).get_string_from_utf8().strip_edges(true, true);
    if (header.begins_with("log():"))
    {
        UtilityFunctions::print_rich("[color=green]FMOD Godot: Script Client returned " + header.substr(6) +
                                     "[/color]");
    }
}

bool FmodScriptClient::is_connected_to_studio()
{
    return get_status() == STATUS_CONNECTED;
}

bool FmodScriptClient::send_script_command(const String &command)
{

    if (!is_connected_to_studio())
    {
        connect_to_fmod();
    }
    put_data(command.to_utf8_buffer());
    while (get_available_bytes() == 0)
    {
        poll();
    }
    Array arr = get_partial_data(128);
    if ((int)arr[0] != Error::OK)
    {
        return false;
    }

    String result = PackedByteArray(arr[1]).get_string_from_utf8().strip_edges(true, true);
    if (result.begins_with("out():"))
    {
        UtilityFunctions::print_rich("[color=dim_gray]" + result.substr(6) + "[/color]");
    }
    return (result.contains("true"));
}

String FmodGodot::FmodScriptClient::get_script_output(const String &command)
{

    put_utf8_string(command);
    PackedByteArray buffer;
    while (get_available_bytes() > 0)
    {
        buffer.append_array(get_data(get_available_bytes()));
        poll();
    }
    String result = buffer.get_string_from_utf8().strip_edges(true, true);
    if (result.begins_with("out():"))
    {
        return result.substr(6);
    }
    return "";
}

void FmodScriptClient::_bind_methods()
{
}
