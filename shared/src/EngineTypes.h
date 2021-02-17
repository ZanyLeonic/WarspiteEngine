#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>
#include <map>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>
#include "Vector2D.h"

struct SInputObject;
struct SGameObject;
struct SLevelObject;
class IGame;
class IWGame;
class IGameObject;

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

enum class ECollisionResult
{
	NONE = 0,
	COLLIDED = 1,
	OVERLAP = 2
};

enum class EDirection
{
	NONE = 0,
	NORTH = 1,
	EAST = 2,
	SOUTH = 3,
	WEST = 4
};

static CVector2D GetForwardVector(EDirection dir)
{
	switch(dir)
	{
	case EDirection::NORTH:
		return CVector2D(0, -1);
	case EDirection::EAST:
		return CVector2D(1, 0);
	case EDirection::SOUTH:
		return CVector2D(0, 1);
	case EDirection::WEST:
		return CVector2D(-1, 0);
	default:
		return CVector2D();
	}
}

static std::string GetDirectionName(EDirection dir)
{
	switch (dir)
	{
	case EDirection::NORTH:
		return "North";
	case EDirection::EAST:
		return "East";
	case EDirection::SOUTH:
		return "South";
	case EDirection::WEST:
		return "West";
	default:
		return "Unknown";
	}
}

struct SCollisionData
{
	IGameObject* m_otherObject = 0;
	ECollisionResult m_result = ECollisionResult::NONE;
	CVector2D m_location;
};

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);
#endif // #ifndef __ENGINETYPES_H__