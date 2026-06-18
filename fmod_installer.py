#!/usr/bin/env python3
import logging
from os.path import isdir
from sys import platform
import tarfile
import gzip
import os
from typing import IO
import requests
import argparse
import getpass
import platform
import shutil
import subprocess

ERR_LOGIN = 1
ERR_DOWNLOAD = 2
ERR_SO_NUMBER = 3
ERR_INSTALL = 4
ERR_UNSUPPORTED_OS = 5

script_dir = os.path.dirname(os.path.abspath(__file__))
fmodlink = "https://www.fmod.com/api-login"
fmod_downloads_link = "https://www.fmod.com/api-downloads"
current_os = ""

def get_token(username : str, password : str) -> str:
    response = requests.post(fmodlink, auth = (username, password))
    if response.status_code < 200 or response.status_code >= 300:
        logging.error("Failed to login")
        exit(ERR_LOGIN);
    
    return response.json()["token"]

def list_versions(token : str):
    json = requests.get(fmod_downloads_link, headers = {"Authorization": f"FMOD {token}"}).json()
    categories = json["downloads"]["categories"]
    for category in categories:
        if category["title"] == "FMOD Engine" :
            for product in category["products"]:
                for version in product["versions"] :
                    if version["version"].startswith("2"):
                        print(version["version"])
def get_download_link(version:str, platform : str):
    if platform == 'linux':
        # linux
        return f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename=fmodstudioapi{version}linux.tar.gz&user='
    elif platform == 'macos':
        # OS X
        return f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Mac/&filename=fmodstudioapi{version}mac-installer.dmg&user='
    elif platform == 'windows':
        # Windows...
        return f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Windows/&filename=fmodstudioapi{version}win-installer.exe&user='
    elif platform == 'android':
        # Android...
        return f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Android/&filename=fmodstudioapi{version}android.tar.gz&user='
    elif platform == 'ios':
        # iOS...
        return f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/iOS/&filename=fmodstudioapi{version}ios-installer.dmg&user=$1'

def get_file_name(version:str, platform : str) -> str:
    if platform == 'linux':
        # linux
        return f'fmodstudioapi{version}linux.tar.gz'
    elif platform == 'macos':
        # OS X
        return f'fmodstudioapi{version}osx.dmg'
    elif platform == 'windows':
        # Windows...
        return f'fmodstudioapi{version}win-installer.exe'
    elif platform == 'android':
        # Android...
        return f'fmodstudioapi{version}android.tar.gz'
    elif platform == 'ios':
        # iOS...
        return f'fmodstudioapi{version}ios.dmg'
    else:
        return f'fmodstudioapi{version}linux.tar.gz'

def download_version(version : str, token : str, target_platform : str):
    filename = get_file_name(version, target_platform)
    downloadlink = get_download_link(version, target_platform)
    if(downloadlink == None):
        print("Failed to get download")
        exit (ERR_DOWNLOAD)
    # Next request a download link using the token!
    response = requests.get(downloadlink, headers = {"Authorization": f"Bearer {token}"})
    if response.status_code < 200 and response.status_code >= 300 :
        logging.error("Failed to get download link")
        exit(ERR_DOWNLOAD)
    url = response.json()["url"]
    print("Received download link", url)
    response = requests.get(url, allow_redirects=True, stream=True)
    total_bytes = int(response.headers.get("content-length",0))
    total_kilobytes = total_bytes / 1000
    total_megabytes = total_kilobytes / 1000
    total_gigabytes = total_megabytes / 1000
    factor = 1
    unit = "B"
    if total_gigabytes > 1:
        factor = 1000000000
        unit = "GB"
    elif total_megabytes > 1:
        factor = 1000000
        unit = "MB"
    elif total_kilobytes > 1:
        factor = 1000
        unit = "KB"

    block_size = 1024
    progress = 0
    try:
        with open(os.path.join(script_dir, filename), 'wb') as file:
            for data in response.iter_content(block_size):
                file.write(data)
                progress += block_size
                if progress > total_bytes:
                    progress %= total_bytes
                progress_bar(progress/factor, total_bytes / factor, unit, 2)
    except:
        exit(ERR_DOWNLOAD)

def progress_bar(partial : float, total: float,unit : str, rnd: int):
    try:
        prog = str(round(partial ,rnd)) + unit + "/" + str(round(total,rnd)) + unit + ' ' + str(round((partial/total) * 100, 2)) + '% '
        print("\r" + prog, end = "")
        for i in range(len(prog),os.get_terminal_size().columns,1):
            if i == len(prog):
                print('[', end='')
            elif i == os.get_terminal_size().columns-1:
                print(']', end='')
            elif ((i - len(prog))/(os.get_terminal_size().columns - len(prog))) < partial / total:
                print('#', end='')
            else:
                print('-', end='')
    except:
        return
def get_so_number(fmodtar: str) -> int:
    with tarfile.open(fmodtar, mode="r") as tar:
        for name in tar.getnames():
            last_sep = name.rfind('/')
            last_sep = max(last_sep,name.rfind('\\'))
            filename = name[last_sep+1:]
            if ".so." in filename:
                split_name = name.split('.')
                if len(split_name) > 2:
                    return int(split_name[2]);
        print("could not get so number")
        exit(ERR_SO_NUMBER)
vector_cast_snippet = \
"""        public static implicit operator Godot.Vector3(VECTOR v)
        {
            return new Godot.Vector3(v.x, v.y, v.z);
        }
        public static implicit operator VECTOR(Godot.Vector3 v)
        {
            return new VECTOR()
            {
                x = v.X,
                y = v.Y,
                z = v.Z,
            };
        }
"""
fmod_version_snippet = \
"""#if DEBUG || TOOLS
        public const string logging = "L";
#else
        public const string logging = "";
#endif
#if GODOT_LINUXBSD
        public const string dll = $"fmod{logging}.so.%s";
#else
        public const string dll = $"fmod{logging}";
#endif
"""
fmod_studio_version_snippet = \
"""#if DEBUG || TOOLS
        public const string logging = "L";
#else
        public const string logging = "";
#endif
#if GODOT_LINUXBSD
        public const string dll = $"fmodstudio{logging}.so.%s";
#else
        public const string dll = $"fmodstudio{logging}";
#endif
"""
def copy_fmod_file(src_file: IO[bytes] | None,output_dir: str ,filename: str, so_number: int):
    if src_file == None:
        return
    with open(os.path.join(output_dir,filename),'w') as file:
        if src_file != None:
            while True:
                line = src_file.readline().decode('utf-8').replace('\r','')
                if not line:
                    break
                if filename == "fmod.cs":
                    if "VERSION" in line and "class" in line:
                        file.write(line)
                        file.write(src_file.readline().decode('utf-8').replace('\r',''))
                        file.write(src_file.readline().decode('utf-8').replace('\r',''))
                        file.write(fmod_version_snippet % so_number)
                        while not "}" in line:
                            line = src_file.readline().decode('utf-8')
                    if "VECTOR" in line and "struct" in line:
                        while not "}" in line:
                            file.write(line)
                            line = src_file.readline().decode('utf-8').replace('\r','')
                        file.write(src_file.readline().decode('utf-8').replace('\r',''))
                        file.write(vector_cast_snippet)
                elif filename == 'fmod_studio.cs':
                    if "STUDIO_VERSION" in line and "class" in line:
                        file.write(line)
                        file.write(src_file.readline().decode('utf-8').replace('\r',''))
                        file.write(fmod_studio_version_snippet % so_number)
                        while not "}" in line:
                            line = src_file.readline().decode('utf-8').replace('\r','')
                file.write(line);

def install_cs_func(fmodtar: str,output_directory: str):
    print("Installing")
    try:
        with tarfile.open(fmodtar, mode="r") as tar:
            for name in tar.getnames() :
                if name.endswith(".cs"):
                    last_sep = name.rfind('/')
                    last_sep = max(last_sep,name.rfind('\\'))
                    filename = name[last_sep+1:]
                    copy_fmod_file(tar.extractfile(member=name),output_directory,filename,get_so_number(fmodtar)); 
    except:      
            exit(ERR_INSTALL)
def get_os()-> str:
    match platform.system():          
        case "Linux":
            return "linux"
        case "Windows":
            return "windows"
        case _:
            print("Unsupported system %s" % platform.system())
            exit(ERR_UNSUPPORTED_OS)
def copy_headers_and_libs(base_path:str, platform: str):
    core_inc = os.path.join(base_path, "core", "inc")
    core_lib = os.path.join(base_path, "core", "lib")
    studio_inc = os.path.join(base_path, "studio", "inc")
    studio_lib = os.path.join(base_path, "studio", "lib")
    inc_dst = os.path.join(script_dir, "headers");
    lib_dst = os.path.join(script_dir, "libs", platform);
    os.makedirs(lib_dst, exist_ok=True);
    os.makedirs(inc_dst, exist_ok=True);
    for f in os.listdir(core_inc):
        shutil.copy(os.path.join(core_inc, f), inc_dst)
    for f in os.listdir(studio_inc):
        shutil.copy(os.path.join(studio_inc, f), inc_dst)
    shutil.copytree(core_lib, os.path.join(script_dir, "libs", platform),dirs_exist_ok=True)
    shutil.copytree(studio_lib, os.path.join(script_dir, "libs", platform),dirs_exist_ok=True)

def setup_linux(token : str, version : str):
    download_version(version, token, 'linux')
    filename : str = get_file_name(version, 'linux')
    api = filename.removesuffix(".tar.gz")
    with tarfile.open(filename, 'r') as tar:
        tar.extractall()
    os.remove(os.path.join(script_dir, filename))
    base_path = os.path.join(script_dir, api, "api")
    copy_headers_and_libs(base_path=base_path, platform="linux")

def setup_windows(token : str, version : str):
    download_version(version, token, 'windows')
    filename : str = get_file_name(version, 'windows')
    subprocess.run([os.path.join(script_dir, filename), "/S"])
    
    base_path = os.path.join(os.environ["ProgramFiles(x86)"], "FMOD SoundSystem", "FMOD Studio API Windows", "api")
    lib_dst = os.path.join(script_dir, "libs", "windows");
    copy_headers_and_libs(base_path=base_path, platform="windows")
    if isdir(os.path.join(lib_dst, "x86_64")):
        shutil.rmtree(os.path.join(lib_dst, "x86_64"))
    shutil.move(os.path.join(lib_dst, "x64"), os.path.join(lib_dst, "x86_64"))

def main():
    parser = argparse.ArgumentParser(
                    prog='FmodInstaller',
                    description='Download the fmod library to finish setting up the plugin')
    subparsers = parser.add_subparsers(dest="command", help="Descriptions", required=True, description="")
    subparsers.add_parser(name= "list_versions",help="List FMOD versions", description="List the fmod versions available for download",)
    dv = subparsers.add_parser(name= "download_version",help="Download FMOD Version", description="Download the specified version from the web")
    cs = subparsers.add_parser(name= "install_cs",help="copy fmod C# wrappers to directory")
    st = subparsers.add_parser(name= "setup", help="setup up repo to build extension")
    cs.add_argument("tarfile")
    cs.add_argument("output_directory")
    dv.add_argument("fmod_version")
    st.add_argument("fmod_version")
    parser.add_argument("-u", "--username")
    parser.add_argument("-p", "--password")
    parser.add_argument("-t","--targetplatform")
    parser.add_argument("--noprompts", action="store_true")
    args = parser.parse_args()

    username = args.username
    password = args.password
    current_os = get_os();
    if args.targetplatform == None:
        args.targetplatform = get_os() 
    if args.command != "install_cs" and not args.noprompts:
        if not args.username :
            password = input("Enter FMOD account username:\n")
        if not args.password :
            password = getpass.getpass("Enter FMOD account password:\n")
    match args.command:
        case "install_cs":
            install_cs_func(args.tarfile, args.output_directory)
            print("Install Complete")
        case "list_versions":
            token = get_token(username, password)
            list_versions(token)
        case "download_version":
            token = get_token(username, password)
            version : str = args.fmod_version.replace('.', '')
            download_version(version, token, args.targetplatform)
        case "setup":
            token = get_token(username, password)
            version : str = args.fmod_version.replace('.', '')
            match current_os:
                case "linux":
                    setup_linux(token, version)
                case "windows":
                    setup_windows(token, version)
                case _:
                    print("Unsupported system %s" % platform.system())
                    exit(ERR_UNSUPPORTED_OS)
        case _:
            print("Unknown Command: %s" % args.command)
            exit(1);
    exit(0)
        
    
if __name__ == '__main__':
    main()