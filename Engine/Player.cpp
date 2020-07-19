#include "Player.h"
#include "InputHandler.h"
#include "Camera.h"
#include "ObjectLayer.h"
#include "Game.h"
#include "PlayState.h"
#include "TextureManager.h"
#include <iostream>

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

CPlayer::CPlayer()
	: CWarspiteObject()
{
}

void CPlayer::OnPlay()
{
	// Are we in the PlayState?
	CPlayState* ps = static_cast<CPlayState*>(CGame::Instance()->GetStateManager()->GetCurrentState());

	if (ps)
	{
		// If so - we can grab the current level and its Layers
		std::vector<ILayer*> objs = *ps->GetLoadedLevel()->GetLayers();

		for (int i = 0; i < objs.size(); i++)
		{
			// Only care about the ObjectLayers - since that's what we are going to collide with.
			CObjectLayer* obl = dynamic_cast<CObjectLayer*>(objs[i]);
			if (!obl) continue;

			// Add each GameObject vector to a vector of vectors.
			m_objects.push_back(obl->GetGameObjects());
		}
	}
}

void CPlayer::Load(const CObjectParams* pParams)
{
	CInputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_C, std::bind(&CPlayer::KeyDown, this));
	CInputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_C, std::bind(&CPlayer::KeyUp, this));

	CWarspiteObject::Load(pParams);
}

bool CPlayer::OnThink()
{
	// std::cout << "\r";
	// std::cout << "PLAYER -> X: " << m_position.GetX() << " Y: " << m_position.GetY() << " CAM -> X: " << Camera::Instance()->GetPosition().GetX() << " Y: " << Camera::Instance()->GetPosition().GetY() << " TimeLeft: " << float(m_timeLeft / 100) <<  " Frame: " << m_frameOffset << "   ";
	
	HandleInput();
	
	m_currentFrame = 0;

	if (moving)
	{
		m_position = VectorMath::Lerp(lastPosition, nextPosition, (m_timeLeft / 100));
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
			CGame::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		CTextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			CGame::Instance()->GetRenderer());
	}
}

void CPlayer::KeyDown()
{
	std::cout << "Key has been pressed down!\n";
}

void CPlayer::KeyUp()
{
	std::cout << "Key has been released!\n";
}

void CPlayer::HandleInput()
{
	if (m_timeLeft >= 100)
	{
		// Can this code be improved? (I hope so.)	
		if (CInputHandler::Instance()->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			MoveForward(1);
		}
		else if (CInputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
		{
			MoveForward(-1);
		}
		else if (CInputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
		{
			MoveRight(1);
		}
		else if (CInputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
		{
			MoveRight(-1);
		}
		else if (m_timeLeft >= 100)
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
	CVector2D curPos = m_position;
	
	m_currentRow = (axis > 0) ? 1 : 2;

	// Analog movement coming never.
	curPos.SetY(curPos.GetY() + (m_moveStep * axis));
	if (IsPositionFree(&curPos))
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;
	}
	else
	{
		std::cout << "Cannot move " << ((axis > 0) ? "down" : "up") << " - collided!\n";
	}
}

void CPlayer::MoveRight(float axis)
{
	CVector2D curPos = m_position;
	
	m_currentRow = (axis > 0) ? 4 : 3;

	curPos.SetX(curPos.GetX() + (m_moveStep * axis));
	if (IsPositionFree(&curPos))
	{
		moving = true;
		lastPosition = m_position;
		nextPosition = curPos;
		m_timeLeft = 0;
		m_stepLastFrame = true;
	}
	else
	{
		std::cout << "Cannot move "<< ((axis > 0) ? "right" : "left") <<" - collided!\n";
	}
}

bool CPlayer::IsPositionFree(CVector2D* pNext)
{
	// Get the value from the pointer and store it in a rvalue.
	CVector2D nPos = CVector2D(*pNext);

	// Go through each ObjectLayer we got earlier
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		if (!m_objects[i]) continue;

		// Get an rvalue of the list of GameObject's for the iterated layer
		std::vector<IGameObject*>& ir = *m_objects[i];

		for (size_t j = 0; j < ir.size(); j++)
		{
			// Check if the GameObject is in the way and isn't us
			if (ir[j] != this && ir[j]->GetPosition() == nPos && ir[j]->ShouldCollide())
			{
				return false;
			}
		}
	}

	// Also do a check if we are going off the level
	CPlayState* pPS = static_cast<CPlayState*>(CGame::Instance()->GetStateManager()->GetCurrentState());
	if (pPS != nullptr)
	{
		CLevel* pLevel = pPS->GetLoadedLevel();

		if ((pNext->GetX() < 0) || (pNext->GetX() + PLAYER_WIDTH > pLevel->m_LevelSize.GetX()))
		{
			return false;
		}

		if ((pNext->GetY() < 0) || (pNext->GetY() + PLAYER_HEIGHT > pLevel->m_LevelSize.GetY()))
		{
			return false;
		}
	}

	// Nothing is in the way!
	return true;
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