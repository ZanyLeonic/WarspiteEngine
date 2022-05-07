# Warspite Engine

## Overview

A game engine that has been built using SDL2 and C++.

Built for the purpose of learning C++ and SDL2 as well as for my project for A-Level Computer Science.

| Azure Pipelines builds                                                                                                                                                                                                             | Codefactor                                                                                                                                                          |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [![Build Status](https://poweredbykurisu.visualstudio.com/WarspiteGame/_apis/build/status/WarspiteGame?branchName=develop)](https://poweredbykurisu.visualstudio.com/WarspiteGame/_build/latest?definitionId=1&branchName=develop) | [![CodeFactor](https://www.codefactor.io/repository/github/zanyleonic/warspiteengine/badge)](https://www.codefactor.io/repository/github/zanyleonic/warspiteengine) |

# Binaries

Binaries can be found under the [releases](https://github.com/ZanyLeonic/WarspiteEngine/releases).

## Engine Tools

The source code for the engine's tools can be found [here](https://github.com/ZanyLeonic/WarspiteEngine.AuthoringTools).

# How to build

## Windows

Dependencies:

- CMake 3.8 (or higher)
- Visual Studio 2019 (or higher)
  - Desktop Development C++ must be installed when installing

1. Run `Set-ExecutionPolicy RemoteSigned` in an elevated Powershell prompt to allow execution of the build script
2. Run `build-deps.ps1`, which use Vcpkg to install and build all required dependencies
3. In a command prompt window, navigate to `build` and run `cmake --build . --config <Debug/Release> -j4` (Replace <Debug/Release> with Debug or Release as required.)
4. Run `ws.exe` found in the Debug or Release in the build directory.

## Linux

Dependencies:

- CMake 3.8 (or higher)
- build-essential (or equivalent on your distribution)

1. Run `build-deps.sh` to install build dependencies and to create the build directory.
2. Run `build.sh` to build the engine. (Change the target to the desired in build.sh)
3. Run `ws` in the Debug or Release in the build directory.
