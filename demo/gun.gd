extends CSGBox3D

@export
var explo: Node3D;
@export
var ray: RayCast3D;
@export_custom(PROPERTY_HINT_NONE, "FMODEvent")
var explosion: Vector4i;

signal shoot()
func _input(event: InputEvent) -> void:	
	if event is InputEventMouseButton:
		var mm : InputEventMouseButton = event
		if event.is_pressed() && mm.button_index == MOUSE_BUTTON_LEFT:
			if ray.is_colliding():
				FmodAudioServer.play_one_shot_by_id(explosion,ray.get_collision_point())
				explo.global_position = ray.get_collision_point();
				explo.visible = true
			shoot.emit()
