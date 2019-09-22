#include "Player.h"
#include "InputHandler.h"
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

	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));

	SDLGameObject::OnThink();
}

void Player::HandleInput()
{
	if (TheInputHandler::Instance()->JoysticksInitialised())
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
	}
}
