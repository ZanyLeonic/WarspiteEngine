#!/usr/bin/env python3
import pathlib
import sys
import os
import json
import glob
import hashlib
import requests
import shutil
import zipfile

# def getFiles(dirName):
#     listOfFile = os.listdir(dirName)
#     completeFileList = list()
#     for file in listOfFile:
#         completePath = os.path.join(dirName, file)
#         if os.path.isdir(completePath):
#             completeFileList = completeFileList + getFiles(completePath)
#         else:
#             item = {completePath : hashFile(completePath)}
#             completeFileList.append(item)
#     return completeFileList

# def hashFile(fileName):
#     BLOCKSIZE = 65536
#     hasher = hashlib.sha1()
#     with open(fileName, 'rb') as afile:
#         buf = afile.read(BLOCKSIZE)
#         while len(buf) > 0:
#             hasher.update(buf)
#             buf = afile.read(BLOCKSIZE)
#     return hasher.hexdigest()

# Base parameters
base_dir = pathlib.Path(__file__).parent
base_url = "https://www.python.org/ftp/python/{}/python-{}-embed-{}.zip"
base_filename = "python-{}-embed-{}.zip"

file_blacklist = [
    "python*.dll",
    "python*._pth",
    "pythonw.exe",
    "python.cat",
    "python.exe"
]

# Initialise variables
build_dir = pathlib.Path(sys.argv[1])
version = sys.argv[2]
arch = sys.argv[3]
config = sys.argv[4]

if arch == "x64":
    arch = "amd64"
elif arch == "x86":
    arch = "win32"

assert build_dir.is_dir() # Meant to be a directory
assert arch == "win32" or arch == "amd64" # Needs to be 'win32' or 'amd64'
assert config == "Debug" or config == "Release" # Needs to be 'Debug' or 'Release'

download_dir = build_dir.joinpath("tmp")
extracted_dir = pathlib.Path(download_dir, "python")
result_folder = build_dir.joinpath(config, "platform")

if os.path.exists(result_folder) and os.path.isdir(result_folder):
    if os.listdir(result_folder):
        print("\"{}\" is not empty, not redownloading.".format(result_folder))
        sys.exit(0)

# Create the download folder
if (not download_dir.exists()):
    os.mkdir(download_dir)

desired_path = pathlib.Path.joinpath(download_dir, base_filename.format(version, arch))
desired_url = base_url.format(version, version, arch)

print("Downloading Python {} ({})...".format(version, arch))
r = requests.get(desired_url)
print("File downloaded.")

with open(desired_path, 'wb') as f:
    f.write(r.content)

print("*** File information: ***")
print(r.status_code)
print(r.headers['content-type'])
print(r.encoding)
print("*** File meta end ***")

with zipfile.ZipFile(desired_path, 'r') as zipObj:
    zipObj.extractall(extracted_dir)

for i in file_blacklist:
    for p in extracted_dir.glob(i):
        p.unlink()
        print("Deleted \"{}\"".format(p))

os.makedirs(result_folder.joinpath("DLLs"), exist_ok=True)
os.makedirs(result_folder.joinpath("Libs"), exist_ok=True)
os.makedirs(result_folder.joinpath("sites-packages"), exist_ok=True)

with open(result_folder.joinpath("sites-packages", ".empty"), 'w') as f:
    f.write("Place 3rd parties libaries here.")

# Copying files to other directory      
for k in os.listdir(extracted_dir):
    cPath = pathlib.Path(extracted_dir, k)
    if cPath.suffix == ".zip":
        with zipfile.ZipFile(cPath, 'r') as zipObj:
            zipObj.extractall(result_folder.joinpath("Libs"))
        os.remove(cPath)
    else:
        dPath = result_folder.joinpath("DLLs", cPath.name)
        if cPath.exists():
            shutil.move(cPath, dPath)
            print("Moved \"{}\" to \"{}\"".format(cPath, dPath))

shutil.rmtree(download_dir) # Delete the old folder
print("Cleaned up download folder")
# Caching
# outList = getFiles(result_folder)
# cache_list['files'] = outList

# with open(pathlib.Path(base_dir, "PythonInfo.json"), 'w') as f:
#     json.dump(cache_list, f)