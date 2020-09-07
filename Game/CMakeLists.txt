# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/assets/bin" )
set(WarspiteGame_Src
    src/WGame.cpp
    src/main.cpp
)
set(WarspiteGame_Header
    src/WGame.h
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_target(GameDLLMetadata
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/src/GameDLLMetadata.py"
        COMMENT "Generating build metadata..."
)

find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

find_library(GAME_SDK discord_game_sdk)
find_library(GAME_SDK_CPP discord_game_sdk_cpp)
find_path(GAME_SDK_INCLUDE discord-game-sdk/discord.h)

message(STATUS "Found GAME_SDK: ${GAME_SDK}")
message(STATUS "Found GAME_SDK_CPP: ${GAME_SDK_CPP}")
message(STATUS "Found GAME_SDK_INCLUDE: ${GAME_SDK_INCLUDE}")

set (WarspiteGame_Libs
    ${SPDLOG_LIBRARY}
    ${PYBIND11_LIBRARIES}
    fmt::fmt
    ${GAME_SDK}
    ${GAME_SDK_CPP}
)

include_directories("${PROJECT_SOURCE_DIR}/engine")
include_directories(${SHARED_INCLUDE_DIR})
include_directories(${SPDLOG_INCLUDE_DIR})
include_directories(${PYBIND11_INCLUDE_DIRS})
include_directories(${GAME_SDK_INCLUDE})

include (GenerateExportHeader)

add_library(${GAME_NAME} SHARED ${WarspiteGame_Src} ${WarspiteGame_Header})
add_dependencies(${GAME_NAME} GameDLLMetadata)

# As a last ditch effort...
if ("${Python_EXECUTABLE}" STREQUAL "")
    if (WIN32)
        set(Python_EXECUTABLE "python")
    else()
        set(Python_EXECUTABLE "python3")
    endif()
endif()

# Copy assets for debug
add_custom_command(TARGET ${GAME_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${BIN_PATH_DEBUG}/assets/")

target_link_libraries(${GAME_NAME} PUBLIC ${WarspiteGame_Libs})

set_target_properties(${GAME_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_DEBUG}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(-DENV64BIT)
else()
    add_definitions(-DENV32BIT)
endif()

if (UNIX)
    install(FILES ${CMAKE_BINARY_DIR}/Game/lib${GAME_NAME}.so DESTINATION ./bin/${PROJECT_NAME}/assets/bin)
    install (DIRECTORY assets DESTINATION ./bin/${PROJECT_NAME})
elseif (WIN32)
    install(TARGETS ${GAME_NAME} RUNTIME DESTINATION ./assets/bin)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./assets/bin
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
    install (DIRECTORY assets DESTINATION ./)
endif()