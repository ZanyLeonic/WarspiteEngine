#pragma once
#ifndef __IGAMESTATEDICT_H_
#define __IGAMESTATEDICT_H_

#include <string>
#include <memory>
#include "EngineTypes.h"
#include "ObjectFactory.h"
#ifdef _GAME_
#include "WGame.h"
#endif

class CGameStateBase;

class IGameStateDictionary
{
public:
	virtual bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator) = 0;
	virtual std::unique_ptr<CGameStateBase> Create(std::string stateID) = 0;
};

template<class T>
class CGameStateFactory : public IObjectFactory<CGameStateBase>
{
public:
	// When the class is created - take the respective map reference string
	CGameStateFactory(std::string pStateRef)
	{
#ifdef _ENGINE_
		// Register ourselves in the dictionary.
		CGameStateDictionary::Instance()->RegisterType(pStateRef, this);
#else _GAME_
		IGameStateDictionary* obj = (IGameStateDictionary*)CGame::Instance()->GetPointers()[ESingletonIDs::STATEDICT];

		obj->RegisterType(pStateRef, this);
#endif
	}

	// Simply returns the type our template is.
	std::unique_ptr<CGameStateBase> Create()
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
#define REG_STATE_TO_REF(stateRefName, compiledClassName) \
	static CGameStateFactory<compiledClassName> stateRefName( #stateRefName );

#endif