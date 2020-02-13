#include "GameObjectFactory.h"

GameObjectFactory* GameObjectFactory::s_pInstance = 0;

bool GameObjectFactory::RegisterType(std::string typeID, BaseCreator* pCreator)
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

GameObject* GameObjectFactory::Create(std::string typeID)
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
