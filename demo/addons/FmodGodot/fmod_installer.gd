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
	connect("close_requested", on_hide,CONNECT_PERSIST);
	pass
func on_hide():
	if current_state == State.NONE:
		hide();
	
func fail(message: String):
	current_state = State.NONE
	login.show()
	install.hide()
	error.text = message
func download() -> void:
	var dict = OS.execute_with_pipe("python3", ["addons/FmodGodot/fmod_installer.py","--noprompts", "-u", username.text, "-p", password.text, "download_version", FmodAudioServer.get_version_number()])	
	process_id = dict["pid"]
	stdio = dict["stdio"]
	stderr = dict["stderr"]
	login.hide()
	install.show()
	current_state = State.DOWNLOAD
func install_cs() -> void:
	progress_bar.value = 100.0
	var filename = 'fmodstudioapi%slinux.tar.gz' % FmodAudioServer.get_version_number().replace(".","")
	var dict = OS.execute_with_pipe("python3", ["addons/FmodGodot/fmod_installer.py","--noprompts", "install_cs", filename, "addons/FmodGodot"])	
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
				
				var line2 = stdio.get_line()
				var line = line2;
				while line2:
					line = line2
					line2 = stdio.get_line()
				
				if line and line.contains("%"):
					var percentage : String = line.split(" ")[1].replace("%","")
					progress_bar.value = float(percentage)
				elif line:
					cout.text += line
					
				pass
		State.INSTALLING:
			cout.text += stdio.get_as_text();
			var err = stderr.get_as_text();
			if err != "":
				cout.text += "[color=red]"+ err + "[/color]"
			if not OS.is_process_running(process_id):
				current_state = State.NONE
				if OS.get_process_exit_code(process_id) == 4:
					fail("Install failed")
				else:
					on_hide()
			pass
func _unhandled_input(event: InputEvent) -> void:
	if(event.is_pressed() && event.is_action("ui_cancel")):
		on_hide();

