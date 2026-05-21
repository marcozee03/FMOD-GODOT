#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
import platform
from pathlib import Path
from fmod_installer import get_os

supported_platforms = ["linux", "windows"]

script_dir = os.path.dirname(os.path.abspath(__file__))

def buildForPlatform(platform, arch):
    p1 = subprocess.run(
        [
            "scons", 
            "use_static_cpp=no",
            "debug_symbols=no",
            "lto=auto",
            "target=template_release", 
            "platform=%s" % platform, 
            "arch=%s" % arch
        ]
    )
    if p1.returncode != 0:
        print("failed to build for release")
        exit(1)
    p2 = subprocess.run(
        [
            "scons",
            "use_static_cpp=no",
            "debug_symbols=no",
            "lto=auto",
            "target=template_debug",
            "platform=%s" % platform,
            "arch=%s" % arch,
        ]
    )
    if p2.returncode != 0:
        print("failed to build for release")
        exit(1)
    p3 = subprocess.run(
        [
            "scons",
            "use_static_cpp=no",
            "debug_symbols=no",
            "lto=auto",
            "target=editor",
            "platform=%s" % platform,
            "arch=%s" % arch,
        ]
    )
    if p3.returncode != 0:
        print("failed to build for release")
        exit(1)

def exportDependencies(platform, arch):
    os.makedirs(os.path.join(script_dir, "plugin_template", "bin", platform, arch),exist_ok=True)
    for file in os.listdir(os.path.join(script_dir, "libs", platform, arch)):
        shutil.copy(
            os.path.join(script_dir,"libs", platform, arch, file),
            os.path.join(script_dir,"plugin_template","bin", platform, arch, file)
        )
    shutil.copy(os.path.join(script_dir,"fmod_installer.py"),os.path.join(script_dir,"plugin_template"))

def main():
    parser = argparse.ArgumentParser(
        prog="ExportFmodGodot",
        description="builds FmodGodot gdextension for platform and can export addon to a folder",
    )
    subparsers = parser.add_subparsers(dest="command", help="Descriptions", required=True, description="Subcommand to run")
    build = subparsers.add_parser(name= "build",help="Build Plugin", description="Builds the plugins and places it in plugin_templates")
    export = subparsers.add_parser(name= "export",help="Export plugin", description="Copies the contents of plugin_template to destination. Likely <path>/<to>/<project>/addons")
    push = subparsers.add_parser(name= "push",help="Push Dependencies", description="Copies necessary files ie the fmod libs to plugin_template/")
    export.add_argument("destination", type=Path, help="the destination folder")
    export.add_argument("-f", "--force",type=bool, help="Overwrite files in destination")
    build.add_argument("-p", "--platform", type=str, help="the platform to build for")
    build.add_argument(
        "-a", "--architecture", type=str, help="the cpu architecture to build for"
    )
    
    push.add_argument("-p", "--platform", type=str, help="the platform to build for")
    push.add_argument(
        "-a", "--architecture", type=str, help="the cpu architecture to build for"
    )
    args = parser.parse_args()

    os_platform = args.platform
    if os_platform == None:
        os_platform = get_os()
    arch = platform.machine()
    if arch == "AMD64":
        arch = "x86_64"
    if args.architecture != None:
        arch = args.architecture
    match args.command:
        case "build":
            if os_platform == "all":
                for os_platform in supported_platforms:
                    buildForPlatform(os_platform, arch)
                    exportDependencies(os_platform, arch)
            else:
                buildForPlatform(os_platform, arch)
                exportDependencies(os_platform, arch)
        case "export":
            if args.destination == None or not args.destination.is_dir():
                print('"' + str(args.output) + '" does not exist or is not a directory')
                return;
            path = Path(args.output)
            pathA = Path(str(path).removesuffix('"')).joinpath("FmodGodot")
            force = args.force
            if args.force == None:
                force = False
            if pathA.exists():
                if not force:
                    print(str(pathA) + " already exists overrwrite ? (y/n)")
                    input_string = input()
                    if input_string.lower() == "y\n" or input_string.lower() == "yes\n":
                        shutil.copytree("plugin_template", pathA, dirs_exist_ok=True)
                    else:
                        return
                shutil.copytree("plugin_template", pathA, dirs_exist_ok=True)
            else:
                shutil.copytree("plugin_template", pathA)
        case "push":
            if os_platform == "all":
                for os_platform in supported_platforms:
                    buildForPlatform(os_platform, arch)
                    exportDependencies(os_platform, arch)
            else:
                exportDependencies(os_platform, arch)
        case _:
            exit(1)


if __name__ == "__main__":
    main()
