extends Camera3D


@export
var MOUSE_SENSITIVITY = .003
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
func _input(event: InputEvent)-> void:
	if event is InputEventMouseMotion and Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		# Rotate parent horizontally (yaw)
		get_parent().rotate_y(-event.relative.x * MOUSE_SENSITIVITY)
		# Rotate camera vertically (pitch)
		rotate_object_local(Vector3.RIGHT, -event.relative.y * MOUSE_SENSITIVITY)
		
		# Clamp vertical rotation to prevent flipping
		rotation.x = clamp(rotation.x, deg_to_rad(-90), deg_to_rad(90))
