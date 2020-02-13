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

	bool RegisterType(std::string typeID, BaseCreator* pCreator);
	GameObject* Create(std::string typeID);
private:
	std::map<std::string, BaseCreator*> m_creators;
};

#endif
