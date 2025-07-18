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
env.Append(CPPPATH=["src/", "src/scene/", "src/scene/2d/", "src/scene/3d","src/editor/", "src/editor/plugins/",
                     "src/editor/nodes/","headers/","include/", "include/scene/", "include/scene/2d", "include/scene/3d",
                       "include/editor/", "include/editor/plugins/", "include/editor/nodes"])
logging = ''
if env["target"] == "editor" or env["target"] == "template_debug" :
    logging = "L"

sources = [
    Glob('src/*.cpp'),
    Glob('src/scene/*.cpp'),
    Glob('src/scene/2d/*.cpp'),
    Glob('src/scene/3d/*.cpp'),
    Glob('src/editor/*.cpp'),
    Glob('src/editor/plugins/*.cpp'),
    Glob('src/editor/nodes/*.cpp'),
    ]
env.Append(LIBPATH=["libs/%s" % env["platform"]])
if env["platform"] == "windows":
    env.Append(LIBS=["fmod_vc","fmodstudio_vc"])
    library = env.SharedLibrary(
        "plugin_template/bin/libs/windows/libFmodGodot{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources, 
    )
elif env["platform"] == "linux":
    env.Append(LIBS=["libfmod%s" % logging,"libfmodstudio%s" % logging])
    print(env["LIBS"])
    library = env.SharedLibrary(
        "plugin_template/bin/libs/linux/libFmodGodot{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources, 
    )

Default(library)
