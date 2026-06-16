#ifdef TOOLS_ENABLED
#include "fmod_script_client.h"
#include "classes/global_constants.hpp"
#include "core/error_macros.hpp"
#include "core/print_string.hpp"
#include "fmod_editor_interface.h"
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
        FmodEditorInterface::get_singleton()->print_error(
            "FMOD Godot: Script Client failed to connect - Check FMOD Studio is running");
        print_error("FMOD Godot: Script Client failed to connect - Check FMOD Studio is running");
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

        FmodEditorInterface::get_singleton()->print_rich(vformat("[color=green]%s,[/color]", header.substr(6)));
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
    if (command.strip_edges().is_empty())
    {
        return "";
    }
    if (!is_connected_to_studio())
    {
        connect_to_fmod();
    }
    put_data(command.to_utf8_buffer());
    while (get_available_bytes() == 0)
    {
        poll();
    }
    Array arr = get_partial_data(get_available_bytes());
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
    if (command.strip_edges().is_empty())
    {
        return "";
    }
    if (!is_connected_to_studio())
    {
        connect_to_fmod();
    }
    put_data(command.to_utf8_buffer());
    PackedByteArray buffer;
    while (get_available_bytes() == 0)
    {
        poll();
    }
    while (get_available_bytes() > 0)
    {
        Array arr = get_data(get_available_bytes());
        if ((int)arr[0] == Error::OK)
        {
            buffer.append_array(arr[1]);
        }
        poll();
    }
    return buffer.get_string_from_utf8().strip_edges(true, true);
}

void FmodScriptClient::_bind_methods()
{
    // BIND_METHOD(get_script_output())
}
#endif
