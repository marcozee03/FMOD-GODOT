
namespace FmodGodot
{
    using System;
    using System.Reflection.Metadata;
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
            public const string dll = $"libFmodGodot.{template}";
#elif GODOT_LINUXBSD
            public const string dll = $"libFmodGodot.{template}.so";
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

        // private static extern IntPtr create_instance(Vector4I guid);

        [DllImport(Version.dll)]
        private static extern void play_one_shot_by_id(Vector4I guid, Vector3 p_position = default);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_by_path(string p_path, Vector3 p_position = default);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_3d_attached_by_id(Vector4I guid, IntPtr p_node, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_3d_attached_by_path(string p_path, IntPtr p_node, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_2d_attached_by_id(Vector4I guid, IntPtr p_node, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_2d_attached_by_path(string p_path, IntPtr p_node, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_rigid_body3d_attached_by_id(Vector4I guid, IntPtr p_rigid_body3d);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_rigid_body3d_attached_by_path(string p_path, IntPtr p_rigid_body3d);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_rigid_body2d_attached_by_id(Vector4I guid, IntPtr p_rigid_body2d);
        [DllImport(Version.dll)]
        private static extern void play_one_shot_rigid_body2d_attached_by_path(string p_path, IntPtr p_rigid_body2d);
        [DllImport(Version.dll)]
        private static extern bool any_sample_data_loading();
        [DllImport(Version.dll)]
        private static extern void attach_instance_to_node3d(IntPtr p_node, IntPtr p_event, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void attach_instance_to_rigid_body3d(IntPtr p_node, IntPtr p_event);
        [DllImport(Version.dll)]
        private static extern void attach_instance_to_node2D(IntPtr p_node, IntPtr p_event, bool p_non_rigid_body_velocity = false);
        [DllImport(Version.dll)]
        private static extern void attach_instance_to_rigid_body2d(IntPtr p_node, IntPtr p_event);
        [DllImport(Version.dll)]
        private static extern void detach_instance_from_node(IntPtr p_event);
        [DllImport(Version.dll)]
        private static extern Vector4I path_to_guid(string p_path);
        [DllImport(Version.dll)]
        private static extern IntPtr get_event_description_by_path(string p_path);
        [DllImport(Version.dll)]
        private static extern IntPtr get_event_description_by_id(Vector4I p_guid);
        [DllImport(Version.dll)]
        private static extern void pause_all_events(bool p_pause);
        [DllImport(Version.dll)]
        private static extern bool is_muted();
        [DllImport(Version.dll)]
        private static extern void set_muted(bool p_muted);
        [DllImport(Version.dll)]
        private static extern IntPtr get_bus(string p_path);
        [DllImport(Version.dll)]
        private static extern IntPtr get_vca(string p_path);
        [DllImport(Version.dll)]
        private static extern FMOD.RESULT load_bank(string p_bankName, bool loadSamples = false);
        [DllImport(Version.dll)]
        private static extern FMOD.RESULT load_bank_by_file(string p_path, bool loadSamples = false);
        [DllImport(Version.dll)]
        private static extern void unload_banks();
        [DllImport(Version.dll)]
        private static extern bool has_bank_loaded(string p_bankName);
        [DllImport(Version.dll)]
        private static extern bool have_all_banks_loaded();
        [DllImport(Version.dll)]
        private static extern void set_listener_2d_rigidbody_location(int p_listenerIndex, IntPtr p_rigidBody2D, IntPtr p_attenuationObject = 0);
        [DllImport(Version.dll)]
        private static extern void set_listener_2d_location(int p_listenerIndex, IntPtr p_node, IntPtr p_attenuationObject = 0);

        [DllImport(Version.dll)]
        private static extern void set_listener_3d_rigidbody_location(int listenerIndex, IntPtr rigidBody, IntPtr attenuationObject = 0);
        [DllImport(Version.dll)]
        private static extern void set_listener_3d_location(int listenerIndex, IntPtr p_node, IntPtr attenuationObject = 0);

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
        public static void PlayOneShot(Vector4I guid, Vector3 position = default)
        {
            play_one_shot_by_id(guid, position);
        }
        public static void PlayOneShot(string path, Vector3 position = default)
        {
            play_one_shot_by_path(path, position);
        }
        public static void PlayOneShotAttached(Vector4I guid, Node3D node, bool nonRigidBodyVelocity = false)
        {
            play_one_shot_3d_attached_by_id(guid, node.NativeInstance, nonRigidBodyVelocity);
        }
        public static void PlayOneShotAttached(string path, Node3D node, bool nonRigidBodyVelocity = false)
        {
            play_one_shot_3d_attached_by_path(path, node.NativeInstance, nonRigidBodyVelocity);
        }
        public static void PlayOneShotAttached(Vector4I guid, Node2D node, bool nonRigidbodyVelocity = default)
        {
            play_one_shot_2d_attached_by_id(guid, node.NativeInstance, nonRigidbodyVelocity);
        }
        public static void PlayOneShotAttached(string path, Node2D node, bool nonRigidbodyVelocity = default)
        {
            play_one_shot_2d_attached_by_path(path, node.NativeInstance, nonRigidbodyVelocity);
        }
        public static void PlayOneShotAttached(Vector4I guid, RigidBody3D rigidBody3D)
        {
            play_one_shot_rigid_body3d_attached_by_id(guid, rigidBody3D.NativeInstance);
        }
        public static void PlayOneShotAttached(string path, RigidBody3D rigidBody3D)
        {
            play_one_shot_rigid_body3d_attached_by_path(path, rigidBody3D.NativeInstance);
        }
        public static void PlayOneShotAttached(Vector4I guid, RigidBody2D rigidBody2D)
        {
            play_one_shot_rigid_body2d_attached_by_id(guid, rigidBody2D.NativeInstance);
        }
        public static void PlayOneShotAttached(string path, RigidBody2D body)
        {
            play_one_shot_rigid_body2d_attached_by_path(path, body.NativeInstance);
        }
        public static bool AnySampleDataLoading()
        {
            return any_sample_data_loading();
        }
        public static void AttachInstance(Node3D node, FMOD.Studio.EventInstance @event, bool nonRigidBodyVelocity = false)
        {
            attach_instance_to_node3d(node.NativeInstance, @event.handle, nonRigidBodyVelocity);
        }
        public static void AttachInstance(RigidBody3D body, FMOD.Studio.EventInstance @event, bool nonRigidBodyVelocity = false)
        {
            attach_instance_to_rigid_body3d(body.NativeInstance, @event.handle);
        }
        public static void AttachInstance(Node2D node, FMOD.Studio.EventInstance @event, bool nonRigidBodyVelocity = false)
        {
            attach_instance_to_node2D(node.NativeInstance, @event.handle, nonRigidBodyVelocity);
        }
        public static void AttachInstance(RigidBody2D node, FMOD.Studio.EventInstance @event, bool nonRigidBodyVelocity = false)
        {
            attach_instance_to_rigid_body2d(node.NativeInstance, @event.handle);
        }
        public static void DetachInstanceFromNode(FMOD.Studio.EventInstance @event)
        {
            detach_instance_from_node(@event.handle);
        }
        public static FMOD.GUID PathToGUID(string path)
        {
            studio.lookupID(path, out var @event);
            return @event;
        }
        public static FMOD.Studio.EventDescription GetEventDescription(string path)
        {
            return new EventDescription()
            {
                handle = get_event_description_by_path(path)
            };
        }
        public static FMOD.Studio.EventDescription GetEventDescription(Vector4I guid)
        {
            return new EventDescription()
            {
                handle = get_event_description_by_id(guid)
            };
        }
        public static void PauseAllEvents(bool pause)
        {
            pause_all_events(pause);
        }
        public static bool IsMuted()
        {
            return is_muted();
        }
        public static void SetMuted(bool muted)
        {
            set_muted(muted);
        }
        public static Bus GetBus(string path)
        {
            return new Bus()
            {
                handle = get_bus(path)
            };
        }
        public static VCA GetVCA(string path)
        {
            return new VCA()
            {
                handle = get_vca(path)
            };
        }
        public static void LoadBank(string bankname, bool loadSamples = false)
        {
            load_bank(bankname, loadSamples);
        }
        public static void LoadBankByFile(string bankPath, bool loadSamples = false)
        {
            load_bank_by_file(bankPath, loadSamples);
        }
        public static void UnloadBanks()
        {
            unload_banks();
        }
        public static bool HasBankLoaded(string bankname)
        {
            return has_bank_loaded(bankname);
        }
        public static bool HaveAllBanksLoaded()
        {
            return have_all_banks_loaded();
        }
        public static void SetListenerLocation(int listenerIndex, RigidBody2D body, Node2D attenuationObject = null)
        {
            set_listener_2d_rigidbody_location(listenerIndex, body.NativeInstance, attenuationObject.NativeInstance);
        }
        public static void SetListenerLocation(int listenerIndex, Node2D node, Node2D attenuationObject)
        {
            set_listener_2d_location(listenerIndex, node.NativeInstance, attenuationObject.NativeInstance);
        }
        public static void SetListenerLocation(int listenerIndex, RigidBody3D body, Node3D attenuationObject = null)
        {
            set_listener_3d_rigidbody_location(listenerIndex, body.NativeInstance, attenuationObject.NativeInstance);
        }
        public static void SetListenerLocation(int listenerIndex, Node3D node, Node3D attenuationObject)
        {
            set_listener_3d_location(listenerIndex, node.NativeInstance, attenuationObject.NativeInstance);
        }
        // public static void PlayOneShotWithParam(string path, float value, string paramName)
        // {
        //     Studio.getEvent(path, out var _event).ToString();
        //     _event.createInstance(out var instance).ToString();
        //     instance.setParameterByName(paramName, value).ToString();
        //     instance.start().ToString();
        //     instance.release().ToString();
        // }
        // public static void PlayOneShotWithParam(GUID guid, float value, string paramName)
        // {
        //     Studio.getEventByID(guid, out var _event).ToString();
        //     _event.createInstance(out var instance).ToString();
        //     instance.setParameterByName(paramName, value).ToString();
        //     instance.start().ToString();
        //     instance.release().ToString();
        // }
    }
}
