# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (LauncherMain_Src
    launcher_main.cpp
    launcher_main.rc
)

set (LauncherMain_Header
    resource.h
)

add_executable (LauncherMain ${LauncherMain_Src} ${LauncherMain_Header})