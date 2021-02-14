#include "PlayerSpawn.h"
#include "Game.h"

void CPlayerSpawn::Load(const CObjectParams* pParams)
{
	m_playerSpawnParam = const_cast<CObjectParams*>(pParams);
	m_collidable = false;
	
	if (m_playerSpawnParam != 0)
	{
		m_playerSpawnParam->SetFactoryID("Player");
		m_playerSpawnParam->SetTextureID("Player");
		m_playerSpawnParam->SetHeight(27);
		m_playerSpawnParam->SetWidth(22);
	}
	CTileObject::Load(pParams);
}

void CPlayerSpawn::OnPlay()
{
	if (m_playerSpawnParam != 0)
	{
		// Create the object that is defined
		std::shared_ptr<IGameObject> pGameObject = CGameObjectDictionary::Instance()
			->Create(m_playerSpawnParam->GetFactoryID());
		
		// Check if the pointer is valid
		if (pGameObject != 0)
		{
			// intialise the object with the data obtained.
			pGameObject->Load(m_playerSpawnParam);
			pGameObject->OnPlay();

			CBaseGame::Instance()->SetPlayer(pGameObject);
		}
	}
}