#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>

struct SInputObject;
struct SGameObject;
struct SLevelObject;

typedef std::function<void()> KeyCallback;
typedef std::function<bool()> HButtonCallback;

typedef std::shared_ptr<SGameObject>  PGamePtr;
typedef std::shared_ptr<SInputObject> PInputPtr;
typedef std::shared_ptr<SLevelObject>  PLevelPtr;

typedef bool (*GameDLL_t)(int argc, char** argv);

#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#endif // #ifndef __ENGINETYPES_H__