#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>
#include <map>

struct SInputObject;
struct SGameObject;
struct SLevelObject;
class IGame;
class IWGame;

typedef std::function<void()> KeyCallback;
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
	SOUNDMGR = 6
};

typedef IWGame* (*GameDLL_t)(int argc, char** argv, std::map<ESingletonIDs, void(*)>* pPtrs);
#endif // #ifndef __ENGINETYPES_H__