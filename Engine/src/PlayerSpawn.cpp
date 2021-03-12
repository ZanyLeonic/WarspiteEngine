#include "PlayerSpawn.h"
#include "Game.h"

CPlayerSpawn::CPlayerSpawn()
	: CTileObject()
{
	// Setup this object
	m_bOverlap = false;
	m_bCancelMovementOnOverlap = false;
	m_bCollides = false;
}

void CPlayerSpawn::Load(CObjectParams* pParams)
{	
	m_playerSpawnParam = pParams;

	if (m_playerSpawnParam != 0)
	{
		pParams->SetFactoryID("Player");
		pParams->SetProperty("textureID", std::string("Player"));
		pParams->SetProperty("textureHeight", 32);
		pParams->SetProperty("textureWidth", 32);
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