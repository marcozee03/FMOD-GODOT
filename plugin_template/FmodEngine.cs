
namespace FmodGodot
{
    using System;
    using System.Runtime.InteropServices;

    using FMOD;
    using FMOD.Studio;
    using Godot;
    using Godot.NativeInterop;

    public partial class FmodEngine : Node
    {
        private struct Version
        {
#if TOOLS
#if GODOT_WINDOWS
            public const string dll = "libFmodGodot.windows.editor.x86_64";
#elif GODOT_LINUXBSD
            public const string dll = "libFmodGodot.linux.editor.x86_64";
#endif
#endif
#if !TOOLS
#if GODOT_WINDOWS
            public const string dll = "libFmodGodot.windows.template_release.x86_64";
#elif GODOT_LINUXBSD
            public const string dll = "libFmodGodot.linux.template_release.x86_64";
#endif
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

        public override void _EnterTree()
        {
            base._EnterTree();
            get_fmod_studio(out studio.handle);
            get_fmod_core(out core.handle);
            // FMOD.Studio.System.create(out studio);
            // studio.initialize(ProjectSettings.GetSetting("Fmod/General/channel_count").AsInt32(), FMOD.Studio.INITFLAGS.NORMAL, FMOD.INITFLAGS.NORMAL, 0);
            // studio.getCoreSystem(out core);
            // core.setDSPBufferSize(ProjectSettings.GetSetting("Fmod/DSP/dsp_buffer_size", 512).As<uint>(), ProjectSettings.GetSetting("Fmod/DSP/dsp_buffer_count", 4).AsInt32());
            // LoadMasterBanks();
            // ADVANCEDSETTINGS settings = new ADVANCEDSETTINGS();
            // core.getAdvancedSettings(ref settings);
        }
        public override void _PhysicsProcess(double delta)
        {
            base._PhysicsProcess(delta);
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
        public override void _ExitTree()
        {
            Studio.release();
        }
    }
}
