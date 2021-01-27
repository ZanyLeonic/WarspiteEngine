#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>
#include <map>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>

struct SInputObject;
struct SGameObject;
struct SLevelObject;
class IGame;
class IWGame;

typedef std::function<void(SDL_Scancode)> HKeyCallback;
typedef std::function<void(SDL_Event)> HInputCallback;
typedef std::function<bool()> HButtonCallback;

typedef std::shared_ptr<SGameObject>  PGamePtr;
typedef std::shared_ptr<SInputObject> PInputPtr;
typedef std::shared_ptr<SLevelObject>  PLevelPtr;

enum class ESingletonIDs
{
	NONE = 0,
	GAME = 1,
	OBJDICT = 2,
	STATEDICT = 3,
	STATEMGR = 4,
	SCRIPTMGR = 5,
	SOUNDMGR = 6,
	CAMERA = 7,
	TEXTUREMANAGER = 8
};

// Simply a way of not exposing the whole of SDL to the GameDLL
enum class EWarRendererFlip
{
 	FLIP_NONE = 0x00000000,     /**< Do not flip */
    FLIP_HORIZONTAL = 0x00000001,    /**< flip horizontally */
    FLIP_VERTICAL = 0x00000002     /**< flip vertically */
};

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);
#endif // #ifndef __ENGINETYPES_H__