#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
source_path = [
    os.path.join("godot-cpp", "include", "godot_cpp"),
    os.path.join("godot-cpp", "gen", "include", "godot_cpp"),
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
env.Append(
    CPPPATH=[
        "src/",
        "src/scene/",
        "src/scene/2d/",
        "src/scene/3d",
        "src/editor/",
        "src/editor/plugins/",
        "src/editor/nodes/",
        "headers/",
        "include/",
        "include/scene/",
        "include/scene/2d",
        "include/scene/3d",
        "include/editor/",
        "include/editor/plugins/",
        "include/editor/nodes",
    ]
)
logging = ""
if env["target"] == "editor" or env["target"] == "template_debug":
    logging = "L"
sources = [
    Glob("src/*.cpp"),
    Glob("src/scene/*.cpp"),
    Glob("src/scene/2d/*.cpp"),
    Glob("src/scene/3d/*.cpp"),
    Glob("src/editor/*.cpp"),
    Glob("src/editor/plugins/*.cpp"),
    Glob("src/editor/nodes/*.cpp"),
]
if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData(
            "src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml")
        )
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

env.Append(LIBPATH=["libs/%s/%s" % (env["platform"], env["arch"])])
libname = "libFmodGodot.{}{}".format(env["target"], env["SHLIBSUFFIX"])
if env["platform"] == "windows":
    env.Append(LIBS=["fmod_vc", "fmodstudio_vc"])
elif env["platform"] == "linux":
    env.Append(LIBS=["libfmod%s" % logging, "libfmodstudio%s" % logging])
    env.Append(LINKFLAGS=["-Wl,-soname,{}.1".format(libname)])
library = env.SharedLibrary(
    "plugin_template/bin/libs/{}/{}/{}".format(env["platform"], env["arch"], libname),
    source=sources,
)
Default(library)
