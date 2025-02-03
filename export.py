#!/usr/bin/env python
import shutil

import subprocess;
import argparse
import os;
import sys;
from pathlib import Path

supported_platforms = ["linux","windows"];
def buildForPlatform(platform):
    p1 = subprocess.run(['scons', 'target=template_release', 'platform=%s' % platform])
    p2 = subprocess.run(['scons', 'target=template_debug', 'platform=%s' % platform])
    p3 = subprocess.run(['scons', 'target=editor', 'platform=%s' % platform])
def exportDependencies(platform):
    for file in os.listdir('libs/%s' % platform) :
        shutil.copy('libs/%s/%s'% (platform,file), 'plugin_template/bin/libs/%s/%s' % (platform,file))
def main():
    parser = argparse.ArgumentParser(prog= 'ExportFmodGodot', description = 'builds FmodGodot gdextension for platform and can export addon to a folder')
    parser.add_argument('-o', '--output', type=Path, help= "the destination folder")
    parser.add_argument('-p', '--platform', type=str, help= "the platform to build for")
    args, unknown = parser.parse_known_args(sys.argv)
    platform = args.platform
    
    if not sys.argv.__contains__('-sc') :
        if platform == None and sys.platform.startswith('linux') :
            platform = "linux"
        if platform == None and sys.platform.startswith('win'):    
            platform = "windows"
        if platform == "all":
            for platform in supported_platforms:
                buildForPlatform(platform)
                exportDependencies(platform)
        else :
            buildForPlatform(platform)
            exportDependencies(platform)
    if args.output.is_dir():
        print("\"" + str(args.output) + "\" does not exist or is not a directory")
        return;

    if(args.output != None):
        path = Path(args.output)
        print(path);
        pathA = Path(str(path).removesuffix('\"')).joinpath('FmodGodot')
        print(pathA);
        if(pathA.exists()):

            if(not sys.argv.__contains__('-f')):
                print(str(pathA) + ' already exists overrwrite ? (y/n)')
                input_string = sys.stdin.readline()
                if(input_string.lower() =='y\n' or input_string.lower() == 'yes\n'):
                   shutil.copytree('plugin_template', pathA, dirs_exist_ok=True)
                else:
                   return;
            shutil.copytree('plugin_template', pathA, dirs_exist_ok=True)
        else:
            shutil.copytree('plugin_template', pathA)


if __name__ == '__main__':
    main()