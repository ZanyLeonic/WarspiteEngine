# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

if (WIN32)
set (Launcher_Src
    main.cpp
)
elseif(UNIX)
set (Launcher_Src
    main_linux.cpp
)
endif()

set (Launcher_Header)

include (GenerateExportHeader)
add_library (Launcher SHARED ${Launcher_Src} ${Launcher_Header})
set_target_properties(Launcher PROPERTIES OUTPUT_NAME "launcher")

install(TARGETS Launcher RUNTIME DESTINATION ./bin)

if (WIN32)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./${PROJECT_NAME}/bin
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
endif()