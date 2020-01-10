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

void Player::OnThink()
{
	std::cout << "\r";
	std::cout << "X: " << m_position.GetX() << " Y: " << m_position.GetY() << " TimeLeft: " << timeLeft << "    ";

	HandleInput();

	m_currentFrame = 0;

	//if (moving)
	//{
	//	m_currentFrame = int(((SDL_GetTicks() / 150) % 3));
	//}

	SDLGameObject::OnThink();
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
		if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
		{
			m_position.SetY(m_position.GetY() - moveStep);
			m_currentRow = 2;

			moving = true;

			timeLeft = 10;
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			m_position.SetY(m_position.GetY() + moveStep);
			m_currentRow = 1;

			moving = true;

			timeLeft = 10;
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
		{
			m_position.SetX(m_position.GetX() - moveStep);
			m_currentRow = 3;

			moving = true;

			timeLeft = 10;
		}
		else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
		{
			m_position.SetX(m_position.GetX() + moveStep);
			m_currentRow = 4;

			moving = true;

			timeLeft = 10;
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
