#include "Trigger.h"
#include "Player.h"
#include "Game.h"
#include <spdlog/spdlog.h>
#include <memory>

CTrigger::CTrigger()
{
	m_bFireOnce = false; // for now
	m_bCancelMovementOnOverlap = true;
	m_collidable = false;
	m_overlap = true;
}

void CTrigger::Load(CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	m_sStartOverlap = pParams->GetProperty<std::string>("startOverlapFunc") != "" ? pParams->GetProperty<std::string>("startOverlapFunc") : "default";
	m_sEndOverlap = pParams->GetProperty<std::string>("endOverlapFunc") != "" ? pParams->GetProperty<std::string>("endOverlapFunc") : "default";

	m_hOverlapStartCallback = CBaseGame::Instance()->GetCallbackHandler()->GetCallback(m_sStartOverlap);
	m_hOverlapEndCallback = CBaseGame::Instance()->GetCallbackHandler()->GetCallback(m_sEndOverlap);
}

void CTrigger::OnPlay()
{
	CTileObject::OnPlay();

	if (!m_hOverlapStartCallback)
	{
		spdlog::warn("[{}] Trigger at ({}, {}) spawned with no valid start overlap associated callback!", 
			m_objectName, m_position.GetX(), m_position.GetY());
	}

	if (!m_hOverlapEndCallback)
	{
		spdlog::warn("[{}] Trigger at ({}, {}) spawned with no valid end overlap associated callback!",
			m_objectName, m_position.GetX(), m_position.GetY());
	}
}

void CTrigger::Destroy()
{
}

bool CTrigger::OnThink()
{
	return false;
}

void CTrigger::Draw()
{
}

void CTrigger::OnOverlapStart()
{
	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());

	if (pPlayer && m_hOverlapStartCallback)
	{
		m_hOverlapStartCallback();
	}
}

void CTrigger::OnOverlapEnd()
{
	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());

	if (pPlayer && m_hOverlapEndCallback)
	{
		m_hOverlapEndCallback();
	}
}