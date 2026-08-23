namespace FmodGodot
{
    using System;
    using System.Runtime.InteropServices;
    using FMOD.Studio;
    using Godot;

    public partial class FmodEventEmitter2D : Node2D
    {
        protected FmodEventEmitter2D()
        {
        }
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_start(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_stop(IntPtr handle);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_attached_to_rigidbody(IntPtr handle);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_attached_to_rigidbody(IntPtr handle, bool p_attached);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern FMOD.GUID emitter2d_get_event(IntPtr handle);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_event(IntPtr handle, FMOD.GUID p_event);


        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_one_shot(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_one_shot(IntPtr handle, bool p_one_shot);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_trigger_once(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_trigger_once(IntPtr handle, bool p_trigger_once);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_non_rigid_body_velocity(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_non_rigid_body_velocity(IntPtr handle, bool p_non_rigid_body_velocity);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_preload_sample_data(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_preload_sample_data(IntPtr handle, bool p_preload_sample_data);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_override_attenuation(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_override_attenuation(IntPtr handle, bool p_override_attenuation);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float emitter2d_get_volume(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_volume(IntPtr handle, float p_volume);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float emitter2d_get_attenuation_min(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_attenuation_min(IntPtr handle, float p_attenuation_min);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float emitter2d_get_attenuation_max(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_attenuation_max(IntPtr handle, float p_attenuation_max);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_allow_fadeout(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_allow_fadeout(IntPtr handle, bool p_allow_fadeout);

        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_parameter(IntPtr handle, [MarshalAs(UnmanagedType.LPUTF8Str)] string p_name, float p_value);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_parameter_by_id(IntPtr handle, PARAMETER_ID p_id, float p_value);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float emitter2d_get_parameter(IntPtr handle, [MarshalAs(UnmanagedType.LPUTF8Str)] string p_name);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern float emitter2d_get_parameter_by_id(IntPtr handle, PARAMETER_ID p_id);
        [DllImport(FmodAudioServer.Version.dll)]

        private static extern bool emitter2d_is_playing(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern bool emitter2d_is_paused(IntPtr handle);
        [DllImport(FmodAudioServer.Version.dll)]
        private static extern void emitter2d_set_paused(IntPtr handle, bool p_is_paused);



        public void Start()
        {
            emitter2d_start(NativeInstance);
        }

        public void Stop()
        {
            emitter2d_stop(NativeInstance);
        }


        bool AttachedToRigidbody
        {
            get => emitter2d_is_attached_to_rigidbody(NativeInstance);
            set => emitter2d_set_attached_to_rigidbody(NativeInstance, value);
        }


        public FMOD.GUID GetEvent
        {
            get => emitter2d_get_event(NativeInstance);
            set => emitter2d_set_event(NativeInstance, value);
        }

        public bool IsOneShot()
        {
            return emitter2d_is_one_shot(NativeInstance);
        }

        public void SetOneShot(bool one_shot)
        {
            emitter2d_set_one_shot(NativeInstance, one_shot);
        }

        public bool TriggerOnce
        {
            get => emitter2d_is_trigger_once(NativeInstance);
            set => emitter2d_set_trigger_once(NativeInstance, value);
        }

        public bool NonRigidBodyVelocity
        {
            get => emitter2d_is_non_rigid_body_velocity(NativeInstance);
            set => emitter2d_set_non_rigid_body_velocity(NativeInstance, value);
        }

        public bool PreloadSampleData
        {
            get => emitter2d_is_preload_sample_data(NativeInstance);
            set => emitter2d_set_preload_sample_data(NativeInstance, value);
        }

        public bool OverrideAttenuation
        {
            get => emitter2d_is_override_attenuation(NativeInstance);
            set => emitter2d_set_override_attenuation(NativeInstance, value);
        }


        public float Volume
        {
            get => emitter2d_get_volume(NativeInstance);
            set => emitter2d_set_volume(NativeInstance, value);
        }

        public float AttenuationMin
        {
            get => emitter2d_get_attenuation_min(NativeInstance);
            set => emitter2d_set_attenuation_min(NativeInstance, value);
        }

        public float AttenuationMax
        {
            get => emitter2d_get_attenuation_max(NativeInstance);
            set => emitter2d_set_attenuation_max(NativeInstance, value);
        }

        public bool AllowFadeout
        {
            get =>
            emitter2d_is_allow_fadeout(NativeInstance);
            set => emitter2d_set_allow_fadeout(NativeInstance, value);
        }


        public void SetParameter([MarshalAs(UnmanagedType.LPUTF8Str)] string name, float value)
        {
            emitter2d_set_parameter(NativeInstance, name, value);
        }

        public void SetParameterById(PARAMETER_ID id, float value)
        {
            emitter2d_set_parameter_by_id(NativeInstance, id, value);
        }

        public float GetParameter([MarshalAs(UnmanagedType.LPUTF8Str)] string name)
        {
            return emitter2d_get_parameter(NativeInstance, name);
        }

        public float GetParameterById(PARAMETER_ID id)
        {
            return emitter2d_get_parameter_by_id(NativeInstance, id);
        }


        public bool IsPlaying()
        {
            return emitter2d_is_playing(NativeInstance);
        }

        public bool Paused
        {
            get => emitter2d_is_paused(NativeInstance);
            set => emitter2d_set_paused(NativeInstance, value);
        }
    }
}
