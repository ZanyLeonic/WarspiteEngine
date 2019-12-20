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

	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));

	SDLGameObject::OnThink();
}

void Player::HandleInput()
{
	Vector2D* vec = TheInputHandler::Instance()->GetMousePosition();

	m_velocity = (*vec - m_position) / 100;

	if (TheInputHandler::Instance()->GetMouseButtonState(LEFT))
	{
		m_velocity.SetX(1);
	}

	if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_F))
	{
		if (windowMode == true)
		{
			SDL_SetWindowFullscreen(Game::Instance()->GetWindow(), 0);
			windowMode = false;
		}
		else
		{
			SDL_SetWindowFullscreen(Game::Instance()->GetWindow(), SDL_WINDOW_FULLSCREEN);
			windowMode = true;
		}
	}

	if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_LEFT))
	{
		m_velocity.SetX(-2);
	}
	if (InputHandler::Instance()->IsKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_velocity.SetX(2);
	}

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
