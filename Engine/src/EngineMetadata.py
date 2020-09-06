#!/usr/bin/env python3
import os
import time
import pathlib
import sys
import subprocess

headerTemplate =  """#pragma once

// PURPOSE: Provides the Engine with metadata of the build.
// GENERATED CODE --- CHANGED ON BUILD --- ANY MANUAL CHANGES WILL BE OVERWRITTEN.
#define GAME_BUILD_NUMBER   {0}
#define GAME_GIT_HASH      "{1}"
#define GAME_BUILD_TIME     {2}
#define GAME_GIT_DESC      "{3}"
"""

# Searches for .git directory recursively until it is found.
def SearchForGitDirectory(path):
    di = os.listdir(path)
    pi = pathlib.Path(path)

    for i, j in enumerate(di):
        if j == ".git":
            return os.path.join(path, di[i])
    
    if(pi.parent != pi):
        return SearchForGitDirectory(str(pi.parent))

# Gets the commit hash from the HEAD or the current commit reference.
def GetHeadSha1(gitDir):
    if (gitDir is None):
        return
    
    # The path to the commit path
    cPath = ""
    with open(os.path.join(gitDir, "HEAD"), "r") as f:
        head = f.readline()
        # Does the HEAD contain a hash or a reference?
        if ("ref:" in head):
            cPath = os.path.join(gitDir, str(head).replace("\n", "").replace(" ","")[4:])
        else:
            cPath = os.path.join(gitDir, "HEAD")
            
    # Return the hash obtained.
    with open(cPath, "r") as f:
        return (f.readline())

# Gets the build number from the persistent text file.
def IncrementBuildNumber():
    searchDir = os.path.dirname(os.path.realpath(__file__))
    bPath = os.path.join(searchDir, "WarspiteEngine.Build.txt")
    buildNum = -1
    
    print("Writing build number to persistent file...")
    
    # Try to change the value if the file exists.
    try:
        with open(bPath, "r+") as f:
            buildNum = int(f.readline())
            f.seek(0)
            f.write(str(buildNum+1))
            f.truncate()
    except FileNotFoundError:
        print("Build text file not found - creating...")
    except OSError as e:
        print("An OSError occurred when writing.")
        print("Exception:\n{0}".format(str(e)))
        print("Attempting to write again...")
    except Exception as e:
        print("An Error occurred when writing.")
        print("Exception:\n{0}".format(str(e)))
        print("Attempting to write again...")
        
    # Try writing again
    with open(bPath, "w") as f:
        f.write(str(buildNum+1))
        f.truncate()
    
    print("Successfully wrote build number {0} to disk!".format(str(buildNum+1))) 
    
    return buildNum + 1

def GetTime():
    return float(time.time())

def GetGitDesc():
    try:
        return str(subprocess.check_output(["git", "describe", "--always"]).strip().decode("utf-8"))
    except FileNotFoundError:
        return "undefined"

if __name__ == "__main__":
    print("Modifying metadata, please wait...")

    scriptDir = pathlib.Path(os.path.dirname(os.path.realpath(__file__)))
    searchDir = scriptDir.parent

    headSha = GetHeadSha1(SearchForGitDirectory(searchDir)).replace("\n", "")

    header = "../../shared/src/{0}.h".format(os.path.splitext(os.path.basename(__file__))[0])

    with open(header, "w") as f:
        f.write(headerTemplate.format(IncrementBuildNumber(), headSha, GetTime(), GetGitDesc()))
    
    print("Wrote metadata to {0}!".format(os.path.abspath(header)))