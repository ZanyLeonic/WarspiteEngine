# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

if (WIN32)
    set(LauncherMain_Libs)
    set (LauncherMain_Src
        launcher_main.cpp
        launcher_main.rc
    )

    set (LauncherMain_Header
        resource.h
    )

    add_executable (LauncherMain WIN32 ${LauncherMain_Src} ${LauncherMain_Header})
elseif(UNIX)
    set(LauncherMain_Libs dl)
    set (LauncherMain_Src
        launcher_main.cpp
    )

    set (LauncherMain_Header
    )
    add_executable (LauncherMain ${LauncherMain_Src} ${LauncherMain_Header})
endif()

# So these libraries will actually recompile whenever the Launcher is ran.
add_dependencies(LauncherMain ${MOD_NAME} ${GAME_NAME})

target_link_libraries(LauncherMain PUBLIC ${LauncherMain_Libs})
set_target_properties(LauncherMain PROPERTIES 
                        OUTPUT_NAME "ws"
                        VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_DEBUG}")

if (UNIX)
    install(TARGETS LauncherMain RUNTIME DESTINATION ./bin/${PROJECT_NAME})
elseif (WIN32)
    install(TARGETS LauncherMain RUNTIME DESTINATION ./)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
endif()