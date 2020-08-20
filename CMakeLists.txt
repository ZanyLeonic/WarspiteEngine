# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Warspite")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Apply some debug options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(WARDEBUG)
endif()

if (WIN32)
    add_compile_definitions(_WIN32)
elseif(UNIX)
    add_compile_definitions(_UNIX)
endif()

# Mod name (for the Engine)
set (MOD_NAME "WarspiteEngine")
add_compile_definitions(MOD_NAME="${MOD_NAME}")

# Include sub-projects.
add_subdirectory ("Engine")
add_subdirectory ("Launcher")
