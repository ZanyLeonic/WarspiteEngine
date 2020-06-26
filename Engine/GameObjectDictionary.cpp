#include "GameObjectDictionary.h"

#include <iostream>

CGameObjectDictionary* CGameObjectDictionary::s_pInstance = 0;

bool CGameObjectDictionary::RegisterType(std::string typeID, IObjectFactory* pCreator)
{
	auto it = m_creators.find(typeID);

	// Delete if it already exists in the creator map.
	if (it != m_creators.end())
	{
		// delete pCreator;
		return false;
	}
	m_creators[typeID] = pCreator;

	return true;
}

IGameObject* CGameObjectDictionary::Create(std::string typeID)
{
	auto it = m_creators.find(typeID);

	if (it == m_creators.end())
	{
		std::cout << "Cannot find Object of type: " << typeID << "\n";
		return nullptr;
	}

	return (*it).second->Create();
}

