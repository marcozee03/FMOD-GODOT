#!/usr/bin/env python3
import logging
import curses
from os import get_terminal_size, terminal_size
import os
from re import X
import requests
import sys
import argparse
import getpass

fmodlink = "https://www.fmod.com/api-login"
fmod_downloads_link = "https://www.fmod.com/api-downloads"

def get_token(username : str, password : str) -> str:
    response = requests.post(fmodlink, auth = (username, password)).json()
    return response["token"];

def list_versions(token : str):
    json = requests.get(fmod_downloads_link, headers = {"Authorization": f"FMOD {token}"}).json();
    categories = json["downloads"]["categories"];
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
    response = requests.get(downloadlink, headers = {"Authorization": f"Bearer {token}"});
    if response.status_code < 200 and response.status_code >= 300 :
        logging.error("Failed to get download link")
        exit(-1)
    url = response.json()["url"]
    print("Received download link", url)
    response = requests.get(url, allow_redirects=True, stream=True)
    total_bytes = int(response.headers.get("content-length",0));
    total_kilobytes = total_bytes / 1000;
    total_megabytes = total_kilobytes / 1000;
    total_gigabytes = total_megabytes / 1000;
    factor = 1;
    unit = "B";
    if total_gigabytes > 1:
        factor = 1000000000
        unit = "GB";
    elif total_megabytes > 1:
        factor = 1000000
        unit = "MB";
    elif total_kilobytes > 1:
        factor = 1000
        unit = "KB";

    

    block_size = 1024;
    progress = 0;
    with open(filename, 'wb') as file:
        for data in response.iter_content(block_size):
            file.write(data);
            progress += block_size;
            if progress != total_bytes:
                progress %= total_bytes
            progress_bar(progress/factor, total_bytes / factor, unit, 2)
            # print(progress / total_size);
def progress_bar(partial : float, total: float,unit : str, rnd: int):
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



def main():
    parser = argparse.ArgumentParser(
                    prog='FmodInstaller',
                    description='Helper script for installing fmod godot',
                    epilog='Text at the bottom of help')
    subparsers = parser.add_subparsers(dest="command", required=True)
    lv = subparsers.add_parser(name= "list_versions");
    dv = subparsers.add_parser(name= "download_version");
    dv.add_argument("fmod_version");
    parser.add_argument("-u", "--username");
    parser.add_argument("-p", "--password");
    args = parser.parse_args()

    username = args.username;
    password = args.password;
    if not args.username :
        password = input("Enter FMOD account username:\n");
    if not args.password :
        password = getpass.getpass("Enter FMOD account password:\n");

    token = get_token(args.username, password);
    if args.command == "list_versions":
        list_versions(token);
    elif args.command == "download_version":
        version : str = args.fmod_version.replace('.', '');
        download_version(version, token, args.username);
    
if __name__ == '__main__':
    main();