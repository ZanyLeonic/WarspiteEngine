# Warspite Engine

## Overview

A game engine that has been built using SDL2 and C++

| Windows (x64)                                                                                                                                                                                                              | Codefactor                                                                                                                                                                                                                  |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [![Build Status](https://poweredbykurisu.visualstudio.com/WarspiteGame/_apis/build/status/WarspiteGame?branchName=new)](https://poweredbykurisu.visualstudio.com/WarspiteGame/_build/latest?definitionId=1&branchName=new) | [![CodeFactor](https://www.codefactor.io/repository/github/zanyleonic/warspite/badge/develop?s=cef60d0c07739d46cd426e20adf7dd35945163d5)](https://www.codefactor.io/repository/github/zanyleonic/warspite/overview/develop) |

# Binaries

Binaries can be found under the [releases](https://github.com/ZanyLeonic/WarspiteEngine/releases).

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
