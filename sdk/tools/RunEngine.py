#!/usr/bin/env python3

import sys
import os

from pathlib import Path
from subprocess import Popen, PIPE

appName = "WarspiteEngine SDK: Run Engine"
appVer = "1.0.0.0"
appDesc = """Runs the Engine with the specified map."""

def DoesParameterExist(parmName):
    for i in sys.argv:
        if i == parmName:
            return True
    return False

def GetParameterValue(parmName, required=False):
    for i, j in enumerate(sys.argv):
        if j == parmName and (i+1 < len(sys.argv)):
            if not sys.argv[i+1].startswith("-"):
                return sys.argv[i+1]

    if required:
        raise RuntimeError("Cannot find specified required parameter \"{0}\"!".format(parmName))
        
    return None

def RunEngine(args):
        print("Preparing to run the engine")
        
        print("Launch arguments:")
        for i, j in enumerate(args):
            print("[{0}]: \"{1}\"".format(i, j))

        print("Running the engine...")

        # Run the Engine
        p = Popen(largs, stdout=PIPE, stderr=PIPE)
        
        for line in iter(p.stdout.readline, b''):
            print(line.rstrip().decode("utf-8"))

if __name__ == "__main__":

    print("{0}".format(appName))
    print("Version: {0}".format(appVer))
    print("Description:\n{0}\n".format(appDesc))

    mapFile = Path(GetParameterValue("-map", False)) # Expecting the first parameter after the script to be the map file.
    engine = Path(GetParameterValue("-engine", True)) # We need the path to the engine
    baseFolder = GetParameterValue("-baseFolder") if DoesParameterExist("-baseFolder") else "assets" # the base folder for storing assets - can be game folder or generic engine assets

    workingDir = os.path.dirname(engine)

    if (mapFile != None):
        print("Map file defined, exporting before launch...")
        import ExportMap

        if not (ExportMap.ExportMap(workingDir, mapFile, baseFolder)):
            print("Error while exporting map! Exiting...")
            sys.exit(-1)

    # For the engine, so it doesn't crash.
    os.chdir(workingDir)

    if (mapFile != None):
        # Get the map name in a way the engine can use it
        mapName = mapFile.name.replace(mapFile.suffix, "")

        largs = [engine, '-map', mapName, '-dev']

        RunEngine(largs)
    else:
        largs = [engine, '-dev']

        RunEngine(largs)


