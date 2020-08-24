#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>

struct SInputObject;
struct SGameObject;
struct SLevelObject;
class IGame;

typedef std::function<void()> KeyCallback;
typedef std::function<bool()> HButtonCallback;

typedef std::shared_ptr<SGameObject>  PGamePtr;
typedef std::shared_ptr<SInputObject> PInputPtr;
typedef std::shared_ptr<SLevelObject>  PLevelPtr;

typedef bool (*GameDLL_t)(int argc, char** argv, IGame* pGame);

#endif // #ifndef __ENGINETYPES_H__