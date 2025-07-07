
namespace FmodGodot
{
    using System;
    using System.Runtime.InteropServices;

    using FMOD;
    using FMOD.Studio;
    using Godot;
    using Godot.NativeInterop;

    public static partial class FmodAudioServer : Object
    {
        static FmodAudioServer()
        {
            get_fmod_studio(out studio.handle);
            get_fmod_core(out core.handle);
        }
        private struct Version
        {

#if TOOLS
            public const string template = "editor";
#elif DEBUG
public const string template = "template_debug";
#else
public const string template = "template_release";
#endif
#if GODOT_WINDOWS
            public const string dll = $"libFmodGodot.windows.{template}.x86_64";
#elif GODOT_LINUXBSD
            public const string dll = $"libFmodGodot.linux.{template}.x86_64";
#endif
        }
        private static FMOD.Studio.System studio;

        public static FMOD.Studio.System Studio
        {
            get => studio;
            private
                set => studio = value;
        }

        private static FMOD.System core;

        public static FMOD.System Core
        {
            get => core;
            private
                set => core = value;
        }

        [DllImport(Version.dll)]
        private static extern int get_fmod_core(out IntPtr handle);
        [DllImport(Version.dll)]
        private static extern int get_fmod_studio(out IntPtr handle);

        public static void PlayOneShot(string path)
        {
            Studio.getEvent(path, out var _event).ToString();
            _event.createInstance(out var instance).ToString();
            instance.start().ToString();
            instance.release().ToString();
        }
        public static void PlayOneShotWithParam(string path, float value, string paramName)
        {

            Studio.getEvent(path, out var _event).ToString();
            _event.createInstance(out var instance).ToString();
            instance.setParameterByName(paramName, value).ToString();
            instance.start().ToString();
            instance.release().ToString();
        }
        public static void PlayOneShotWithParam(GUID guid, float value, string paramName)
        {

            Studio.getEventByID(guid, out var _event).ToString();
            _event.createInstance(out var instance).ToString();
            instance.setParameterByName(paramName, value).ToString();
            instance.start().ToString();
            instance.release().ToString();
        }
        public static void PlayOneShot(GUID @event)
        {
            Studio.getEventByID(@event, out var _event);
            _event.createInstance(out var instance);
            instance.start();
            instance.release();
        }
    }
}
