namespace FmodGodot
{
    using System;
    using System.Runtime.InteropServices;
    using FMOD.Studio;
    using Godot;

    public partial class FmodBank : Resource
    {
        protected FmodBank()
        {
        }
        Bank Bank
        {
            get => new()
            {
                handle = fmod_bank_get_bank(NativeInstance)
            }; set => fmod_bank_set_bank(NativeInstance, value.handle);
        }
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT fmod_bank_set_bank(IntPtr handle, IntPtr bank);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern IntPtr fmod_bank_get_bank(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern LOADING_STATE fmod_bank_get_sample_loading_state(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern LOADING_STATE fmod_bank_get_loading_state(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT fmod_bank_unload_sample_data(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.RESULT fmod_bank_unload(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool fmod_bank_is_valid(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.GUID fmod_bank_get_id(IntPtr handle);

        LOADING_STATE GetLoadingState()
        {
            return fmod_bank_get_loading_state(NativeInstance);
        }
        LOADING_STATE GetSampleLoadingState()
        {
            return fmod_bank_get_sample_loading_state(NativeInstance);
        }
        public FMOD.RESULT UnloadSampleData()
        {
            return fmod_bank_unload_sample_data(NativeInstance);
        }
        public FMOD.RESULT Unload()
        {
            return fmod_bank_unload(NativeInstance);
        }
        public bool IsValid()
        {
            return fmod_bank_is_valid(NativeInstance);
        }
        public FMOD.GUID GetId(IntPtr handle)
        {
            return fmod_bank_get_id(NativeInstance);
        }

    }
}
