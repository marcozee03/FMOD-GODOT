# About
Fmod-Godot is a GDExtension library that integrates the Fmod API with the Godot Engine. The library provides a way for C# to interact directly with the fmod studio instance the FmodAudioServer uses to directly use the lower-level fmod studio and core apis.

# Features
## Event tags
Add a FmodEvent tag to strings or Vector4Is to choose an FMOD_Event from the
inspector to be passed to FMOD API Functions <sub>*Convert will Vector4i to an FMOD::GUID with an implicit castand vice versa</sub>  
#### C#
```
[Export(PropertyHint.None,"FmodEvent")]
public string soundFX1;
[Export(PropertyHint.None,"FmodEvent")]
public Vector4I soundFX2;
```
#### GDScript
```
@export_custom(PROPERTY_HINT_NONE, "FmodEvent)
var soundfx1 : String
@export_custom(PROPERTY_HINT_NONE, "FmodEvent)
var soundfx2 : Vector4I
```
![](images/exported_event.png)

## Event Browser
The editor will load all of the banks in the banks folder. and cache them at startup. to be referenced by inspectors. you can drag and drop events from the browser


![](images/event_browser.png)  
### Completion
 ✅ Events  
 ✅ Banks  
❌ VCA  
❌ [ ] Parameter

## Class Overview
 - ![](./plugin_template/icons/fmod_icon.svg) **FmodAudioServer** API to play events. and attach events to objects for spatialization updating positions and velocity.

 - ![](plugin_template/icons/bank_loader_icon.svg)**FmodBankLoader**:
   This node loads the banks in its bank list when it enters the scene tree. and unload them if no other bank loader node is loading the bank. banks loaded this way are reference counted.

 - ![](plugin_template/icons/bank_icon.svg)**FmodBank**: Fmod Bank files are imported into godot and exported automatically. Banks can be opened in the inspector to view what events they contain and their GUID.  
  ![](images/bank_inspector_example.png)

 - ![](plugin_template/icons/FmodEventEmitter2D.svg) **FmodEventEmitter2D/3D**: Plays selected sound effect. Edit parameters, Spatialize the audio and more \[WIKI goes here (WIP)\] 

 - ![](plugin_template/icons/FmodListener2D.svg) **FmodListener2D/3D**: place listeners into the scene that automatically update their position.

# Supported Platforms
| Platform | Support | Notes                                             |
|----------|---------|---------------------------------------------------|
|Linux     |✅       |Manually Tested                                    |
|Windows   |🟨       |Untested should just need to modify SConstruct file|
|Other     |❌       |Untested/Unsupported                               | 
# Installation and Getting Started
Open project Settings to adjust settings like error logging, banks loaded at runtime and more.
## Compiling From Source
\[WIP\]
## Pre-built packages
\[WIP none currently\]


# C# roadmap
currently C# should be able to acces the Fmod Studio and Core Systems with a tweak to the fmod C# wrapper. For GDextension it is planned to add additional interop with the audio server. and remove the old PlayOneShot methods in FmodAudioServer.cs. A module version of this extension is in consideration for better C# support
