# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (Launcher_Src
    main.cpp
)

set (Launcher_Header)

include (GenerateExportHeader)
add_library (Launcher SHARED ${Launcher_Src} ${Launcher_Header})