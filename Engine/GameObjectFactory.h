#pragma once
#ifndef __GameObjectFactory_H__
#define __GameObjectFactory_H__

#include <string>
#include "BaseCreator.h"

class GameObjectFactory
{
private:
	GameObjectFactory() {};
	static GameObjectFactory* s_pInstance;

public:
	static GameObjectFactory* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new GameObjectFactory();
			return s_pInstance;
		}
		return s_pInstance;
	}

	bool RegisterType(std::string typeID, BaseCreator* pCreator)
	{
		std::map<std::string, BaseCreator*>::iterator it =
			m_creators.find(typeID);

		// Delete if it already exists in the creator map.
		if (it != m_creators.end())
		{
			delete pCreator;
			return false;
		}
		m_creators[typeID] = pCreator;

		return true;
	}

	GameObject* Create(std::string typeID)
	{
		std::map<std::string, BaseCreator*>::iterator it =
			m_creators.find(typeID);

		if (it == m_creators.end())
		{
			std::cout << "Cannot find Object of type: " << typeID << "\n";
			return NULL;
		}

		BaseCreator* pCreator = (*it).second;
		return pCreator->CreateGameObject();
	}

private:
	std::map<std::string, BaseCreator*> m_creators;
};

#endif
