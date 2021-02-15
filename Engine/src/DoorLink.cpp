#include "DoorLink.h"
#include "Game.h"
#include "Player.h"
#include "WarspiteUtil.h"
#include <memory>

CDoorLink::CDoorLink()
	: CTileObject()
{
	m_bCancelMovementOnOverlap = true;
}

void CDoorLink::Load(const CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	m_targetDoorID = pParams->GetDoorTargetID();
	m_collidable = false;
	m_overlap = true;
}

void CDoorLink::OnPlay()
{
	CTileObject::OnPlay();

	m_targetDoor = std::dynamic_pointer_cast<CDoorLink>(CWarspiteUtil::FindGameObject(CBaseGame::Instance()->GetLoadedLevel(), m_targetDoorID));
	
	if (m_targetDoor)
	{
		spdlog::debug("[{}] Found target door ID \"{}\"", m_objectName, m_targetDoorID);
	}
	else
	{
		spdlog::warn("[{}] Cannot find target door ID \"{}\"!", m_objectName, m_targetDoorID);
	}
}

void CDoorLink::OnOverlapStart()
{
	CTileObject::OnOverlapStart();

	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());
	
	if (!m_bTravelledTo && pPlayer)
	{
		m_targetDoor->SetTravelledTo(true);
		pPlayer->SetNextLocation(m_targetDoor->GetPosition());
	}
}

void CDoorLink::OnOverlapEnd()
{
	CTileObject::OnOverlapEnd();

	if (m_targetDoor->m_bTravelledTo)
	{
		m_targetDoor->SetTravelledTo(false);
	}
}