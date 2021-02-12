#!/usr/bin/env python3

import sys
import os
import pathlib
import winreg
import json

appName = "WarspiteEngine SDK: Setup Tiled Project"
appVer = "1.1.0.0"
appDesc = """Generates run configurations for Tiled."""

# {0} is always the script
exportArgs = '\"{0}\" -map %mapfile -baseFolder \"assets\"'
runMapEngineArgs = '\"{0}\" -engine \"{1}\" -map %mapfile -baseFolder \"assets\"' 
runMapArgs = '\"{0}\" -engine \"{1}\" -baseFolder \"assets\"'

projectJSON = """{
  "automappingRulesFile": "",
  "commands": [
    {
      "arguments": "",
      "command": "",
      "enabled": true,
      "name": "Export Map",
      "saveBeforeExecute": true,
      "shortcut": "F8",
      "showOutput": true,
      "workingDirectory": ""
    },
    {
      "arguments": "",
      "command": "",
      "enabled": true,
      "name": "Run Engine (with Map)",
      "saveBeforeExecute": true,
      "shortcut": "F5",
      "showOutput": true,
      "workingDirectory": ""
    },
    {
      "arguments": "",
      "command": "",
      "enabled": true,
      "name": "Run Engine",
      "saveBeforeExecute": true,
      "shortcut": "F9",
      "showOutput": true,
      "workingDirectory": ""
    }
  ],
  "extensionsPath": "ext",
  "folders": [
    "."
  ],
  "objectTypesFile": ""
}"""

if __name__ == "__main__":

    print("{0}".format(appName))
    print("Version: {0}".format(appVer))
    print("Description:\n  {0}\n".format(appDesc))

    # Update your Python
    if (not sys.version.startswith('3')):
        print("You need to have at least Python 3 to run this script.")   
        sys.exit(-1)

    # Sorry Linux and MacOS users.
    if (sys.platform != "win32"):
        print("This script can only be ran under Win32.")
        sys.exit(-1)

    print("Locating Python...", end="")
    # Get our python install
    pythonExecutable = "\"{0}\"".format(sys.executable)
    print("done!")

    print("Finding Warspite...", end="")
    # Grab our Warspite install from the installer
    access_registry = winreg.ConnectRegistry(None, winreg.HKEY_CURRENT_USER)
    aKey = winreg.OpenKey(access_registry,r"Software\Warspite")

    warspiteInst = winreg.QueryValue(aKey, "")
    warspiteExecutable = os.path.join(warspiteInst, "ws.exe")

    print("done!")

    exportScript = os.path.join(warspiteInst, "sdk\\tools\\ExportMap.py")
    runScript = os.path.join(warspiteInst, "sdk\\tools\\RunEngine.py")
    
    outPath = pathlib.Path(os.path.join(warspiteInst, "sdk\\WarspiteGame.tiled-project"))

    print("Writing project files...", end="")
    fTiled = json.loads(projectJSON)

    fTiled['objectTypesFile'] = str(pathlib.Path(os.path.join(warspiteInst, "sdk\\resource\\objects.json")))
    commands = fTiled['commands']

    commands[0]['arguments'] = exportArgs.format(exportScript)
    commands[1]['arguments'] = runMapEngineArgs.format(runScript, warspiteExecutable)
    commands[2]['arguments'] = runMapArgs.format(runScript, warspiteExecutable)

    for i in commands:
        i['command'] = pythonExecutable
        i['workingDirectory'] = warspiteInst

    try:
        jw = open(outPath, "w")
    except OSError as e:
        print("error!")
        print("An error occurred while writing \"%s\"!" % str(outPath.absolute()))
        print("Error:\n%s" % str(e))    
    else:
        with jw:
            json.dump(fTiled, jw, indent = 6)

print("done!")
print("")
print("Finished generating project files!")
print("")
print("If there are no errors displayed above, you can now load the tiled project to create maps.")
print("Located: {}".format(outPath))
print("")
input("Press [Enter] to close this window...")