#include "Player.h"
#include "InputHandler.h"
#include "Camera.h"
#include "ObjectLayer.h"
#include "Game.h"
#include "PlayState.h"
#include "TextureManager.h"
#include "ScriptManager.h"
#include "SoundManager.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <spdlog/spdlog.h>

CPlayer::CPlayer()
	: CWarspiteObject()
{
	pUseSound = new SWaveFile();
	pDenyUseSound = new SWaveFile();
}

void CPlayer::OnPlay()
{
	// CCamera::Instance()->SetTarget(&m_vPosition);
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_E, std::bind(&CPlayer::InteractAction, this, std::placeholders::_1));

	// Input binding
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_UP, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_DOWN, 1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_LEFT, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_RIGHT, 1.f);

	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_W, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_S, 1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_A, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_D, 1.f);

	CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath("use.wav", CEngineFileSystem::EPathType::SOUND), *pUseSound);
	CSoundManager::Instance()->Load(CEngineFileSystem::ResolvePath("denyuse.wav", CEngineFileSystem::EPathType::SOUND), *pDenyUseSound);
}

void CPlayer::Load(CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);
}

bool CPlayer::OnThink()
{
	HandleInput();

	m_iCurrentFrame = 1;
	m_iCurrentRow = GetRowFromDirection();

	if (m_bMoving)
	{
		m_vPosition = VectorMath::Lerp(m_vLastPosition, m_vNextPosition, (m_fTimeLeft / 100));
		CCamera::Instance()->SetTarget(&m_vPosition);
		if (CSoundManager::Instance()->IsInitialised())
		{
			// alCall(alListener3f, AL_POSITION, m_vPosition.GetX(), m_vPosition.GetY(), 0.f);
		}

		DecideFrame();
	}

	std::cout << "Viewport position X: " << GetViewportPosition().GetX() << " Y: " << GetViewportPosition().GetY() << "       ";
	std::cout << "\r    ";

	CWarspiteObject::OnThink();

	return true;
}

void CPlayer::Draw()
{
	// Get the camera position to offset the drawing
	CVector2D cPos = CCamera::Instance()->GetPositionT();

	// Flip the sprite automatically if the velocity is negative.
	if (m_vVelocity.GetX() > 0)
	{
		CTextureManager::Instance()->DrawFrame(m_sTextureID,
			int(m_vPosition.GetX() - cPos.GetX()), int(m_vPosition.GetY() - cPos.GetY()),
			m_iWidth, m_iHeight, m_iCurrentRow, m_iCurrentFrame,
			CBaseGame::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		CTextureManager::Instance()->DrawFrame(m_sTextureID,
			int(m_vPosition.GetX() - cPos.GetX()), int(m_vPosition.GetY() - cPos.GetY()),
			m_iWidth, m_iHeight, m_iCurrentRow, m_iCurrentFrame,
			CBaseGame::Instance()->GetRenderer());
	}
}

void CPlayer::SetNextLocation(CVector2D nextLocation, float pos, bool callCallbacks)
{
	m_bMoving = true;
	m_vLastPosition = m_vPosition;
	m_vNextPosition = nextLocation;
	m_fTimeLeft = pos;
	m_bStepLastFrame = true;

	if (m_sLastCollision.m_otherObject && callCallbacks)
		if (m_sLastCollision.m_otherObject->IsOverlapping() && m_sLastCollision.m_otherObject->ShouldOverlap())
			m_sLastCollision.m_otherObject->OnOverlapEnd();
}

void CPlayer::AddMovementStartCallback(std::string id, HPlayerCallback call)
{
	m_vOnMoveStart[id] = call;
}

void CPlayer::RemoveMovementStartCallback(std::string id)
{
	m_vOnMoveStart.erase(id);
}

void CPlayer::AddMovementEndCallback(std::string id, HPlayerCallback call)
{
	m_vOnMoveEnd[id] = call;
}

void CPlayer::RemoveMovementEndCallback(std::string id)
{
	m_vOnMoveEnd.erase(id);
}

void CPlayer::HandleInput()
{
	if (m_fTimeLeft >= 100)
	{
		if (CInputHandler::Instance()->JoysticksInitialised() && !m_bDisablePlayerInput)
		{
			CVector2D tMove;

			if (CInputHandler::Instance()->GetXAxis(0, 1) > 0 ||
				CInputHandler::Instance()->GetXAxis(0, 1) < 0)
			{
				tMove.SetX(tMove.GetX() + (1 * CInputHandler::Instance()->GetXAxis(0, 1)));
			}
			if (CInputHandler::Instance()->GetYAxis(0, 1) > 0 ||
				CInputHandler::Instance()->GetYAxis(0, 1) < 0)
			{
				tMove.SetY(tMove.GetY() + (1 * CInputHandler::Instance()->GetYAxis(0, 1)));
			}
			if (CInputHandler::Instance()->GetXAxis(0, 2) > 0 ||
				CInputHandler::Instance()->GetXAxis(0, 2) < 0)
			{
				tMove.SetX(tMove.GetX() + (1 * CInputHandler::Instance()->GetXAxis(0, 2)));
			}
			if (CInputHandler::Instance()->GetYAxis(0, 2) > 0 ||
				CInputHandler::Instance()->GetYAxis(0, 2) < 0)
			{
				tMove.SetY(tMove.GetY() + (1 * CInputHandler::Instance()->GetYAxis(0, 2)));
			}

			MoveForward(tMove.GetY() + CInputHandler::Instance()->GetAxisValue("MoveForward"));
			MoveRight(tMove.GetX() + CInputHandler::Instance()->GetAxisValue("MoveRight"));
		}
		else if (!m_bDisablePlayerInput)
		{
			// Can this code be improved? (I hope so.)	
			MoveForward(CInputHandler::Instance()->GetAxisValue("MoveForward"));
			MoveRight(CInputHandler::Instance()->GetAxisValue("MoveRight"));
		}

		if (m_fTimeLeft >= 100 && m_bMoving)
		{
			m_bMoving = false;
			callEndCallbacks();
		}
	}
	else
	{
		m_fTimeLeft += 5;
	}
}

void CPlayer::InteractAction(SDL_Scancode key)
{
	CVector2D curPos = m_vPosition;
	int axis = 0;

	switch (m_eObjectDirection)
	{
	case EDirection::SOUTH:
		axis = 1;
		curPos.SetY(curPos.GetY() + (m_iMoveStep * axis));
		break;
	case EDirection::NORTH:
		axis = -1;
		curPos.SetY(curPos.GetY() + (m_iMoveStep * axis));
		break;
	case EDirection::EAST:
		axis = 1;
		curPos.SetX(curPos.GetX() + (m_iMoveStep * axis));
		break;
	case EDirection::WEST:
		axis = -1;
		curPos.SetX(curPos.GetX() + (m_iMoveStep * axis));
		break;
	}

	if (UseObject(&curPos))
	{
		CSoundManager::Instance()->PlaySound(pUseSound);
	}
	else
	{
		CSoundManager::Instance()->PlaySound(pDenyUseSound);
	}
}

bool CPlayer::UseObject(CVector2D* pNext)
{
	SCollisionData r = CBaseGame::Instance()->GetStateManager()->IsColliding(*pNext);

	if (m_bMoving) return false;
	if (r.m_result != ECollisionResult::COLLIDED) return false;
	if (!r.m_otherObject->CollidesOnChannel(ECollisionChannel::CHANNEL_1)) return false;

	return r.m_otherObject->InteractAction(this);
}

void CPlayer::MoveForward(float axis)
{
	if (axis == 0.f) return;
	CVector2D curPos = m_vPosition;
	
	m_iCurrentRow = (axis > 0) ? 1 : 4;
	m_eObjectDirection = (axis > 0) ? EDirection::SOUTH : EDirection::NORTH;

	// Analog movement coming never.
	curPos.SetY(curPos.GetY() + (m_iMoveStep * axis));

	HandleMovement(&curPos);
}

void CPlayer::MoveRight(float axis)
{
	if (axis == 0.f) return;
	CVector2D curPos = m_vPosition;
	
	m_iCurrentRow = (axis > 0) ? 3 : 2;
	m_eObjectDirection = (axis > 0) ? EDirection::EAST : EDirection::WEST;

	curPos.SetX(curPos.GetX() + (m_iMoveStep * axis));

	HandleMovement(&curPos);
}

void CPlayer::HandleMovement(CVector2D* pNext)
{
	IGameObject* lastObj = m_sLastCollision.m_otherObject;
	bool result = WillCollide(pNext);

	if (!result && m_sLastCollision.m_result == ECollisionResult::OVERLAP)
	{
		callStartCallbacks();

		if (m_sLastCollision.m_otherObject)
			if (!m_sLastCollision.m_otherObject->IsOverlapping()
				&& m_sLastCollision.m_otherObject->ShouldOverlap())
			{
				m_sLastCollision.m_otherObject->OnOverlapStart();
			}

		if (lastObj)
			if (lastObj->IsOverlapping() && lastObj->ShouldOverlap())
				lastObj->OnOverlapEnd();

		m_bMoving = true;
		m_vLastPosition = m_vPosition;
		m_vNextPosition = *pNext;
		m_fTimeLeft = 0;
		m_bStepLastFrame = true;
	}
	else if (!result)
	{
		callStartCallbacks();

		m_bMoving = true;
		m_vLastPosition = m_vPosition;
		m_vNextPosition = *pNext;
		m_fTimeLeft = 0;
		m_bStepLastFrame = true;

		if (lastObj)
			if (lastObj->IsOverlapping() && lastObj->ShouldOverlap())
				lastObj->OnOverlapEnd();
	}
	else
	{
		spdlog::info("Cannot move {} - collided!", GetDirectionName(m_eObjectDirection));
	}
}

bool CPlayer::WillCollide(CVector2D* pNext)
{
	SCollisionData r = CBaseGame::Instance()->GetStateManager()->IsColliding(*pNext);
	m_sLastCollision = r;

	// Also do a check if we are going off the level
	bool ps
		= CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() == SID_PLAY;
	
	if (ps)
	{
		CVector2D pLevelSize = CCamera::Instance()->GetLevelSize();

		if ((pNext->GetX() < 0) || (pNext->GetX() + PLAYER_WIDTH > pLevelSize.GetX())
			|| (pNext->GetY() < 0) || (pNext->GetY() + PLAYER_HEIGHT > pLevelSize.GetY()))
		{
			return true;
		}
	}

	switch (r.m_result)
	{
	case ECollisionResult::COLLIDED:
		return true;
	default:
		return false; // Nothing in the way
	}
}

void CPlayer::DecideFrame()
{
	int nFrame = (SDL_GetTicks() / 100) % 3;

	if (m_bStepLastFrame)
	{
		nFrame = 1;
		m_bStepLastFrame = false;
	}

	m_iCurrentFrame = nFrame;
}

int CPlayer::GetRowFromDirection()
{
	switch(m_eObjectDirection)
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

void CPlayer::callStartCallbacks()
{
	std::map<std::string, HPlayerCallback>::iterator it = m_vOnMoveStart.begin();

	for (std::pair<std::string, HPlayerCallback> e : m_vOnMoveStart) {
		spdlog::debug("[{}] Calling registered callback ({})", m_sObjectName, e.first);
		e.second(this);
	}
}

void CPlayer::callEndCallbacks()
{
	std::map<std::string, HPlayerCallback>::iterator it = m_vOnMoveEnd.begin();

	for (std::pair<std::string, HPlayerCallback> e : m_vOnMoveEnd) {
		spdlog::debug("[{}] Calling registered callback ({})", m_sObjectName, e.first);
		e.second(this);
	}
}