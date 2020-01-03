#include "InputHandler.h"
#include <iostream>
#include "Game.h"

InputHandler* InputHandler::s_pInstance = 0;

InputHandler::InputHandler()
{
	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}
}

void InputHandler::OnThink()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				Game::Instance()->Quit();
				break;

			case SDL_JOYAXISMOTION:
				onJoystickAxisMove(e);
				break;

			case SDL_JOYBUTTONUP:
				onJoystickButtonUp(e);
				break;

			case SDL_JOYBUTTONDOWN:
				onJoystickButtonDown(e);
				break;

			case SDL_MOUSEMOTION:
				onMouseMove(e);
				break;

			case SDL_MOUSEBUTTONDOWN:
				onMouseButtonDown(e);
				break;

			case SDL_MOUSEBUTTONUP:
				onMouseButtonUp(e);
				break;

			case SDL_KEYDOWN:
				onKeyDown();
				break;

			case SDL_KEYUP:
				onKeyUp();
				break;

			default:
				break;
		}
	}
}

void InputHandler::Destroy()
{
	if (m_bJoysticksInitialised)
	{
		for (unsigned int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void InputHandler::InitialiseJoysticks()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}

	if (SDL_NumJoysticks() > 0)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_Joystick* joy = SDL_JoystickOpen(i);

			if (joy)
			{
				m_joysticks.push_back(joy);
				m_joystickValues.push_back(std::make_pair(new
					Vector2D(0, 0), new Vector2D(0, 0)));

				std::vector<bool> tempButtons;

				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++)
				{
					tempButtons.push_back(false);
				}

				m_buttonStates.push_back(tempButtons);
			}
			else
			{
				std::cout << SDL_GetError();
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
		m_bJoysticksInitialised = true;

		std::cout << "Initialised " << m_joysticks.size() <<
			" joystick(s)\n";
	}
	else
	{
		m_bJoysticksInitialised = false;
	}
}

int InputHandler::GetXAxis(int joy, int stick)
{
	if (m_joystickValues.size() > 0) {
		if (stick == 1)
		{
			return m_joystickValues[joy].first->GetX();
		}
		else if (stick == 2)
		{
			return m_joystickValues[joy].second->GetX();
		}
	}
	return 0;
}

int InputHandler::GetYAxis(int joy, int stick)
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
		{
			return m_joystickValues[joy].first->GetY();
		}
		else if (stick == 2)
		{
			return m_joystickValues[joy].second->GetY();
		}
	}
	return 0;
}

bool InputHandler::IsKeyDown(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keystates[key] == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void InputHandler::onKeyDown()
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);
}

void InputHandler::onKeyUp()
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);
}

void InputHandler::onMouseMove(SDL_Event& event)
{
	m_mousePosition->SetX(event.motion.x);
	m_mousePosition->SetY(event.motion.y);
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = true;
	}

	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = true;
	}

	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = true;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = false;
	}

	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = false;
	}

	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = false;
	}
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	switch (event.jaxis.axis)
	{
		// left stick move left or right 
		case(0):
			if (event.jaxis.value > m_joystickDeadZone)
			{
				m_joystickValues[whichOne].first->SetX(1);
			}
			else if (event.jaxis.value < -m_joystickDeadZone)
			{
				m_joystickValues[whichOne].first->SetX(-1);
			}
			else
			{
				m_joystickValues[whichOne].first->SetX(0);
			}
			break;

		// left stick move up or down 
		case(1):
			if (event.jaxis.value > m_joystickDeadZone)
			{
				m_joystickValues[whichOne].first->SetY(1);
			}
			else if (event.jaxis.value < -m_joystickDeadZone)
			{
				m_joystickValues[whichOne].first->SetY(-1);
			}
			else
			{
				m_joystickValues[whichOne].first->SetY(0);
			}
			break;

		// right stick move left or right  
		case(3):
			if (event.jaxis.value > m_joystickDeadZone)
			{
				m_joystickValues[whichOne].second->SetX(1);
			}
			else if (event.jaxis.value < -m_joystickDeadZone)
			{
				m_joystickValues[whichOne].second->SetX(-1);
			}
			else 
			{
				m_joystickValues[whichOne].second->SetX(0); 
			}
			break;

		// right stick move up or down  
		case(4):
			if (event.jaxis.value > m_joystickDeadZone)
			{
				m_joystickValues[whichOne].second->SetY(1);
			}
			else if (event.jaxis.value < -m_joystickDeadZone)
			{
				m_joystickValues[whichOne].second->SetY(-1);
			}
			else
			{
				m_joystickValues[whichOne].second->SetY(0);
			}
			break;
	}
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = false;
}
