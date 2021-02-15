#include "PlayerSpawn.h"
#include "Game.h"

CPlayerSpawn::CPlayerSpawn()
	: CTileObject()
{
	// Setup this object
	m_overlap = false;
	m_bCancelMovementOnOverlap = false;
	m_collidable = false;
}

void CPlayerSpawn::Load(const CObjectParams* pParams)
{
	m_playerSpawnParam = const_cast<CObjectParams*>(pParams);
	
	if (m_playerSpawnParam != 0)
	{
		m_playerSpawnParam->SetFactoryID("Player");
		m_playerSpawnParam->SetTextureID("Player");
		m_playerSpawnParam->SetHeight(32);
		m_playerSpawnParam->SetWidth(32);
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