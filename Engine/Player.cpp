#include "Player.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Game.h"
#include <iostream>

#define PLAYER_WIDTH 22
#define PLAYER_HEIGHT 27

Player::Player()
	: WarspiteObject()
{

}

void Player::Load(const ObjectParams* pParams)
{
	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_C, std::bind(&Player::KeyDown, this));
	InputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_C, std::bind(&Player::KeyUp, this));

	WarspiteObject::Load(pParams);
}

bool Player::OnThink()
{
	std::cout << "\r";
	std::cout << "X: " << m_position.GetX() << " Y: " << m_position.GetY() << " TimeLeft: " << float(timeLeft / 100) << "   ";

	HandleInput();

	m_currentFrame = 0;

	if (moving)
	{
		m_position = VectorMath::Lerp(lastPosition, nextPosition, (timeLeft / 100));
		Camera::Instance()->SetTarget(&m_position);
		m_currentFrame = int(((SDL_GetTicks() / (1000 / 4)) % 2));
	}

	WarspiteObject::OnThink();
	return true;
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
	if (timeLeft >= 100)
	{
		Vector2D curPos = m_position;

		if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
		{
			//m_position.SetY(m_position.GetY() - moveStep);
			
			m_currentRow = 2;

			curPos.SetY(curPos.GetY() - moveStep);
			if (IsPositionFree(&curPos))
			{
				moving = true;
				lastPosition = m_position;
				nextPosition = curPos;
				timeLeft = 0;
			}
			else
			{
				std::cout << "Cannot move up - collided!\n";
			}
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			curPos.SetY(curPos.GetY() + moveStep);

			m_currentRow = 1;

			if (IsPositionFree(&curPos))
			{
				moving = true;
				lastPosition = m_position;
				nextPosition = curPos;
				timeLeft = 0;
			}
			else
			{
				std::cout << "Cannot move down - collided!\n";
			}
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
		{
			curPos.SetX(curPos.GetX() - moveStep);

			m_currentRow = 3;

			if (IsPositionFree(&curPos))
			{
				moving = true;
				lastPosition = m_position;
				nextPosition = curPos;
				timeLeft = 0;
			}
			else
			{
				std::cout << "Cannot move left - collided!\n";
			}
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
		{
			curPos.SetX(curPos.GetX() + moveStep);

			m_currentRow = 4;

			if (IsPositionFree(&curPos))
			{
				moving = true;
				lastPosition = m_position;
				nextPosition = curPos;
				timeLeft = 0;
			}
			else
			{
				std::cout << "Cannot move right - collided!\n";
			}
		}
		else if (timeLeft >= 100)
		{
			moving = false;
		}
	}
	else
	{
		timeLeft += 5;
	}
}

bool Player::IsPositionFree(Vector2D* pNext)
{
	std::vector<GameObject*> pGameObj = Game::Instance()->GetStateManager()->GetCurrentState()->GetGameObjects();

	Vector2D nPos = Vector2D(*pNext);

	// Linear search through the current GameObjects
	for (int i = 0; i < pGameObj.size(); i++)
	{
		// Check if the GameObject is in the way and isn't us
		if (pGameObj[i] != this && pGameObj[i]->GetPosition() == nPos)
		{
			return false;
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
