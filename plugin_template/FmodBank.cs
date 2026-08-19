namespace FmodGodot
{
    using System;
    using System.Reflection.Metadata;
    using System.Runtime.InteropServices;
    using FMOD.Studio;
    using Godot;
    using Godot;
    using Godot.NativeInterop;

    public partial class FmodBank : Resource
    {
        Bank bank
        {
            get
            {
                return new Bank
                {
                    handle = get_bank(NativeInstance)
                };
            }
            set
            {
                set_bank(NativeInstance, value.handle);
            }
        }
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT set_bank(IntPtr handle, IntPtr bank);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern IntPtr get_bank(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern LOADING_STATE get_sample_loading_state(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern LOADING_STATE get_loading_state(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT unload_sample_data(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT unload(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool is_bank_valid(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.GUID get_id(IntPtr handle);

        LOADING_STATE GetLoadingState()
        {
            return get_loading_state(bank.handle);
        }
        LOADING_STATE GetSampleLoadingState()
        {
            return get_sample_loading_state(bank.handle);
        }
    }
}

