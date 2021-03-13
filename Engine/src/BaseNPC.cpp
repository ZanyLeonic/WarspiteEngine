#include "BaseNPC.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Game.h"
#include <spdlog/spdlog.h>

CBaseNPC::CBaseNPC()
{
	m_bCancelMovementOnOverlap = false;
	m_bCollides = true;
	m_bOverlap = false;
}

void CBaseNPC::Load(CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	m_sWorldTextureID = pParams->GetProperty<std::string>("worldTexture");

	m_iWidth = 32;
	m_iHeight = 32;
	m_iCurrentFrame = 1;
	m_iNumFrames = 4;
	m_iCurrentRow = 1;
}

void CBaseNPC::OnPlay()
{
	spdlog::info("[{}] NPC spawned into the world!", GetName());
}

void CBaseNPC::Destroy()
{
}

bool CBaseNPC::OnThink()
{
	m_iCurrentRow = GetRowFromDirection();

	return true;
}

void CBaseNPC::Draw()
{
	if (bDrawObject)
	{
		CVector2D cPos = CCamera::Instance()->GetPositionT();

		CTextureManager::Instance()->DrawFrame(m_sWorldTextureID,
			int(m_vPosition.GetX() - cPos.GetX()), int(m_vPosition.GetY() - cPos.GetY()),
			m_iWidth, m_iHeight, m_iCurrentRow, m_iCurrentFrame,
			CBaseGame::Instance()->GetRenderer());
	}

	CTileObject::Draw();
}

bool CBaseNPC::InteractAction(IGameObject* pOther)
{
	CVector2D newPos = pOther->GetPosition() - GetPosition();

	newPos /= 32;

	m_eObjectDirection = GetDirectionFromVector(newPos);

	return true;
}

int CBaseNPC::GetRowFromDirection()
{
	switch (m_eObjectDirection)
	{
	case EDirection::NORTH:
		return 4;
	case EDirection::EAST:
		return 3;
	case EDirection::SOUTH:
		return 1;
	case EDirection::WEST:
		return 2;
	default:
		return 1;
	}
}