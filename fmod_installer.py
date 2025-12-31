#!/usr/bin/env python3
import logging
import tarfile
import gzip
import os
from typing import IO
import requests
import argparse
import getpass

fmodlink = "https://www.fmod.com/api-login"
fmod_downloads_link = "https://www.fmod.com/api-downloads"

def get_token(username : str, password : str) -> str:
    response = requests.post(fmodlink, auth = (username, password))
    if response.status_code < 200 or response.status_code >= 300:
        logging.error("Failed to login")
        exit(1);
    
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
def download_version(version : str, token : str, username : str):
    filename = f'fmodstudioapi{version}linux.tar.gz'
    downloadlink = f'https://www.fmod.com/api-get-download-link?path=files/fmodstudio/api/Linux/&filename=fmodstudioapi{version}linux.tar.gz&user=' + username
    # Next request a download link using the token!
    response = requests.get(downloadlink, headers = {"Authorization": f"Bearer {token}"})
    if response.status_code < 200 and response.status_code >= 300 :
        logging.error("Failed to get download link")
        exit(2)
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
        with open(filename, 'wb') as file:
            for data in response.iter_content(block_size):
                file.write(data)
                progress += block_size
                if progress > total_bytes:
                    progress %= total_bytes
                progress_bar(progress/factor, total_bytes / factor, unit, 2)
    except:
        exit(2)

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
        exit(3)
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
                        file.write("#if DEBUG || TOOLS\n");
                        file.write("        public const string logging = \"L\";\n")
                        file.write("#else\n")
                        file.write("        public const string logging = \"\";\n")
                        file.write("#endif\n")
                        file.write("#if GODOT_LINUXBSD\n")
                        file.write("        public const string dll = $\"fmod{logging}.so.%s\";\n" % so_number)
                        file.write("#else\n")
                        file.write("        public const string dll = $\"fmod{logging}\";\n")
                        file.write("#endif\n")
                        while not "}" in line:
                            line = src_file.readline().decode('utf-8')
                elif filename == 'fmod_studio.cs':
                    if "STUDIO_VERSION" in line and "class" in line:
                        file.write(line)
                        file.write(src_file.readline().decode('utf-8').replace('\r',''))
                        file.write("#if DEBUG || TOOLS\n");
                        file.write("        public const string logging = \"L\";\n")
                        file.write("#else\n")
                        file.write("        public const string logging = \"\";\n")
                        file.write("#endif\n")
                        file.write("#if GODOT_LINUXBSD\n")
                        file.write("        public const string dll = $\"fmodstudio{logging}.so.%s\";\n" % so_number)
                        file.write("#else\n")
                        file.write("        public const string dll = $\"fmodstudio{logging}\";\n")
                        file.write("#endif\n")
                        while not "}" in line:
                            line = src_file.readline().decode('utf-8')
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
        exit(4)

                
    
def main():
    parser = argparse.ArgumentParser(
                    prog='FmodInstaller',
                    description='Download the fmod library to finish setting up the plugin')
    subparsers = parser.add_subparsers(dest="command", help="Descriptions", required=True, description="")
    subparsers.add_parser(name= "list_versions",help="List FMOD versions", description="List the fmod versions available for download",)
    dv = subparsers.add_parser(name= "download_version",help="Download FMOD Version", description="Download the specified version from the web")
    cs = subparsers.add_parser(name= "install_cs",help="copy fmod C# wrappers to directory")
    cs.add_argument("tarfile")
    cs.add_argument("output_directory")
    dv.add_argument("fmod_version")
    parser.add_argument("-u", "--username")
    parser.add_argument("-p", "--password")
    parser.add_argument("--noprompts", action="store_true")
    args = parser.parse_args()

    username = args.username
    password = args.password
    if args.command != "install_cs" and not args.noprompts:
        if not args.username :
            password = input("Enter FMOD account username:\n")
        if not args.password :
            password = getpass.getpass("Enter FMOD account password:\n")
    if args.command == "list_versions":
        token = get_token(username, password)
        list_versions(token)
    elif args.command == "download_version":
        token = get_token(username, password)
        version : str = args.fmod_version.replace('.', '')
        download_version(version, token, args.username)
    elif args.command == "install_cs":
        install_cs_func(args.tarfile, args.output_directory)
        print("Install Complete")
    exit(0)
        
    
if __name__ == '__main__':
    main()