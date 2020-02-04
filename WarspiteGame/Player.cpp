#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include <iostream>

Player::Player(const ObjectParams* pParams)
	: SDLGameObject(pParams)
{
	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_C, std::bind(&Player::KeyDown, this));
	InputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_C, std::bind(&Player::KeyUp, this));
}

bool Player::OnThink()
{
	std::cout << "\r";
	std::cout << "X: " << m_position.GetX() << " Y: " << m_position.GetY() << " TimeLeft: " << timeLeft << "    ";

	HandleInput();

	m_currentFrame = 0;

	if (moving)
	{
		m_currentFrame = int(((SDL_GetTicks() / (1000 / 4)) % 2));
	}

	SDLGameObject::OnThink();
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
	if (timeLeft <= 0)
	{
		Vector2D curPos = m_position;

		if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
		{
			//m_position.SetY(m_position.GetY() - moveStep);
			
			m_currentRow = 2;
			moving = true;

			curPos.SetY(curPos.GetY() - moveStep);
			if (IsPositionFree(&curPos))
			{
				m_position = curPos;
				timeLeft = 10;
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
			moving = true;

			if (IsPositionFree(&curPos))
			{
				m_position = curPos;
				timeLeft = 10;
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
			moving = true;

			if (IsPositionFree(&curPos))
			{
				m_position = curPos;
				timeLeft = 10;
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
			moving = true;

			if (IsPositionFree(&curPos))
			{
				m_position = curPos;
				timeLeft = 10;
			}
			else
			{
				std::cout << "Cannot move right - collided!\n";
			}
		}
		else if (timeLeft <= 0)
		{
			moving = false;
		}
	}
	else
	{
		timeLeft -= 1;
	}
}

bool Player::IsPositionFree(Vector2D* pNext)
{
	std::vector<GameObject*> pGameObj = Game::Instance()->GetStateManager()->GetCurrentState()->GetGameObjects();

	Vector2D nPos = Vector2D(*pNext);

	for (int i = 0; i < pGameObj.size(); i++)
	{
		if (pGameObj[i] != this)
		{
			if (dynamic_cast<SDLGameObject*>(pGameObj[i])->GetPosition() == nPos)
			{
				return false;
			}
		}
	}

	return true;
}
