#pragma once
#ifndef __IGAMEOBJFACT_H_
#define __IGAMEOBJFACT_H_

#include "ObjectFactory.h"
#ifdef _ENGINE_
#include "GameObjectDictionary.h"
#elif _GAME_
#include "ObjectRegisterBuffer.h"
#endif

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
#elif _GAME_
		// TODO: Find a way to load the GameDLL when the pointers are populated.
		CObjectRegisterBuffer::Instance()->RegisterType(pMapRef, this);
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