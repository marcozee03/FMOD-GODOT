@tool
extends Window

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.
@export
var username : LineEdit
@export
var password : LineEdit
@export
var login : Control
@export
var install : Control
@export
var error : RichTextLabel
@export
var cout : RichTextLabel
@export
var progress_bar: ProgressBar
var stdio : FileAccess
var stderr: FileAccess
enum State {
	NONE,	
	DOWNLOAD,
	INSTALLING
	}
var current_state : State = State.NONE
func _init() -> void:
	connect("popup_hide", on_hide,CONNECT_ONE_SHOT);
	pass
func on_hide():
	OS.kill(process_id)
	
func fail(message: String):
	current_state = State.NONE
	login.show()
	install.hide()
	error.text = message
func download() -> void:
	var dict = OS.execute_with_pipe("python3", ["addons/FmodGodot/fmod_installer.py", "-u", username.text, "-p", password.text, "download_version", FmodAudioServer.get_version_number()])	
	process_id = dict["pid"]
	stdio = dict["stdio"]
	stderr = dict["stderr"]
	login.hide()
	install.show()
	current_state = State.DOWNLOAD
func install_cs() -> void:
	var filename = 'fmodstudioapi%slinux.tar.gz' % FmodAudioServer.get_version_number().replace(".","")
	var dict = OS.execute_with_pipe("python3", ["addons/FmodGodot/fmod_installer.py", "install_cs", filename, "addons/FmodGodot"])	
	process_id = dict["pid"]
	stdio = dict["stdio"]
	stderr = dict["stderr"]
	login.hide()
	install.show()
	current_state = State.INSTALLING
func submit() -> void:
	download()
var process_id : int
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	match(current_state):
		State.NONE:
			return
		State.DOWNLOAD:
			
			if not OS.is_process_running(process_id):
				print("stopped", process_id)
				var res = OS.get_process_exit_code(process_id)
				print(res)
				error.text = stderr.get_as_text()
				match (res):
					0:
						install_cs()
						return
					1:
						fail("Login Failed")
						return
					2:
						fail("Download Failed")
						return
					3:
						fail("Failed to resolve SO number")
						return
					4:
						fail("Install failed")
						return
				fail("Unknown Login/Download error")
			else:
				var line = stdio.get_line()
				
				if line and line.contains("%"):
					var percentage : String = line.split(" ")[1].replace("%","")
					progress_bar.value = float(percentage)
				elif line:
					cout.text += line
					
				pass
		State.INSTALLING:
			cout.text += stdio.get_as_text();
			cout.text += "[color=red]"+ stderr.get_as_text() + "[/color]"
			if not OS.is_process_running(process_id):
				current_state = State.NONE
			pass
