#!/usr/bin/env python3
import os, pathlib, time

headerTemplate =  """#pragma once

// PURPOSE: Provides the Engine with metadata of the build.
// GENERATED CODE --- CHANGED ON BUILD --- ANY MANUAL CHANGES WILL BE OVERWRITTEN.
#define GAME_BUILD_NUMBER {0}
#define GAME_GIT_HASH "{1}"
#define GAME_BUILD_TIME {2}
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
    else:
        return None

# Gets the commit hash from the HEAD or the current commit reference.
def GetHeadSha1(gitDir):
    if (gitDir != None):
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
    bPath = os.path.join(searchDir, "build.txt")
    buildNum = -1

    try:
        with open(bPath, "r+") as f:
            buildNum = int(f.readline())
            f.seek(0)
            f.write(str(buildNum+1))
            f.truncate()
    except:
        with open(bPath, "w") as f:
            f.write(str(buildNum+1))
            f.truncate()
    
    return buildNum + 1

def GetTime():
    return float(time.time())

if __name__ == "__main__":
    scriptDir = pathlib.Path(os.path.dirname(os.path.realpath(__file__)))
    searchDir = scriptDir.parent

    headSha = GetHeadSha1(SearchForGitDirectory(searchDir)).replace("\n", "")

    header = "{0}.h".format(os.path.splitext(__file__)[0])

    with open(header, "w") as f:
        f.write(headerTemplate.format(IncrementBuildNumber(), headSha, GetTime()))
    
    print("Wrote metadata to {0}!".format(header))