# About
Fmod Godot C# is a GDExtension library that integrates the Fmod API with the Godot Engine that provides.
unlike the wonderful [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension) by [utopia-rise](https://github.com/utopia-rise)
does not necessarily aim to be a fully-fledged engine integration. It provides some nodes for convenience but is meant to pass 
the FMOD_System_Core and Fmod_Studio_Core objects to C# code to then be used with the official FMOD C# wrapper.
# Features
Add a FmodEvent tag to strings or Vector4Is to choose an FMOD_Event from the
inspector to be passed to FMOD API Functions <sub>*Convert will Vector4i to an FMOD::GUID with an implicit castand vice versa</sub>
```
[Export(PropertyHint.None,"FMODEvent")]
public string soundFX1;
[Export(PropertyHint.None,"FMODEvent")]
public Vector4I soundFX2;
```
## Nodes
 - **BankLoader**:
   this node loads the banks in its bank list when it enters the scene tree. and unload them if no other bank loader node is loading the bank.
   
# Installation
\[WIP\]
