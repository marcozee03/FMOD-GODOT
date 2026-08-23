namespace FmodGodot
{
    using System;
    using System.Runtime.InteropServices;
    using FMOD.Studio;
    using Godot;

    public partial class FmodListener2D : Node2D
    {
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern int listener2d_get_listener_index(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void listener2d_set_listener_index(IntPtr handle, int p_listener_index);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float listener2d_get_listener_weight(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void listener2d_set_listener_weight(IntPtr handle, float weight);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool listener2d_is_attached_to_parent(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void listener2d_set_attached_to_parent(IntPtr handle, bool p_attached_to_parent);

        public int ListenerIndex
        {
            get => listener2d_get_listener_index(NativeInstance);
            set => listener2d_set_listener_index(NativeInstance, value);
        }
        public float ListenerWeight
        {
            get => listener2d_get_listener_weight(NativeInstance);
            set => listener2d_set_listener_weight(NativeInstance, value);
        }
        public bool AttachedToParent
        {
            get => listener2d_is_attached_to_parent(NativeInstance);
            set => listener2d_set_attached_to_parent(NativeInstance, value);
        }
    }
}
