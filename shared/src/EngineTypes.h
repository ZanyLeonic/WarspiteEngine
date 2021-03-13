#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>
#include <map>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_events.h>
#include "Vector2D.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

struct SInputObject;
struct SGameObject;
struct SLevelObject;

class IGame;
class IWGame;
class IGameObject;

typedef std::function<void(SDL_Scancode)> HKeyCallback;
typedef std::function<void(SDL_Event)> HInputCallback;
typedef std::function<bool()> HButtonCallback;
typedef std::function<void()> HGenericCallback;

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

enum class ECollisionChannel
{
	NONE = 0,
	CHANNEL_1 = 1, // 
	CHANNEL_2 = 2,
	CHANNEL_3 = 3,
	CHANNEL_4 = 4,
	CHANNEL_5 = 5,
	CHANNEL_6 = 6,
	CHANNEL_7 = 7,
	CHANNEL_8 = 8,
	CHANNEL_9 = 9,
};

enum class EDirection
{
	NONE = 0,
	NORTH = 1,
	EAST = 2,
	SOUTH = 3,
	WEST = 4
};

enum class EPlaybackDirection
{
	NONE = 0,
	FORWARD = 1,
	BACKWARD = -1
};
typedef std::function<void(EPlaybackDirection, bool)> HFadeComplete;

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

static EDirection GetDirectionFromVector(CVector2D pVec)
{
	float x = pVec.GetX();
	float y = pVec.GetY();

	if (x == 0 && y == -1) return EDirection::NORTH;
	else if (x == 1 && y == 0) return EDirection::EAST;
	else if (x == 0 && y == 1) return EDirection::SOUTH;
	else if (x == -1 && y == 0) return EDirection::WEST;
	else return EDirection::NONE;
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

enum class MapProperties
{
	PROP_INVALID,
	PROP_SCRIPT,
	PROP_TEXTUREID,
	PROP_TEXWIDTH,
	PROP_TEXHEIGHT,
	PROP_NUMFRAMES,
	PROP_ANIMSPEED,
	PROP_ONCLICKCALL,
	PROP_ONENTERCALL,
	PROP_ONLEAVECALL,
	PROP_SOUNDPATH,
	PROP_DOORTARGET,
	PROP_DOORWORLDTEXTURE,
	PROP_STARTOVERLAP,
	PROP_ENDOVERLAP
};
#endif // #ifndef __ENGINETYPES_H__