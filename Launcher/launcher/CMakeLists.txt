# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/bin" )

if (WIN32)
    set (Launcher_Src
        src/main.cpp
    )
elseif(UNIX)
    set (Launcher_Src
        src/main_linux.cpp
    )
endif()

set (Launcher_Header)

include (GenerateExportHeader)
add_library (Launcher SHARED ${Launcher_Src} ${Launcher_Header})
set_target_properties(Launcher PROPERTIES 
                        OUTPUT_NAME "launcher"
                        VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_DEBUG}"
)

if (UNIX)
    install(FILES ${CMAKE_BINARY_DIR}/Launcher/launcher/liblauncher.so DESTINATION ./bin/${PROJECT_NAME}/bin)
elseif (WIN32)
    install(TARGETS Launcher RUNTIME DESTINATION ./bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./bin
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
endif()