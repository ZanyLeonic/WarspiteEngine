#!/usr/bin/env python3
import os, sys, json
from pathlib import Path
from shutil import copyfile

try:
    workingDir = Path(sys.argv[2])
except IndexError:
    workingDir = Path(os.getcwd())

mapFile = Path(sys.argv[1]) # expecting the first parameter after the script to be the map file.
baseFolder = "assets" # the base folder for storing assets - can be game folder or generic engine assets

print("Processing map %s..." % mapFile.name)

# A few checks
if not (os.path.isfile(mapFile)):
    print("Cannot find map file \"%s\"!" % mapFile)

if not (os.path.exists(workingDir)):
    print("Cannot find path \"%s\"!" % workingDir)

if not (os.path.exists(workingDir.joinpath(baseFolder))):
    print("Cannot find base folder \"%s\"!" % workingDir.joinpath(baseFolder))

mapData = dict()
try:
    mf = open(mapFile, "r")
except OSError as e:
    print("Failed to read mapFile!")
    print("Error:\n%s" % str(e))

    sys.exit(-1)
else:
    with mf:
        mapData = json.load(mf)

print("Successfully read mapFile \"%s\"!" % mapFile)

for j, i in enumerate(mapData["tilesets"]):
    cTileset = dict()
    pTileset = mapFile
    npTileset = ""

    # Check if we are dealing with an external tileset file
    if "source" in i:
        try:
            # Set the path of the tileset
            pTileset = Path(os.path.join(mapFile.parent, i["source"]))
            # Usually relative to the mapFile
            fs = open(pTileset, "r")
        except OSError as e:
            print("Failed to read tileset!")
            print("Error:\n%s" % str(e))

            sys.exit(-1)
        else:
            with fs:
                cTileset = json.load(fs)
    else:
        # It's an embedded tileset
        cTileset = i
    
    npTileset = Path(os.path.join(workingDir, baseFolder, "tilesets", pTileset.name))

    # Usually relative to the tileset
    iTileSet = Path(os.path.join(pTileset.parent, cTileset["image"]))
    dTileSet = Path(os.path.join(workingDir, baseFolder, "textures", iTileSet.name))

    if not (os.path.exists(dTileSet.parent)):
        os.mkdir(dTileSet.parent)

    copyfile(iTileSet, dTileSet)
    
    cTileset["image"] = str(dTileSet.relative_to(workingDir))

    # If we are dealing with an external file...
    if not (pTileset == mapFile):
        try:
            ws = open(npTileset, "w")
        except OSError as e:
            print("Cannot write tileset!")
            print("Error:\n%s" % str(e))

            sys.exit(-1)
        else:
            with ws:
                json.dump(cTileset, ws)
        
        mapData["tilesets"][j]["source"] = str(npTileset.relative_to(workingDir))
    else:
        mapData["tilesets"][j] = cTileset

print("Copied tilesets!")

# Does the map have custom properties?
if ("properties" in mapData):
    for i, j in enumerate(mapData["properties"]):
        if(j["type"] == "file"):
            propFile=Path(mapFile.parent, j["value"])
            nPropFile=Path(workingDir, baseFolder, propFile.name)
            if (propFile.resolve() != nPropFile.resolve()):
                copyfile(propFile, nPropFile)

            mapData["properties"][i]["value"] = str(nPropFile.relative_to(workingDir))
print ("Checked property values")

nMapFile = Path(os.path.join(workingDir, baseFolder, "maps", mapFile.name))
try:
    jw = open(nMapFile, "w")
except OSError as e:
    print("An error occurred while writing \"%s\"!" % str(nMapFile.absolute()))
    print("Error:\n%s" % str(e))    
else:
    with jw:
        json.dump(mapData, jw)

print("Rewrote map files!")
print("Moved \"%s\" to \"%s\"" % (str(nMapFile.name), str(nMapFile.parent)))




    
