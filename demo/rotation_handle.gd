extends Node2D
@export
var rotating_child : Node2D

@export_custom(PROPERTY_HINT_NONE, "FmodEvent")
var sound : Vector4i;

var dragging : bool = false;

func in_range() -> bool:
	return get_local_mouse_position().length() < 20;
	
func _input(event: InputEvent) -> void:
	#if event.is_action_pressed("ui_accept") :  
		#FmodAudioServer.play_one_shot_by_id(sound, Vector3(10000  , 10000, 100000  ))
	if event is InputEventMouseMotion:
		if dragging:
			rotating_child.rotation = get_local_mouse_position().angle()
		var e : InputEventMouseMotion = event
		queue_redraw()
	if event is InputEventMouseButton :
		var e : InputEventMouseButton = event
		if e.is_pressed() and e.button_index == MOUSE_BUTTON_LEFT:
			if in_range() :
				FmodAudioServer.play_one_shot_by_id(sound, Vector3(get_global_mouse_position().x, get_global_mouse_position().y, 0));
				dragging = true
			else:
				FmodAudioServer.play_one_shot_by_id(sound, Vector3(get_global_mouse_position().x, get_global_mouse_position().y, 100));
		else :
			if dragging :
				rotating_child.rotation = get_local_mouse_position().angle()
			dragging = false
		

func _draw() -> void:
	draw_circle(Vector2.ZERO, 250, Color.GRAY, false, 3, true)
	if dragging :
		draw_circle(get_local_mouse_position().normalized() * 20, 5, Color.DARK_BLUE,true, -1, true);
	if in_range() or dragging :
		draw_circle(Vector2.ZERO, 20, Color.BLUE, false, 3, true)
	else :
		draw_circle(Vector2.ZERO, 20, Color.GRAY, false, 3, true)
	pass
