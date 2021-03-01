#include "Trigger.h"
#include "Player.h"
#include "Game.h"
#include <spdlog/spdlog.h>
#include <memory>

CTrigger::CTrigger()
	: CTileObject()
{
	m_bFireOnce = false; // for now
	m_bCancelMovementOnOverlap = false;
	m_collidable = false;
	m_overlap = true;
	m_bFiredStart = false;
	m_bFiredEnd = false;
}

void CTrigger::Load(CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	m_sStartOverlap = pParams->PropertyExists("startOverlapFunc") ? pParams->GetProperty<std::string>("startOverlapFunc") : "default";
	m_bFireOnce = pParams->PropertyExists("fireOnce") ? pParams->GetProperty<bool>("fireOnce") : false;
	m_sEndOverlap = pParams->PropertyExists("endOverlapFunc") ? pParams->GetProperty<std::string>("endOverlapFunc") : "default";

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
	return CTileObject::OnThink();
}

void CTrigger::Draw()
{
	CTileObject::Draw();
}

void CTrigger::OnOverlapStart()
{
	CTileObject::OnOverlapStart();

	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());

	if (pPlayer && m_hOverlapStartCallback && !m_bFiredStart)
	{
		m_hOverlapStartCallback();
		if (m_bFireOnce) m_bFiredStart = true;
	}
}

void CTrigger::OnOverlapEnd()
{
	CTileObject::OnOverlapEnd();

	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());

	if (pPlayer && m_hOverlapEndCallback && !m_bFiredEnd)
	{
		m_hOverlapEndCallback();
		if (m_bFireOnce) m_bFiredEnd = true;
	}
}