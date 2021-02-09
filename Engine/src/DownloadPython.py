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

# Base parameters
base_dir = pathlib.Path(__file__).parent
base_url = "https://www.python.org/ftp/python/{}/python-{}-embed-{}.zip"
base_filename = "python-{}-embed-{}.zip"

file_blacklist = [
    "python*._pth"
]

file_copy = [
    "pythonw.exe",
    "python.cat",
    "python.exe",
    "python*.dll"
]

# Initialise variables
build_dir = pathlib.Path(sys.argv[1])
version = sys.argv[2]
arch = sys.argv[3]
config = sys.argv[4]

# Handle input from CMake
if arch == "x64":
    arch = "amd64"
elif arch == "x86":
    arch = "win32"

# Checks
assert build_dir.is_dir() # Meant to be a directory
assert arch == "win32" or arch == "amd64" # Needs to be 'win32' or 'amd64'
assert config == "Debug" or config == "Release" # Needs to be 'Debug' or 'Release'

# Construct our directories we will be working with
download_dir = build_dir.joinpath("tmp")
extracted_dir = pathlib.Path(download_dir, "python")
result_folder = build_dir.joinpath(config, "platform")

# Destination folders
result_DLL_folder = result_folder.joinpath("DLLs")
result_Lib_folder = result_folder.joinpath("Lib")

# Very simple check to see if the platform folder is not empty
# TODO Implement a checksum check or something to validate the files.
if os.path.exists(result_folder) and os.path.isdir(result_folder):
    if os.listdir(result_folder):
        print("\"{}\" is not empty, not redownloading.".format(result_folder))
        sys.exit(0)

# Create the download folder
if (not download_dir.exists()):
    os.mkdir(download_dir)

# Which Python version we are going to download as well as where we are going to extract it.
desired_path = pathlib.Path.joinpath(download_dir, base_filename.format(version, arch))
desired_url = base_url.format(version, version, arch)

print("Downloading Python {} ({})...".format(version, arch))
r = requests.get(desired_url)
print("File downloaded.")

with open(desired_path, 'wb') as f:
    f.write(r.content)

print("*** File information: ***")
print("Filename: {}".format(desired_path.name))
print("HTTP Response: {}".format(r.status_code))
print("MIME type: {} ".format(r.headers['content-type']))
try:
    rbytes = round(((int(r.headers['content-length'])/1024)/1024), 2)
except:
    rbytes = "(unknown)"
print("Size: {} MiB".format(rbytes))
print("*** File meta end ***")

# Extract the Zip we downloaded
with zipfile.ZipFile(desired_path, 'r') as zipObj:
    zipObj.extractall(extracted_dir)

# Delete files we do not need for distribution
for i in file_blacklist:
    for p in extracted_dir.glob(i):
        p.unlink()
        print("Deleted \"{}\"".format(p))

# Make the folders we are copying folders to
os.makedirs(result_DLL_folder,  exist_ok=True)
os.makedirs(result_Lib_folder,  exist_ok=True)

with open(result_folder.joinpath("README.txt"), 'w') as f:
    print("This is the distributed Python install for the Engine.", file=f)
    print("Place thirdparty libraries in 'Lib/site-packages' like you would for a regular Python install.", file=f)
    print("However, keep in mind some libraries may not be compatible with the environment.", file=f)
    f.flush()

for i in file_copy:
    for p in extracted_dir.glob(i):
        diPath = result_folder.joinpath(p.name)
        shutil.move(p, diPath)
        print("Moved \"{}\" to \"{}\"".format(p, diPath))

# Copying files to other directory      
for k in os.listdir(extracted_dir):
    cPath = pathlib.Path(extracted_dir, k)
    if cPath.suffix == ".zip":
        # Extract the common library, since our build is
        # compiled with gzip for some unknown reason
        with zipfile.ZipFile(cPath, 'r') as zipObj:
            zipObj.extractall(result_Lib_folder)
        os.remove(cPath)
    else:
        # Copy all the pyd files
        dPath = result_DLL_folder.joinpath(cPath.name)
        if cPath.exists():
            shutil.move(cPath, dPath)
            print("Moved \"{}\" to \"{}\"".format(cPath, dPath))

shutil.rmtree(download_dir) # Delete the old folder
print("Cleaned up download folder")