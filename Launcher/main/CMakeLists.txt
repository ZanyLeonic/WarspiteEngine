# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)


if (WIN32)
    set (LauncherMain_Src
        launcher_main.cpp
        launcher_main.rc
    )

    set (LauncherMain_Header
        resource.h
    )

    add_executable (LauncherMain WIN32 ${LauncherMain_Src} ${LauncherMain_Header})
elseif(UNIX)
    set (LauncherMain_Src
        launcher_main.cpp
    )

    set (LauncherMain_Header
    )
    add_executable (LauncherMain ${LauncherMain_Src} ${LauncherMain_Header})
endif()

set_target_properties(LauncherMain PROPERTIES OUTPUT_NAME "ws")

install(TARGETS LauncherMain RUNTIME DESTINATION ./)

if (WIN32)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./${PROJECT_NAME}
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
endif()