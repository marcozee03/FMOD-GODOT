@tool
extends Window


func get_script_dir() -> String :
	return self.get_script().resource_path.get_base_dir()
var fmodlink := "https://www.fmod.com"
var fmodlogin := "https://www.fmod.com/api-login"
var fmod_downloads_link = "https://www.fmod.com/api-downloads"
var current_os = ""
var http : HTTPRequest = HTTPRequest.new();
func _theme_changed() -> void:
	var ctrl : Control = get_node("BG")
	ctrl.add_theme_stylebox_override("panel",get_theme_stylebox("PanelForeground", "EditorStyles"))
	

func _init() -> void:
	add_child(http);
	connect("close_requested", on_hide,CONNECT_PERSIST);
	theme_changed.connect(_theme_changed)
func _ready():
	# theme = EditorInterface.get_editor_theme()	
	_theme_changed()

class Response:
	var result: int = 0
	var code: int = 0
	var headers: PackedStringArray = []
	var body: PackedByteArray = []

func request(method: int, url: String, headers: PackedStringArray = [], body :String = "") -> Response:
	# Some headers
	headers.append("Accept: */*")

	var err = http.request(url, headers,method, body)
	assert(err == OK) # Make sure all is OK.
	var signal_args :Array = await http.request_completed
	var response :Response = Response.new();
	response.result = signal_args[0]
	response.code = signal_args[1]
	response.headers = signal_args[2]
	response.body = signal_args[3]

	# assert(response.result == HTTPRequest.RESULT_SUCCESS or response.code == HTTPClient.RESPONSE_OK) # Make sure request finished well.
	return response;
func _draw()->void:
	pass


func httpget(url: String, headers : PackedStringArray = []) -> Response:
	return await request(HTTPClient.METHOD_GET,url ,headers)
func post(url: String, headers: PackedStringArray = [], body : Dictionary = {}) -> Response:
	headers.append("Content-Type: application/json")
	return await request(HTTPClient.METHOD_POST, url, headers, JSON.stringify(body))

func get_token(username : String, password : String):
	var response = await post(fmodlogin, ["Authorization: Basic %s" % (Marshalls.utf8_to_base64("%s:%s" % [username,password]))], {})
	if response.code == HTTPClient.RESPONSE_UNAUTHORIZED:
		fail("The specified username or password is invalid")
		return
	if response.code != HTTPClient.RESPONSE_OK:
		fail("Failed to Login: Unknown Error with status code:%s" % response.code)
		return
	return JSON.parse_string(response.body.get_string_from_utf8())["token"]
func get_file_name(version : String, platform : String):
	if platform == "linux":
		# linux
		return "fmodstudioapi%slinux.tar.gz" % version
	elif platform == "macos":
		# OS X
		return "fmodstudioapi%sosx.dmg" % version
	elif platform == "windows":
		# Windows...
		return "fmodstudioapi%swin-installer.exe" % version
	elif platform == "android":
		# Android...
		return "fmodstudioapi%sandroid.tar.gz" % version
	elif platform == "ios":
		# iOS...
		return "fmodstudioapi%sios.dmg" % version


func get_download_link(version:String, platform : String):
	if platform == "linux":
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename=fmodstudioapi%slinux.tar.gz&user=" % version
	elif platform == "macos":
		# OS X
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Mac/&filename=fmodstudioapi%smac-installer.dmg&user=" % version
	elif platform == "windows":
		# Windows...
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Windows/&filename=fmodstudioapi%swin-installer.exe&user=" % version
	elif platform == "android":
		# Android...
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Android/&filename=fmodstudioapi%sandroid.tar.gz&user=" % version
	elif platform == "ios":
		# iOS...
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename=fmodstudioapi%sios-installer.dmg&user=$1" %version
	else:
		return null

func download_version(version: String, token: String, target_platform: String):
	var filename = get_file_name(version, target_platform)
	var downloadlink = get_download_link(version, target_platform)
	if downloadlink == null or filename == null:
		printerr("Could not decide what file to download \"Unsupported Platform?\"")
	# Next request a download link using the token!
	var response = await httpget(downloadlink, ["Authorization: Bearer %s" % token])
	if response == null :
		printerr("Failed to get download link")
	var url = JSON.parse_string(response.body.get_string_from_utf8())["url"]
	print("Received download link", url)
	# http.download_file = get_script_dir().path_join(filename);
	progress_bar.indeterminate = true;
	cout.text = "Finding Download Link"
	http.download_file = get_script_dir().path_join("fmodstudioapi%slinux.tar.gz" % FmodAudioServer.get_version_number().replace(".",""))
	var err = http.request(url)
	assert(err == OK)

	var visible_dots := 3
	var status = http.get_http_client_status()
	while status != HTTPClient.STATUS_BODY and status != HTTPClient.STATUS_DISCONNECTED :
		print(status)
		match status:
			HTTPClient.STATUS_RESOLVING:
				cout.text = "Resolving..."	
				cout.visible_characters = 9 + visible_dots
			HTTPClient.STATUS_CONNECTING, HTTPClient.STATUS_CONNECTED:
				cout.text = "Connecting..."
				cout.visible_characters = 10 + visible_dots
			HTTPClient.STATUS_REQUESTING:
				cout.text = "Requesting..."
				cout.visible_characters = 10 + visible_dots
			HTTPClient.STATUS_CONNECTION_ERROR:
				fail("Connection Error")
		visible_dots += 1
		visible_dots %= 4
		status = http.get_http_client_status()
		await get_tree().process_frame;
		

	progress_bar.indeterminate = false
	cout.text = "Downloading..."
	while http.get_http_client_status() == HTTPClient.STATUS_BODY:
		progress_bar.value = http.get_downloaded_bytes() / float(http.get_body_size());
		cout.visible_characters = 11 + visible_dots
		visible_dots += 1
		visible_dots %= 4
		await get_tree().process_frame;
	install_cs()
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
var cout : Label
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
func on_hide():
	if current_state == State.NONE:
		hide();
	
func fail(message: String):
	current_state = State.NONE
	login.show()
	install.hide()
	error.text = message
func download() -> void:
	var token = await get_token(username.text, password.text)
	current_state = State.DOWNLOAD
	login.hide()
	install.show()
	await download_version(FmodAudioServer.get_version_number().replace('.',''),token,"linux");	
func install_cs() -> void:
	progress_bar.value = 1.0
	var filename = "fmodstudioapi%slinux.tar.gz" % FmodAudioServer.get_version_number().replace(".","")
	var dict = OS.execute_with_pipe("python3", ["addons/FmodGodot/fmod_installer.py","--noprompts", "install_cs", ProjectSettings.globalize_path( get_script_dir().path_join(filename)), "addons/FmodGodot"])	
	process_id = dict["pid"]
	stdio = dict["stdio"]
	stderr = dict["stderr"]
	login.hide()
	install.show()
	current_state = State.INSTALLING
func install_cs_gd() -> void:
	pass
func submit() -> void:
	download()
var process_id : int
# Called every frame. "delta" is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	match(current_state):
		State.NONE:
			return
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
					login.show()
					install.hide();
			pass
func _unhandled_input(event: InputEvent) -> void:
	if(event.is_pressed() && event.is_action("ui_cancel")):
		on_hide();


