#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include <iostream>

Player::Player(const ObjectParams* pParams)
	: SDLGameObject(pParams)
{
}

void Player::OnThink()
{
	m_velocity.SetX(0);
	m_velocity.SetY(0);

	HandleInput();

	m_currentFrame = 0;

	if (moving)
	{
		//switch (m_currentFrame)
		//{
		//case 0:
		//	m_currentFrame = 1;
		//	break;
		//case 1:
		//	m_currentFrame = 2;
		//	break;
		//case 2:
		//	m_currentFrame = 3;
		//	break;
		//}
		m_currentFrame = int(((SDL_GetTicks() / 150) % 3));
	}

	SDLGameObject::OnThink();
}

void Player::HandleInput()
{
	if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_UP))
	{
		m_position.SetY(m_position.GetY() - 1);
		// m_velocity.SetY(-0.5);
		m_currentRow = 2;
		moving = true;
	}
	else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_DOWN))
	{
		m_position.SetY(m_position.GetY() + 1);
		// m_velocity.SetY(0.5);
		m_currentRow =1;
		moving = true;
	}
	else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		m_position.SetX(m_position.GetX() - 1);
		// m_velocity.SetX(-0.5);
		m_currentRow = 3;
		moving = true;
	}
	else if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_position.SetX(m_position.GetX() + 1);
		// m_velocity.SetX(0.5);
		m_currentRow = 4;
		moving = true;
	}
	else
	{
		moving = false;
	}

	/*if (TheInputHandler::Instance()->JoysticksInitialised())
	{
		if (TheInputHandler::Instance()->GetButtonState(0, 3)) 
		{ 
			m_velocity.SetX(1); 
		}

		if (TheInputHandler::Instance()->GetXAxis(0, 1) > 0 ||
			TheInputHandler::Instance()->GetXAxis(0, 1) < 0)
		{
			m_velocity.SetX(1 * TheInputHandler::Instance()->GetXAxis(0, 1));
		}

		if (TheInputHandler::Instance()->GetYAxis(0, 1) > 0 ||
			TheInputHandler::Instance()->GetYAxis(0, 1) < 0)
		{
			m_velocity.SetX(1 * TheInputHandler::Instance()->GetYAxis(0, 1));
		}

		if (TheInputHandler::Instance()->GetXAxis(0, 2) > 0 ||
			TheInputHandler::Instance()->GetXAxis(0, 2) < 0)
		{
			m_velocity.SetX(1 * TheInputHandler::Instance()->GetXAxis(0, 2));
		}

		if (TheInputHandler::Instance()->GetYAxis(0, 2) > 0 ||
			TheInputHandler::Instance()->GetYAxis(0, 2) < 0)
		{
			m_velocity.SetX(1 * TheInputHandler::Instance()->GetYAxis(0, 2));
		}
	}*/
}
