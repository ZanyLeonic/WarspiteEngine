#include "GameObjectDictionary.h"
#include <iostream>
#include <spdlog/spdlog.h>

CGameObjectDictionary* CGameObjectDictionary::s_pInstance = 0;

// Registers our GameObjectFactory
bool CGameObjectDictionary::RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator)
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

// Returns a GameObject that our GameObjectFactory created
std::unique_ptr<IGameObject> CGameObjectDictionary::Create(std::string typeID)
{
	auto it = m_creators.find(typeID);

	if (it == m_creators.end())
	{
		spdlog::warn("Cannot find Object of type: \"{}\"", typeID);
		return nullptr;
	}

	return (*it).second->Create();
}