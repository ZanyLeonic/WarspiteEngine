﻿# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(WarspiteEngine_Src
	AnimatedGraphic.cpp
	Button.cpp
	Enemy.cpp
    GameObjectFactory.cpp
    Game.cpp
    GameStateBase.cpp
    GameStateManager.cpp
    InputHandler.cpp
    main.cpp
    ObjectParams.h
    PauseState.cpp
    Player.cpp
    PlayState.cpp
    SDLGameObject.cpp
    StateParser.cpp
    TestObject.cpp
    TextureManager.cpp
    MainMenuState.cpp
)

set(WarspiteEngine_Header
	AnimatedGraphic.h
    BaseCreator.h
    Button.h
    Enemy.h
    GameObject.h
    GameObjectFactory.h
    Game.h
    GameState.h
    GameStateBase.h
    GameStateManager.h
    InputHandler.h
    PauseState.h
    Player.h
    PlayState.h
    SDLGameObject.h
    StateParser.h
    TestObject.h
    TextureManager.h
    MainMenuState.h
	MenuState.h
    Vector2D.h
)

if (WIN32)
    # Workaround for RelWithDebInfo builds not installing all the libraries
    set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL Release)
    set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

    set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
	set(SDL2IMAGE_LIBRARIES SDL2::SDL2_image)
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(rapidjson CONFIG REQUIRED)


add_custom_target(projMetadata 
	COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/GitVersion.py"
	COMMENT "Generating build metadata..."
)

# Add this because apparently find_package(SDL2 REQUIRED) doesn't work right on Arch?
if("${SDL2_LIBRARIES}" STREQUAL "")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

if("${SDL2IMAGE_LIBRARIES}" STREQUAL "")
    set(SDL2IMAGE_LIBRARIES "SDL2::SDL2_image")
endif()

set (Warspite_Libs
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2IMAGE_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${WarspiteEngine_Src} ${WarspiteEngine_Header})
add_dependencies(${PROJECT_NAME} projMetadata)

# As a last ditch effort...
if ("${Python_EXECUTABLE}" STREQUAL "")
	if (WIN32)
		set(Python_EXECUTABLE "python")
	else()
		set(Python_EXECUTABLE "python3")
	endif()
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${Warspite_Libs})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions(-DENV64BIT)
else()
    add_definitions(-DENV32BIT)
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ./Warspite)

if (WIN32)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
        DESTINATION ./Warspite
        FILES_MATCHING PATTERN "*.dll"
        PATTERN ".cmake" EXCLUDE
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "include" EXCLUDE
        PATTERN "Testing" EXCLUDE
    )
endif()

install (DIRECTORY assets DESTINATION ./Warspite)