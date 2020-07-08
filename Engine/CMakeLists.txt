# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(WarspiteEngine_Src
    AnimatedGraphic.cpp
    Button.cpp
    Camera.cpp
    Enemy.cpp
    Game.cpp
    GameObjectDictionary.cpp
    GameStateBase.cpp
    GameStateManager.cpp
    InputHandler.cpp
    Level.cpp
    LevelParser.cpp
    main.cpp
    MainMenuState.cpp
    MapObject.cpp
    ObjectLayer.cpp
    PauseState.cpp
    Player.cpp
    PlayState.cpp
    SoundManager.cpp
    StateParser.cpp
    TestObject.cpp
    TextureManager.cpp
    TileLayer.cpp
    WarspiteObject.cpp
)

set(WarspiteEngine_Header
    AnimatedGraphic.h
    Button.h
    Camera.h
    Enemy.h
    Game.h
    GameObject.h
    GameObjectDictionary.h
    GameState.h
    GameStateBase.h
    GameStateManager.h
    InputHandler.h
    Layer.h
    Level.h
    LevelParser.h
    MainMenuState.h
    MapObject.h
    MenuState.h
    ObjectLayer.h
    ObjectParams.h
    PauseState.h
    Player.h
    PlayState.h
    SoundManager.h
    StateParser.h
    TestObject.h
    TextureManager.h
    TileLayer.h
    Vector2D.h
    WarspiteObject.h
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

# Apply some debug options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(WARDEBUG)
endif()

find_package(Python3 COMPONENTS Interpreter REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(rapidjson REQUIRED)
find_package(OpenAL REQUIRED)
find_package(Vorbis REQUIRED)
find_package(ZLIB REQUIRED)

if (WIN32)
add_custom_target(ProjectMetadata
	COMMAND "py" "-3" "${CMAKE_CURRENT_SOURCE_DIR}/GitVersion.py"
	COMMENT "Generating build metadata..."
)
else()
add_custom_target(ProjectMetadata
	COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/GitVersion.py"
	COMMENT "Generating build metadata..."
)
endif()

# Add this because apparently find_package(SDL2 REQUIRED) doesn't work right on Arch?
if("${SDL2_LIBRARIES}" STREQUAL "")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

if("${SDL2_IMAGE_LIBRARIES}" STREQUAL "")
    set(SDL2IMAGE_LIBRARIES "SDL2::SDL2_image")
endif()

set (Warspite_Libs
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
	${OPENAL_LIBRARY}
    ${VORBIS_LIBRARIES}
	${ZLIB_LIBRARIES}
)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIRS})
include_directories(${OGG_INCLUDE_DIR})
include_directories(${OPENAL_INCLUDE_DIRS})
include_directories(${VORBIS_INCLUDE_DIR})
include_directories(${ZLIB_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${WarspiteEngine_Src} ${WarspiteEngine_Header})
add_dependencies(${PROJECT_NAME} ProjectMetadata)

# As a last ditch effort...
if ("${Python_EXECUTABLE}" STREQUAL "")
	if (WIN32)
		set(Python_EXECUTABLE "py -3")
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
