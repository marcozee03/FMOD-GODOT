@tool
extends Window

var vector_cast_snippet: String = r"""	public static implicit operator Godot.Vector3(VECTOR v)
	{
		return new Godot.Vector3(v.x, v.y, v.z);
	}
	public static implicit operator VECTOR(Godot.Vector3 v)
	{
		return new VECTOR()
		{
			x = v.X,
			y = v.Y,
			z = v.Z,
		};
	}
"""
var fmod_version_snippet: String = r"""#if GODOT_LINUXBSD
		public const string dll = $"fmod{suffix}.so.%s";
#else
		public const string dll = $"fmod{suffix}";
#endif
"""
var fmod_studio_version_snippet: String = r"""#if GODOT_LINUXBSD
		public const string dll = $"fmodstudio{VERSION.suffix}.so.%s";
#else
		public const string dll = $"fmodstudio{VERSION.suffix}";
#endif
"""


func get_script_dir() -> String:
	return self.get_script().resource_path.get_base_dir()


var fmodlink := "https://www.fmod.com"
var fmodlogin := "https://www.fmod.com/api-login"
var fmod_downloads_link = "https://www.fmod.com/api-downloads"
var current_os = ""
var http: HTTPRequest = HTTPRequest.new()


func _theme_changed() -> void:
	var ctrl: Control = get_node("BG")
	ctrl.add_theme_stylebox_override("panel", get_theme_stylebox("PanelForeground", "EditorStyles"))


func _init() -> void:
	add_child(http)
	theme_changed.connect(_theme_changed)


func _button_down() -> void:
	get_node("BG/Login/UserInputs/password/Show Password").icon = get_theme_icon(
		"GuiVisibilityVisible",
		"EditorIcons",
	)


func _button_up() -> void:
	get_node("BG/Login/UserInputs/password/Show Password").icon = get_theme_icon(
		"GuiVisibilityHidden",
		"EditorIcons",
	)


func _ready():
	get_node("BG/Login/UserInputs/password/Show Password").icon = get_theme_icon(
		"GuiVisibilityHidden",
		"EditorIcons",
	)
	_theme_changed()

	get_node("BG/Login/UserInputs/password/Show Password").button_up.connect(_button_up)
	get_node("BG/Login/UserInputs/password/Show Password").button_down.connect(_button_down)


class Response:
	var result: int = 0
	var code: int = 0
	var headers: PackedStringArray = []
	var body: PackedByteArray = []


func request(
	method: int,
	url: String,
	headers: PackedStringArray = [],
	body: String = "",
) -> Response:
	# Some headers
	headers.append("Accept: */*")

	var err = http.request(url, headers, method, body)
	assert(err == OK) # Make sure all is OK.
	var signal_args: Array = await http.request_completed
	var response: Response = Response.new()
	response.result = signal_args[0]
	response.code = signal_args[1]
	response.headers = signal_args[2]
	response.body = signal_args[3]
	return response


func _draw() -> void:
	pass


func httpget(url: String, headers: PackedStringArray = []) -> Response:
	return await request(HTTPClient.METHOD_GET, url, headers)


func post(url: String, headers: PackedStringArray = [], body: Dictionary = { }) -> Response:
	headers.append("Content-Type: application/json")
	return await request(HTTPClient.METHOD_POST, url, headers, JSON.stringify(body))


func get_token(username: String, password: String):
	var response = await post(
		fmodlogin,
		["Authorization: Basic %s" % (Marshalls.utf8_to_base64("%s:%s" % [username, password]))],
		{ },
	)
	if response.code == HTTPClient.RESPONSE_UNAUTHORIZED:
		fail("The specified username or password is invalid")
		return
	if response.code != HTTPClient.RESPONSE_OK:
		fail("Failed to Login: Unknown Error with status code:%s" % response.code)
		return
	return JSON.parse_string(response.body.get_string_from_utf8())["token"]


func get_file_name(version: String, platform: String):
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


func get_download_link(version: String, platform: String):
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
		return "https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename=fmodstudioapi%sios-installer.dmg&user=$1" % version
	else:
		return null


func get_so_number(fmod_directory: DirAccess) -> int:
	var files = DirAccess.get_files_at(
		fmod_directory.get_current_dir().path_join(get_downloaded_filename()).path_join(
			"api/core/lib/x86_64"
		)
	)
	var regex: RegEx = RegEx.new()
	regex.compile(r"\.so\.([\d]+)\.?\d*")
	for file in files:
		var match := regex.search(file)
		if match == null:
			continue
		return match.get_string(1).to_int()
	return 0


func download_version(version: String, token: String, target_platform: String) -> bool:
	var filename = get_file_name(version, target_platform)
	var downloadlink = get_download_link(version, target_platform)
	if downloadlink == null or filename == null:
		fail("Could not decide what file to download \"Unsupported Platform?\"")
		return false
	# Next request a download link using the token!
	var response = await httpget(downloadlink, ["Authorization: Bearer %s" % token])
	if response.code != HTTPClient.RESPONSE_OK:
		fail("Failed to get Download link")
		return false
	var url = JSON.parse_string(response.body.get_string_from_utf8())["url"]
	# http.download_file = get_script_dir().path_join(filename);
	progress_bar.indeterminate = true
	install_message.text = "Finding Download Link"
	http.download_file = get_script_dir().path_join("%s.tar.gz" % get_downloaded_filename())
	var err = http.request(url)

	var visible_dots := 3
	var status = http.get_http_client_status()
	while status != HTTPClient.STATUS_BODY and status != HTTPClient.STATUS_DISCONNECTED:
		match status:
			HTTPClient.STATUS_RESOLVING:
				install_message.text = "Resolving..."
			HTTPClient.STATUS_CONNECTING, HTTPClient.STATUS_CONNECTED:
				install_message.text = "Connecting..."
			HTTPClient.STATUS_REQUESTING:
				install_message.text = "Requesting..."
			HTTPClient.STATUS_CONNECTION_ERROR:
				fail("Connection Error")
				return false
		install_message.visible_characters = install_message.text.length() - visible_dots
		visible_dots += 1
		visible_dots %= 4
		status = http.get_http_client_status()
		await get_tree().process_frame

	progress_bar.indeterminate = false
	install_message.text = "Downloading..."
	while http.get_http_client_status() == HTTPClient.STATUS_BODY:
		progress_bar.value = http.get_downloaded_bytes() / float(http.get_body_size())
		install_message.visible_characters = 11 + visible_dots
		visible_dots += 1
		visible_dots %= 4
		await get_tree().process_frame
	return true


@export var username: LineEdit
@export var password: LineEdit
@export var login: Control
@export var install: Control
@export var error: Label
@export var install_message: Label
@export var progress_bar: ProgressBar
enum State {
	NONE,
	BUSY,
}
var current_state: State = State.NONE


func get_version_id() -> String:
	return FmodAudioServer.get_version_number().replace(".", "")


func get_downloaded_filename() -> String:
	return "fmodstudioapi%slinux" % get_version_id()


func on_hide():
	if current_state == State.NONE:
		hide()


func fail(message: String):
	show_login()
	password.text = ""
	error.text = message


func download() -> bool:
	var token = await get_token(username.text, password.text)
	if (token == null):
		return false
	show_progress()
	return await download_version(get_version_id(), token, "linux")


func show_login():
	login.show()
	install.hide()
	current_state = State.NONE


func show_progress():
	login.hide()
	install.show()
	current_state = State.BUSY


func install_cs() -> void:
	progress_bar.value = 1.0
	var file = "%s.tar.gz" % get_downloaded_filename()
	var global_downloaded_file_path = ProjectSettings.globalize_path(
		get_script_dir().path_join(file)
	)
	var temp = DirAccess.create_temp("fmod")
	temp.get_current_dir()
	show_progress()
	install_message.text = "Extracting..."
	progress_bar.indeterminate = true
	var visible_dots = 3
	var process = OS.create_process(
		"tar",
		[
			"-xzf",
			global_downloaded_file_path,
			"-C",
			ProjectSettings.globalize_path(temp.get_current_dir()),
		],
	)
	while (OS.is_process_running(process)):
		install_message.visible_characters = install_message.text.length() - visible_dots
		visible_dots += 1
		visible_dots %= 4
		await get_tree().process_frame

	var so_number = get_so_number(temp)
	install_message.text = "Installing Core API Files"
	await get_tree().process_frame
	temp.change_dir(get_downloaded_filename().path_join("api/core/inc"))
	for filename in temp.get_files():
		if filename.get_extension() == "cs":
			copy_cs_file(
				temp.get_current_dir().path_join(filename),
				"res://addons/FmodGodot/%s" % filename,
				so_number,
			)

	install_message.text = "Installing Studio API Files"
	await get_tree().process_frame
	temp.change_dir("../../studio/inc")
	for filename in temp.get_files():
		if filename.get_extension() == "cs":
			copy_cs_file(
				temp.get_current_dir().path_join(filename),
				"res://addons/FmodGodot/%s" % filename,
				so_number,
			)
	DirAccess.remove_absolute(global_downloaded_file_path)
	show_login()


func copy_cs_file(src_path: String, dst_path: String, so_number: int) -> void:
	match src_path.get_file():
		"fmod.cs":
			var src: FileAccess = FileAccess.open(src_path, FileAccess.READ)
			var dst: FileAccess = FileAccess.open(dst_path, FileAccess.WRITE)
			dst.store_line(
				r"""#if DEBUG || TOOLS
#define FMOD_LOGGING
#endif"""
			)
			var line: String = ""
			while src.get_position() < src.get_length():
				line = src.get_line()
				if "UNITY" in line:
					dst.store_line(fmod_version_snippet % so_number)

					while not "}" in line:
						line = src.get_line()
				if "VECTOR" in line and "struct" in line:
					while not "}" in line:
						dst.store_line(line)
						line = src.get_line()
					dst.store_line(vector_cast_snippet)
				dst.store_line(line)
		"fmod_studio.cs":
			var src: FileAccess = FileAccess.open(src_path, FileAccess.READ)
			var dst: FileAccess = FileAccess.open(dst_path, FileAccess.WRITE)
			var line: String = ""
			while src.get_position() < src.get_length():
				line = src.get_line()
				if "UNITY" in line:
					dst.store_line(fmod_studio_version_snippet % so_number)
					while not "}" in line:
						line = src.get_line()
				dst.store_line(line)
		_:
			DirAccess.copy_absolute(src_path, dst_path)


func submit() -> void:
	var success = await download()
	if success:
		await install_cs()
		on_hide()


# Called every frame. "delta" is the elapsed time since the previous frame.
func _unhandled_input(event: InputEvent) -> void:
	if (event.is_pressed() && event.is_action("ui_cancel")):
		on_hide()
