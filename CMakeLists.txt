# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Main project name
project ("Warspite")

# GCC will default to C11 if we don't do this and everything will crash and burn
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Our custom find CMakeFiles for dependencies 
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# Apply some debug options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(WARDEBUG)
    add_compile_definitions(USE_CONSOLE)
endif()

# For Linux and Windows specific code
if (WIN32)
    add_compile_definitions(_WIN32)
elseif(UNIX)
    add_compile_definitions(_UNIX)
endif()

# Mod name (for the Engine)
set (MOD_NAME "WarspiteEngine")
set (GAME_NAME "WarspiteGame")

# Window sizes and stuff
add_compile_definitions(DEF_WIDTH=640)
add_compile_definitions(DEF_HEIGHT=480)

# So things like the launcher know what the library is going to be called
add_compile_definitions(MOD_NAME="${MOD_NAME}")
add_compile_definitions(GAME_NAME="${GAME_NAME}")

# Go away M$
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

set (SHARED_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/shared/src")

# Build path for debug
set(BIN_PATH_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(BIN_PATH_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Store stuff when compiled here!
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/bin") 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BIN_PATH_RELEASE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BIN_PATH_RELEASE}/bin") 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_PATH_RELEASE}")

# Include sub-projects.
add_subdirectory ("Engine")
add_subdirectory ("Game")
add_subdirectory ("Launcher")