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
	CAMERA = 7
};

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);
#endif // #ifndef __ENGINETYPES_H__