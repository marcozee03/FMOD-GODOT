#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
source_path = [
    os.path.join("godot-cpp", "include","godot_cpp"),
    os.path.join("godot-cpp", "gen", "include","godot_cpp")
]
env.Append(CPPPATH=[env.Dir(d) for d in source_path])
# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/","Headers/",'src/nodes/','src/editor/','src/nodes/editor_ui'])
sources = [
    Glob('src/*.cpp'),
    Glob('src/nodes/*.cpp'),
    Glob('src/editor/*.cpp'),
    Glob('src/nodes/editor_ui/*.cpp')
    ]
env.Append(LIBPATH=["libs/%s" % env["platform"]])
if env["platform"] == "windows":
    env.Append(LIBS=["fmod_vc","fmodstudio_vc"])
    library = env.SharedLibrary(
        "demo/bin/libs/windows/libFmodGodot{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources, 
    )
elif env["platform"] == "linux":
    env.Append(LIBS=["libfmod","libfmodstudio"])
    library = env.SharedLibrary(
        "demo/bin/libs/linux/libFmodGodot{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources, 
    )

Default(library)
