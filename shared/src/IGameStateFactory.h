#pragma once
#ifndef __ISTATEOBJFACT_H_
#define __ISTATEOBJFACT_H_

#include "ObjectFactory.h"
#ifdef _ENGINE_
#include "GameStateDictionary.h"
#elif _GAME_
#include "StateRegisterBuffer.h"
#endif

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
#elif _GAME_
		// TODO: Find a way to load the GameDLL when the pointers are populated.
		CStateRegisterBuffer::Instance()->RegisterType(pStateRef, this);
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