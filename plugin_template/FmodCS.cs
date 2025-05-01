#if TOOLS
namespace FmodGodot
{
	using Godot;
	[Tool]
	public partial class FmodCS : EditorPlugin
	{
		public override void _EnterTree()
		{
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/auto_initialize"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/auto_initialize", true);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/channel_count"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/channel_count", 1024);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/is_live_update_enabled"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/is_live_update_enabled", true);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/is_memory_tracking_enabled"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/is_memory_tracking_enabled", false);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/default_listener_count"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/default_listener_count", 1);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/Banks/banks_path"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/Banks/banks_path", "res://Banks");
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/General/should_load_by_name"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/General/should_load_by_name", false);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/Software Format/sample_rate"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/Software Format/sample_rate", 48000);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/Software Format/speaker_mode"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/Software Format/speaker_mode", 3);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/DSP/dsp_buffer_size"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/DSP/dsp_buffer_size", 512);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/DSP/dsp_buffer_count"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/DSP/dsp_buffer_count", 4);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/3D Settings/doppler_scale"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/3D Settings/doppler_scale", 1);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/3D Settings/distance_factor"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/3D Settings/distance_factor", 1);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/3D Settings/rolloff_scale"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/3D Settings/rolloff_scale", 1);
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/Banks/Master_Bank_Path"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/Banks/Master_Bank_Path", "res://Banks");
			}
			if (!ProjectSettings.Singleton.HasSetting("Fmod/Banks/Master_Strings_Bank_Path"))
			{
				ProjectSettings.Singleton.SetSetting("Fmod/Banks/Master_Strings_Bank_Path", "res://Banks");
			}
			AddAutoloadSingleton("FmodEngine", "res://addons/FmodGodot/Scenes/fmod_engine.tscn");
		}

		public override void _ExitTree()
		{
		}
	}
}
#endif
