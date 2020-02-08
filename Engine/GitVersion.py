#!/usr/bin/env python3
import os, pathlib, time

headerTemplate =  """#pragma once

// GENERATED CODE --- CHANGED ON BUILD --- ANY MANUAL CHANGES WILL BE OVERWRITTEN.
#define GAME_BUILD_NUMBER {0}
#define GAME_GIT_HASH "{1}"
#define GAME_BUILD_TIME {2}
"""

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

def GetHeadSha1(gitDir):
    if (gitDir != None):
        # The path to the commit path
        cPath = ""
        with open(os.path.join(gitDir, "head"), "r") as f:
            cPath = os.path.join(gitDir, str(f.readline()).replace("\n", "").replace(" ","")[4:])
        
        with open(cPath, "r") as f:
            return (f.readline())

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

    with open("{0}.h".format(os.path.splitext(__file__)[0]), "w") as f:
        f.write(headerTemplate.format(IncrementBuildNumber(), headSha, GetTime()))
         