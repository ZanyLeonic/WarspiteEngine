#include "Player.h"
#include "InputHandler.h"
#include "Camera.h"
#include "ObjectLayer.h"
#include "Game.h"
#include <iostream>

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

Player::Player()
	: WarspiteObject()
{
}

void Player::OnPlay()
{
	// Are we in the PlayState?
	PlayState* ps = static_cast<PlayState*>(Game::Instance()->GetStateManager()->GetCurrentState());

	if (ps)
	{
		// If so - we can grab the current level and its Layers
		std::vector<Layer*> objs = *ps->GetLoadedLevel()->GetLayers();

		for (int i = 0; i < objs.size(); i++)
		{
			// Only care about the ObjectLayers - since that's what we are going to collide with.
			ObjectLayer* obl = dynamic_cast<ObjectLayer*>(objs[i]);
			if (!obl) continue;

			// Add each GameObject vector to a vector of vectors.
			m_objects.push_back(obl->GetGameObjects());
		}
	}
}

void Player::Load(const ObjectParams* pParams)
{
	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_C, std::bind(&Player::KeyDown, this));
	InputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_C, std::bind(&Player::KeyUp, this));

	WarspiteObject::Load(pParams);
}

bool Player::OnThink()
{
	// std::cout << "\r";
	// std::cout << "PLAYER -> X: " << m_position.GetX() << " Y: " << m_position.GetY() << " CAM -> X: " << Camera::Instance()->GetPosition().GetX() << " Y: " << Camera::Instance()->GetPosition().GetY() << " TimeLeft: " << float(m_timeLeft / 100) <<  " Frame: " << m_frameOffset << "   ";
	
	HandleInput();

	m_currentFrame = 0;

	if (moving)
	{
		m_position = VectorMath::Lerp(lastPosition, nextPosition, (m_timeLeft / 100));
		Camera::Instance()->SetTarget(&m_position);
		
		DecideFrame();
	}

	WarspiteObject::OnThink();
	return true;
}

void Player::Draw()
{
	// Get the camera position to offset the drawing
	Vector2D cPos = Camera::Instance()->GetPositionT();

	// Flip the sprite automatically if the velocity is negative.
	if (m_velocity.GetX() > 0)
	{
		TextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			Game::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		TextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			Game::Instance()->GetRenderer());
	}
}

void Player::KeyDown()
{
	std::cout << "Key has been pressed down!\n";
}

void Player::KeyUp()
{
	std::cout << "Key has been released!\n";
}

void Player::HandleInput()
{
	if (m_timeLeft >= 100)
	{
		// Can this code be improved? (I hope so.)	
		if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			MoveForward(1);
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
		{
			MoveForward(-1);
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
		{
			MoveRight(1);
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
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

void Player::MoveForward(float axis)
{
	Vector2D curPos = m_position;
	
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

void Player::MoveRight(float axis)
{
	Vector2D curPos = m_position;
	
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

bool Player::IsPositionFree(Vector2D* pNext)
{
	// Get the value from the pointer and store it in a rvalue.
	Vector2D nPos = Vector2D(*pNext);

	// Go through each ObjectLayer we got earlier
	for (int i = 0; i < m_objects.size(); i++)
	{
		if (!m_objects[i]) continue;

		// Get an rvalue of the list of GameObject's for the iterated layer
		std::vector<GameObject*>& ir = *m_objects[i];

		for (int j = 0; j < ir.size(); j++)
		{
			// Check if the GameObject is in the way and isn't us
			if (ir[j] != this && ir[j]->GetPosition() == nPos)
			{
				return false;
			}
		}
	}

	// Also do a check if we are going off the level
	PlayState* pPS = static_cast<PlayState*>(Game::Instance()->GetStateManager()->GetCurrentState());
	if (pPS != 0)
	{
		Level* pLevel = pPS->GetLoadedLevel();

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

void Player::DecideFrame()
{
	int nFrame = (SDL_GetTicks() / 100) % 3;

	if (m_stepLastFrame)
	{
		nFrame = 0;
		m_stepLastFrame = false;
	}

	m_currentFrame = nFrame;
}