#pragma once
#ifndef __GAMESTATEDICT_H__
#define __GAMESTATEDICT_H__

#include "ObjectFactory.h"
#include <string>
#include <map>
#include "EngineTypes.h"

class CGameStateBase;

// I know it is clone of the GameObject version, but I need to get it working first.
class CGameStateDictionary
{
	CGameStateDictionary() {}
	
public:
	static CGameStateDictionary* Instance();

	bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator);
	CGameStateBase* Create(std::string stateID);
private:
	std::map<std::string, IObjectFactory<CGameStateBase>*> m_creators;
};

template<class T>
class CGameStateFactory : public IObjectFactory<CGameStateBase>
{
public:
	// When the class is created - take the respective map reference string
	CGameStateFactory(std::string pStateRef)
	{
		// Register ourselves in the dictionary.
		CGameStateDictionary::Instance()->RegisterType(pStateRef, this);
	}

	// Simply returns the type our template is.
	CGameStateBase* Create()
	{
		return new T;
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

EXPORT inline CGameStateDictionary* GetStateDict()
{
	return CGameStateDictionary::Instance();
}

#endif