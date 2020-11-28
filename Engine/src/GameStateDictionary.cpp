#include "GameStateDictionary.h"
#include <iostream>
#include <spdlog/spdlog.h>

CGameStateDictionary* CGameStateDictionary::s_pInstance = 0;

// Registers our GameObjectFactory
bool CGameStateDictionary::RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator)
{
	auto it = m_creators.find(stateID);

	// Delete if it already exists in the creator map.
	if (it != m_creators.end())
	{
		// delete pCreator;
		return false;
	}
	m_creators[stateID] = pCreator;

	return true;
}

// Returns a GameObject that our GameObjectFactory created
std::shared_ptr<CGameStateBase> CGameStateDictionary::Create(std::string stateID)
{
	auto it = m_creators.find(stateID);

	if (it == m_creators.end())
	{
		spdlog::warn("Cannot find state of ID: \"{}\"", stateID);
		return nullptr;
	}

	return (*it).second->Create();
}