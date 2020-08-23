# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_PATH_DEBUG}/bin" )

set(WarspiteEngine_Src
        AudioSource.cpp
        AnimatedGraphic.cpp
        Button.cpp
        Camera.cpp
        CTexture.cpp
        FPSCounter.cpp
        Enemy.cpp
        EngineFileSystem.cpp
        FontManager.cpp
        TextObject.cpp
        Game.cpp
        GameObjectDictionary.cpp
        GameStateBase.cpp
        GameStateDictionary.cpp
        GameStateManager.cpp
        InputHandler.cpp
        Level.cpp
        LevelParser.cpp
        LogicAuto.cpp
        main.cpp
        MainMenuState.cpp
        MapObject.cpp
        ObjectLayer.cpp
        PauseState.cpp
        Player.cpp
        PlayState.cpp
        ScriptManager.cpp
        ScriptWrappers.cpp
        SoundManager.cpp
        StateParser.cpp
        TestObject.cpp
        TextureManager.cpp
        TileLayer.cpp
        WarspiteObject.cpp
        WarspiteUtil.cpp
        )

set(WarspiteEngine_Header
        AudioSource.h
        AnimatedGraphic.h
        Button.h
        Camera.h
        CTexture.h
        FPSCounter.h
        Enemy.h
        EngineFileSystem.h
        ../shared/src/EngineTypes.h
        FontManager.h
        TextObject.h
        Game.h
        GameObject.h
        GameObjectDictionary.h
        GameState.h
        GameStateBase.h
        GameStateDictionary.h
        GameStateManager.h
        InputHandler.h
        Layer.h
        Level.h
        LevelParser.h
        LogicAuto.h
        MainMenuState.h
        MapObject.h
        MenuState.h
        ObjectLayer.h
        ObjectFactory.h
        ObjectParams.h
        PauseState.h
        Player.h
        PlayState.h
        ScriptManager.h
        ScriptWrappers.h
        SoundManager.h
        StateParser.h
        TestObject.h
        TextureManager.h
        TileLayer.h
        Vector2D.h
        WarspiteObject.h
        WarspiteUtil.h
        )

# Thirdparty code
set (WarspiteEngine_Src
        ${WarspiteEngine_Src}
        etc/Base64.cpp
        )

set (WarspiteEngine_Header
        ${WarspiteEngine_Header}
        etc/Base64.h
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

add_custom_target(EngineMetadata
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/EngineMetadata.py"
        COMMENT "Generating build Engine metadata..."
)

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
set_target_properties(${MOD_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${BIN_PATH_DEBUG}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(-DENV64BIT)
else()
    add_definitions(-DENV32BIT)
endif()

if (UNIX)
    install(FILES lib${MOD_NAME}.so DESTINATION ./bin/${PROJECT_NAME}/bin)
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