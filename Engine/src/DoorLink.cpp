#include "DoorLink.h"
#include "Game.h"
#include "WarspiteUtil.h"

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
