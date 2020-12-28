#pragma once
#ifndef __IGAMEOBJDICT_H_
#define __IGAMEOBJDICT_H_

#include <string>
#include "EngineTypes.h"
#include "ObjectFactory.h"
#ifdef _GAME_
#include "WGame.h"
#endif

class IGameObject;

class IGameObjectDictionary
{
public:
	virtual bool RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator) = 0;
	virtual std::unique_ptr<IGameObject> Create(std::string typeID) = 0;
};

// For real - what will create our GameObject.
// A template class to know what we are creating.
template <class T>
class CGameObjectFactory : public IObjectFactory<IGameObject>
{
public:
	// When the class is created - take the respective map reference string
	CGameObjectFactory(std::string pMapRef)
	{
#ifdef _ENGINE_
		// Register ourselves in the dictionary.
		CGameObjectDictionary::Instance()->RegisterType(pMapRef, this);
#else _GAME_
		IGameObjectDictionary* obj = (IGameObjectDictionary*)CGame::Instance()->GetPointers()[ESingletonIDs::OBJDICT];

		obj->RegisterType(pMapRef, this);
#endif
	}

	// Simply returns the type our template is.
	std::unique_ptr<IGameObject> Create()
	{
		return std::make_unique<T>();
	}

	// Returns the size of our template
	virtual size_t GetObjectSize()
	{
		return sizeof(T);
	}
};

// Macro to make registering classes easier
#define REG_OBJ_TO_REF(mapRefName, compiledClassName) \
	static CGameObjectFactory<compiledClassName> mapRefName( #mapRefName );

#endif