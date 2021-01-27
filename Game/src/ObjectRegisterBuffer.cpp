#include "ObjectRegisterBuffer.h"
#include "IGameObjectDictionary.h"
#include "EngineTypes.h"
#include "WGame.h"

CObjectRegisterBuffer* CObjectRegisterBuffer::s_pInstance = 0;

bool CObjectRegisterBuffer::RegisterType(std::string typeID, IObjectFactory<IGameObject>* pCreator)
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

bool CObjectRegisterBuffer::AddAllRegistered()
{
	IGameObjectDictionary* obj = (IGameObjectDictionary*)(CGame::Instance()->GetPointers()[ESingletonIDs::OBJDICT]);

	// Register the GameObjects with the game engine
	if (obj)
	{
		std::map<std::string, IObjectFactory<IGameObject>*>::iterator it;

		for (it = m_creators.begin(); it != m_creators.end(); it++)
		{
			obj->RegisterType(it->first, it->second);
		}
		return true;
	}
	return false;
}