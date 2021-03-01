# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/bin" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_PATH_RELEASE}/bin" )

add_compile_definitions(_ENGINE_)

set(WarspiteEngine_Src
        src/AudioSource.cpp
        src/AnimatedGraphic.cpp
        src/Button.cpp
        src/Camera.cpp
        ../shared/src/CallbackHandler.cpp
        src/CTexture.cpp
        src/ConfigParser.cpp
        src/DoorLink.cpp
        src/FadeObject.cpp
        src/FPSCounter.cpp
        src/Enemy.cpp
        src/EngineFileSystem.cpp
        src/FontManager.cpp
        src/TextObject.cpp
        src/Trigger.cpp
        src/Game.cpp
        src/GameObjectDictionary.cpp
        ../shared/src/GameStateBase.cpp
        src/GameStateDictionary.cpp
        src/GameStateManager.cpp
		src/GarbageCollector.cpp
        src/InputHandler.cpp
        src/InputState.cpp
        src/InteractableObject.cpp
        src/Level.cpp
        src/LevelParser.cpp
        src/LogicAuto.cpp
        src/main.cpp
        src/MenuState.cpp
        src/MainMenuState.cpp
        src/MapObject.cpp
        src/ObjectLayer.cpp
        src/PauseState.cpp
        src/Player.cpp
        src/PlayerSpawn.cpp
        src/PlayState.cpp
        src/ScriptManager.cpp
        src/ScriptWrappers.cpp
        src/SoundManager.cpp
        src/StateParser.cpp
        src/TestObject.cpp
        src/TextureManager.cpp
        src/TileLayer.cpp
        src/TileObject.cpp
		../shared/src/UIBase.cpp
        ../shared/src/WarspiteObject.cpp
        src/WarspiteUtil.cpp
        )

set(WarspiteEngine_Header
        src/AudioSource.h
        src/AnimatedGraphic.h
        src/Button.h
        src/Camera.h
        ../shared/src/CallbackHandler.h
        src/CTexture.h
        src/ConfigParser.h
        src/DoorLink.h
        src/FadeObject.h
        src/FPSCounter.h
        src/Enemy.h
        src/EngineFileSystem.h
        ../shared/src/EngineTypes.h
        src/FontManager.h
        src/TextObject.h
        src/Trigger.h
        src/Game.h
        ../shared/src/GameObject.h
        src/GameObjectDictionary.h
        ../shared/src/GameState.h
        ../shared/src/GameStateBase.h
        src/GameStateDictionary.h
        src/GameStateManager.h
		src/GarbageCollector.h
        src/InputHandler.h
        src/InputState.h
        src/InteractableObject.h
        ../shared/src/Layer.h
        src/Level.h
        src/LevelParser.h
        src/LogicAuto.h
        src/MainMenuState.h
        src/MapObject.h
        src/MenuState.h
        src/ObjectLayer.h
        ../shared/src/ObjectFactory.h
        ../shared/src/ObjectParams.h
        src/PauseState.h
        src/Player.h
        src/PlayerSpawn.h
        src/PlayState.h
        src/ScriptManager.h
        src/ScriptWrappers.h
        src/SoundManager.h
        src/StateParser.h
        src/TestObject.h
        src/TextureManager.h
        src/TileLayer.h
        src/TileObject.h
        ../shared/src/Vector2D.h
		../shared/src/UIBase.h
        ../shared/src/WarspiteObject.h
        src/WarspiteUtil.h
        )

# Thirdparty code
set (WarspiteEngine_Src
        ${WarspiteEngine_Src}
        src/etc/Base64.cpp
        )

set (WarspiteEngine_Header
        ${WarspiteEngine_Header}
        src/etc/Base64.h
        )

if (WIN32)
    # Workaround for RelWithDebInfo builds not installing all the libraries
    set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    set(SDL2_IMAGE_LIBRARIES SDL2::SDL2_image)
    set(SDL2_TTF_LIBRARIES SDL2::SDL2_ttf)
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Git REQUIRED)

add_custom_target(EngineMetadata
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/src/EngineMetadata.py" "${GIT_EXECUTABLE}"
        COMMENT "Generating build Engine metadata..."
)

# Download Python
if (WIN32)
    message(STATUS "Downloading Python ${Python3_VERSION} (${TARGET_ARCH})...")
    execute_process(COMMAND
    "${Python3_EXECUTABLE}"
    "-m"
    "pip"
    "install" 
    "requests")
    execute_process(COMMAND 
        "${Python3_EXECUTABLE}" 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/DownloadPython.py"
        "${CMAKE_BINARY_DIR}" 
        "${Python3_VERSION}" 
        "${TARGET_ARCH}" 
        "${CMAKE_BUILD_TYPE}"
    )
    message(STATUS "Download finished!")
endif()

find_package(fmt CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Vorbis REQUIRED)
find_package(ZLIB REQUIRED)

if((Python3_FOUND) AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    add_compile_definitions(PDLLNAME="python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
elseif(Python3_FOUND)
    add_compile_definitions(PDLLNAME="python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}_d")
endif()

# Add this because apparently find_package(SDL2 REQUIRED) doesn't work right on Arch?
if("${SDL2_LIBRARIES}" STREQUAL "")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

if("${SDL2_IMAGE_LIBRARIES}" STREQUAL "")
    set(SDL2IMAGE_LIBRARIES "SDL2::SDL2_image")
endif()

set (Warspite_Libs
    ${Python3_LIBRARIES}
    ${PYBIND11_LIBRARIES}
    fmt::fmt
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SPDLOG_LIBRARY}
    ${OPENAL_LIBRARY}
    ${VORBIS_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

if(UNIX)
    set (Warspite_Libs
        ${Warspite_Libs}
        stdc++fs
    )
endif()

include_directories("${PROJECT_SOURCE_DIR}/Engine/src")
include_directories("${PROJECT_SOURCE_DIR}/Game/src")
include_directories(${SHARED_INCLUDE_DIR})
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${SPDLOG_INCLUDE_DIR})
include_directories(${PYBIND11_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIRS})
include_directories(${OGG_INCLUDE_DIR})
include_directories(${OPENAL_INCLUDE_DIRS})
include_directories(${VORBIS_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIRS})

# Linking errors if we don't do this.
link_directories(${Python3_LIBRARY_DIRS})

include (GenerateExportHeader)
# Add source to this project's executable.
add_library(${MOD_NAME} SHARED ${WarspiteEngine_Src} ${WarspiteEngine_Header})
add_dependencies(${MOD_NAME} EngineMetadata)

# As a last ditch effort...
if ("${Python_EXECUTABLE}" STREQUAL "")
    if (WIN32)
        set(Python_EXECUTABLE "python")
    else()
        set(Python_EXECUTABLE "python3")
    endif()
endif()

target_link_libraries(${MOD_NAME} PUBLIC ${Warspite_Libs})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")                   
    set_target_properties(${MOD_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_DEBUG}")
else()
    set_target_properties(${MOD_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_RELEASE}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(-DENV64BIT)
else()
    add_definitions(-DENV32BIT)
endif()

if (UNIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/lib${MOD_NAME}.so DESTINATION ./bin/${PROJECT_NAME}/bin)
    else()
        install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/lib${MOD_NAME}.so DESTINATION ./bin/${PROJECT_NAME}/bin)
    endif()
elseif (WIN32)
    install(TARGETS ${MOD_NAME} RUNTIME DESTINATION ./bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            DESTINATION ./bin
            FILES_MATCHING PATTERN "*.dll"
            PATTERN ".cmake" EXCLUDE
            PATTERN "CMakeFiles" EXCLUDE
            PATTERN "include" EXCLUDE
            PATTERN "Testing" EXCLUDE
            )
endif()