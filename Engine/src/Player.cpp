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
#include <spdlog/spdlog.h>


#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

CPlayer::CPlayer()
	: CWarspiteObject()
{
}

void CPlayer::OnPlay()
{
	// Input binding
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_UP, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_DOWN, 1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_LEFT, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_RIGHT, 1.f);

	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_W, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveForward", SDL_SCANCODE_S, 1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_A, -1.f);
	CInputHandler::Instance()->SetAxisValue("MoveRight", SDL_SCANCODE_D, 1.f);
}

void CPlayer::Load(const CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);
}

bool CPlayer::OnThink()
{
	HandleInput();
	m_currentFrame = 0;

	if (moving)
	{
		m_position = VectorMath::Lerp(lastPosition, nextPosition, (m_timeLeft / 100));
		if (CSoundManager::Instance()->IsInitialised())
		{
			alCall(alListener3f, AL_POSITION, m_position.GetX(), m_position.GetY(), 0.f);
		}

		CCamera::Instance()->SetTarget(&m_position);
		DecideFrame();
	}

	CWarspiteObject::OnThink();
	return true;
}

void CPlayer::Draw()
{
	// Get the camera position to offset the drawing
	CVector2D cPos = CCamera::Instance()->GetPositionT();

	// Flip the sprite automatically if the velocity is negative.
	if (m_velocity.GetX() > 0)
	{
		CTextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			CBaseGame::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		CTextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			CBaseGame::Instance()->GetRenderer());
	}
}

void CPlayer::SetNextLocation(CVector2D nextLocation)
{
	moving = true;
	lastPosition = m_position;
	nextPosition = nextLocation;
	m_timeLeft = 0;
	m_stepLastFrame = true;
}

void CPlayer::HandleInput()
{
	if (m_timeLeft >= 100)
	{
		if (CInputHandler::Instance()->JoysticksInitialised())
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
		else
		{
			// Can this code be improved? (I hope so.)	
			MoveForward(CInputHandler::Instance()->GetAxisValue("MoveForward"));
			MoveRight(CInputHandler::Instance()->GetAxisValue("MoveRight"));
		}

		if (m_timeLeft >= 100)
		{
			moving = false;
		}
	}
	else
	{
		m_timeLeft += 5;
	}
}

void CPlayer::MoveForward(float axis)
{
	if (axis == 0.f) return;
	CVector2D curPos = m_position;
	IGameObject* lastObj = m_slastCollision.m_otherObject;
	
	m_currentRow = (axis > 0) ? 1 : 2;

	// Analog movement coming never.
	curPos.SetY(curPos.GetY() + (m_moveStep * axis));

	bool result = WillCollide(&curPos);

	if (!result && m_slastCollision.m_result == ECollisionResult::OVERLAP)
	{
		if (m_slastCollision.m_otherObject)
			m_slastCollision.m_otherObject->OnOverlapStart();

		if (lastObj)
			lastObj->OnOverlapEnd();
	}
	else if (!result)
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;

		if (lastObj)
			lastObj->OnOverlapEnd();
	}
	else
	{
		spdlog::info("Cannot move {} - collided!", ((axis > 0) ? "down" : "up"));
	}
}

void CPlayer::MoveRight(float axis)
{
	if (axis == 0.f) return;
	CVector2D curPos = m_position;
	IGameObject* lastObj = m_slastCollision.m_otherObject;
	
	m_currentRow = (axis > 0) ? 4 : 3;

	curPos.SetX(curPos.GetX() + (m_moveStep * axis));

	bool result = WillCollide(&curPos);

	if (!result && m_slastCollision.m_result == ECollisionResult::OVERLAP)
	{
		if (m_slastCollision.m_otherObject)
			m_slastCollision.m_otherObject->OnOverlapStart();

		if (lastObj)
			lastObj->OnOverlapEnd();
	}
	else if(!result)
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;


		if (lastObj)
			lastObj->OnOverlapEnd();
	}
	else
	{
		spdlog::info("Cannot move {} - collided!", ((axis > 0) ? "right" : "left"));
	}
}

bool CPlayer::WillCollide(CVector2D* pNext)
{
	SCollisionData r = CBaseGame::Instance()->GetStateManager()->IsColliding(*pNext);
	m_slastCollision = r;

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

	if (m_stepLastFrame)
	{
		nFrame = 0;
		m_stepLastFrame = false;
	}

	m_currentFrame = nFrame;
}