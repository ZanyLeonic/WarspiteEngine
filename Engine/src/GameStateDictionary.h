#pragma once
#ifndef __GAMESTATEDICT_H__
#define __GAMESTATEDICT_H__

#include "ObjectFactory.h"
#include "IGameStateDictionary.h"
#include "GameStateBase.h"
#include <string>
#include <map>

// I know it is clone of the GameObject version, but I need to get it working first.
class CGameStateDictionary : public IGameStateDictionary
{
	CGameStateDictionary() {}
	static CGameStateDictionary* s_pInstance;

public:
	static CGameStateDictionary* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CGameStateDictionary();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator);
	std::unique_ptr<CGameStateBase> Create(std::string stateID);
private:
	std::map<std::string, IObjectFactory<CGameStateBase>*> m_creators;
};
#endif