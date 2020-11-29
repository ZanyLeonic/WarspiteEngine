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

	CBaseGame::Instance()->SetPlayer(this);
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

void CPlayer::HandleInput()
{
	if (m_timeLeft >= 100)
	{
		// Can this code be improved? (I hope so.)	
		MoveForward(CInputHandler::Instance()->GetAxisValue("MoveForward"));
		MoveRight(CInputHandler::Instance()->GetAxisValue("MoveRight"));

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
	
	m_currentRow = (axis > 0) ? 1 : 2;

	// Analog movement coming never.
	curPos.SetY(curPos.GetY() + (m_moveStep * axis));
	if (!WillCollide(&curPos))
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;
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
	
	m_currentRow = (axis > 0) ? 4 : 3;

	curPos.SetX(curPos.GetX() + (m_moveStep * axis));
	if (!WillCollide(&curPos))
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;
	}
	else
	{
		spdlog::info("Cannot move {} - collided!", ((axis > 0) ? "right" : "left"));
	}
}

bool CPlayer::WillCollide(CVector2D* pNext)
{
	if (CBaseGame::Instance()->GetStateManager()->IsColliding(*pNext))
	{
		return true;
	}

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

	// Nothing is in the way!
	return false;
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