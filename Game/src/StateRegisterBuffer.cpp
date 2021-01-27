#include "StateRegisterBuffer.h"
#include "IGameStateDictionary.h"
#include "EngineTypes.h"
#include "WGame.h"

CStateRegisterBuffer* CStateRegisterBuffer::s_pInstance = 0;

bool CStateRegisterBuffer::RegisterType(std::string stateID, IObjectFactory<CGameStateBase>* pCreator)
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

bool CStateRegisterBuffer::AddAllRegistered()
{
	IGameStateDictionary* obj = (IGameStateDictionary*)CGame::Instance()->GetPointers()[ESingletonIDs::STATEDICT];

	// Register the GameObjects with the game engine
	if (obj)
	{
		std::map<std::string, IObjectFactory<CGameStateBase>*>::iterator it;

		for (it = m_creators.begin(); it != m_creators.end(); it++)
		{
			obj->RegisterType(it->first, it->second);
		}
		return true;
	}
	return false;
}