#!/usr/bin/env python3
import argparse
import os
import shutil
import subprocess
import sys
import platform
from pathlib import Path

supported_platforms = ["linux", "windows"]


def buildForPlatform(platform, arch):
    p1 = subprocess.run(
        ["scons", "target=template_release", "platform=%s" % platform, "arch=%s" % arch]
    )
    if p1.returncode != 0:
        print("failed to build for release")
        exit(1)
    p2 = subprocess.run(
        [
            "scons",
            "debug_symbols=yes",
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
            "debug_symbols=yes",
            "target=editor",
            "platform=%s" % platform,
            "arch=%s" % arch,
        ]
    )
    if p3.returncode != 0:
        print("failed to build for release")
        exit(1)


def exportDependencies(platform, arch):
    for file in os.listdir("libs/%s/%s" % (platform, arch)):
        shutil.copy(
            "libs/%s/%s/%s" % (platform, arch, file),
            "plugin_template/bin/libs/%s/%s/%s" % (platform, arch, file),
        )
    shutil.copy("fmod_installer.py","plugin_template")


def main():
    parser = argparse.ArgumentParser(
        prog="ExportFmodGodot",
        description="builds FmodGodot gdextension for platform and can export addon to a folder",
    )
    parser.add_argument("-o", "--output", type=Path, help="the destination folder")
    parser.add_argument("-p", "--platform", type=str, help="the platform to build for")
    parser.add_argument(
        "-a", "--architecture", type=str, help="the cpu architecture to build for"
    )
    args, unknown = parser.parse_known_args(sys.argv)
    os_platform = args.platform
    arch = platform.machine()
    if arch == "AMD64":
        arch = "x86_64"
    if args.architecture != None:
        arch = args.architecture
    if not sys.argv.__contains__("-sc"):
        if os_platform == None and sys.platform.startswith("linux"):
            os_platform = "linux"
        if os_platform == None and sys.platform.startswith("win"):
            os_platform = "windows"
        if os_platform == "all":
            for os_platform in supported_platforms:
                buildForPlatform(os_platform, arch)
                exportDependencies(os_platform, arch)
        else:
            buildForPlatform(os_platform, arch)
            exportDependencies(os_platform, arch)
    if not args.output.is_dir():
        print('"' + str(args.output) + '" does not exist or is not a directory')
        return

    if args.output != None:
        path = Path(args.output)
        pathA = Path(str(path).removesuffix('"')).joinpath("FmodGodot")
        if pathA.exists():
            if not sys.argv.__contains__("-f"):
                print(str(pathA) + " already exists overrwrite ? (y/n)")
                input_string = input()
                if input_string.lower() == "y\n" or input_string.lower() == "yes\n":
                    shutil.copytree("plugin_template", pathA, dirs_exist_ok=True)
                else:
                    return
            shutil.copytree("plugin_template", pathA, dirs_exist_ok=True)
        else:
            shutil.copytree("plugin_template", pathA)


if __name__ == "__main__":
    main()
