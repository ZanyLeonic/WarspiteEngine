#include "DoorLink.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "WarspiteUtil.h"
#include "FadeObject.h"
#include "SoundManager.h"
#include "EngineFileSystem.h"
#include <memory>

CDoorLink::CDoorLink()
	: CTileObject()
{
	m_bCancelMovementOnOverlap = true;
	m_collidable = false;
	m_overlap = true;
}

void CDoorLink::Load(CObjectParams* pParams)
{
	CTileObject::Load(pParams);

	CBaseGame::Instance()->GetFadeObject()->AddCallback(m_objectName, std::bind(&CDoorLink::fadeCallback, this, std::placeholders::_1, std::placeholders::_2));

	m_sTargetDoorID = pParams->GetProperty<std::string>("targetDoorID");
	m_sWorldTextureID = pParams->GetProperty<std::string>("doorWorldTexture");

	m_width = 32;
	m_height = 32;
	m_currentFrame = 0;
	m_numFrames = 4;
	m_currentRow = 1;

	m_eExitDirection = EDirection::SOUTH;

	if (CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath("door1_move.wav", CEngineFileSystem::EPathType::SOUND), m_prWaveFile))
	{
		// alCall(alSource3f, m_prWaveFile.Source, AL_POSITION, m_position.GetX(), m_position.GetY(), 0.f);
	}
}

void CDoorLink::OnPlay()
{
	CTileObject::OnPlay();

	m_pTargetDoor = std::dynamic_pointer_cast<CDoorLink>(CWarspiteUtil::FindGameObject(CBaseGame::Instance()->GetLoadedLevel(), m_sTargetDoorID));
	
	if (m_pTargetDoor)
	{
		spdlog::debug("[{}] Found target door ID \"{}\"", m_objectName, m_sTargetDoorID);
	}
	else
	{
		spdlog::warn("[{}] Cannot find target door ID \"{}\"!", m_objectName, m_sTargetDoorID);
	}
}

void CDoorLink::Destroy()
{
	CBaseGame::Instance()->GetFadeObject()->RemoveCallback(m_objectName);
}

bool CDoorLink::OnThink()
{
	if (m_bDoDoorAnimation)
	{
		playDoorAnimation(m_eAnimationPlayback);
	}
	return true;
}

void CDoorLink::Draw()
{
	if (bDrawObject)
	{
		CVector2D cPos = CCamera::Instance()->GetPositionT();

		CTextureManager::Instance()->DrawFrame(m_sWorldTextureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			CBaseGame::Instance()->GetRenderer());
	}

	CTileObject::Draw();
}

void CDoorLink::OnOverlapStart()
{
	CTileObject::OnOverlapStart();

	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());
	
	// Set a flag on the player, so they do not get stuck in a loop of going back and forth
	if (!m_bTravelledTo && pPlayer)
	{
		pPlayer->DisablePlayerInput(true);
		m_bTravellingFrom = true;
		m_bMadePlayerMove = false;
		CSoundManager::Instance()->PlaySound(&m_prWaveFile);
		PlayDoorAnimation(EPlaybackDirection::FORWARD);
	}
}

void CDoorLink::OnOverlapEnd()
{
	CTileObject::OnOverlapEnd();

	// Reset the flag on the door the player has travelled to
	if (m_pTargetDoor->m_bTravelledTo)
	{
		m_pTargetDoor->SetTravelledTo(false);
		m_currentFrame = 0;
	}
}

void CDoorLink::PlayDoorAnimation(EPlaybackDirection direction)
{
	m_eAnimationPlayback = direction;

	switch (direction)
	{
	case EPlaybackDirection::FORWARD:
		m_iAnimationFrame = 0;
		m_bDoDoorAnimation = true;
		break;
	case EPlaybackDirection::BACKWARD:
		m_iAnimationFrame = 60;
		m_bDoDoorAnimation = true;
		break;
	}
}

void CDoorLink::fadeCallback(EPlaybackDirection direction, bool fadeFinished)
{
	std::shared_ptr<CPlayer> pPlayer = std::dynamic_pointer_cast<CPlayer>(CBaseGame::Instance()->GetPlayer());

	if (!pPlayer) return; // How?

	// Going into the door
	if (m_bTravellingFrom && fadeFinished && direction == EPlaybackDirection::FORWARD)
	{
		CVector2D vTarget = m_pTargetDoor->GetPosition();

		m_pTargetDoor->SetTravelledTo(true);
		pPlayer->SetNextLocation(vTarget, 95);
		m_bTravellingFrom = false;
		CBaseGame::Instance()->FadeOut();
		CSoundManager::Instance()->PlaySound(&m_prWaveFile);
		m_pTargetDoor->PlayDoorAnimation(EPlaybackDirection::BACKWARD);
	}
	// Coming out of the door
	else if (m_bTravelledTo && !fadeFinished && !m_bMadePlayerMove 
		&& direction == EPlaybackDirection::BACKWARD)
	{
		CVector2D vTarget = GetPosition();

		pPlayer->SetNextLocation(vTarget + (GetForwardVector(m_eExitDirection) * 32), 95, true);
		pPlayer->SetDirection(m_eExitDirection);
		pPlayer->DisablePlayerInput(false);

		m_bMadePlayerMove = true;
	}
}

void CDoorLink::movementEnd(CPlayer* pPlayer)
{

}

void CDoorLink::playDoorAnimation(EPlaybackDirection direction)
{
	switch (direction)
	{
	case EPlaybackDirection::FORWARD:
		
		if (m_iAnimationFrame + 5 > 60)
		{
			m_currentFrame = 3;
			m_bDoDoorAnimation = false;
			CBaseGame::Instance()->FadeIn();
		}
		else
		{
			m_iAnimationFrame += 5;
			m_currentFrame = (int)trunc((m_iAnimationFrame / 2 ) / 10);
		}
		break;
	case EPlaybackDirection::BACKWARD:

		if (m_iAnimationFrame - 5 < 0)
		{
			m_currentFrame = 0;
			m_bDoDoorAnimation = false;
		}
		else
		{
			m_iAnimationFrame -= 5;
			m_currentFrame = (int)trunc((m_iAnimationFrame / 2) / 10);
		}
		break;
	}
}