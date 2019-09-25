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
		if (e.type == SDL_QUIT)
		{
			Game::Instance()->Quit();
		}

		m_keystates = (Uint8*)SDL_GetKeyboardState(0);

		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				m_mouseButtonStates[LEFT] = true;
			}

			if (e.button.button == SDL_BUTTON_MIDDLE)
			{
				m_mouseButtonStates[MIDDLE] = true;
			}

			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				m_mouseButtonStates[RIGHT] = true;
			}
		}

		if (e.type == SDL_MOUSEBUTTONUP)
		{
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				m_mouseButtonStates[LEFT] = false;
			}

			if (e.button.button == SDL_BUTTON_MIDDLE)
			{
				m_mouseButtonStates[MIDDLE] = false;
			}

			if (e.button.button == SDL_BUTTON_RIGHT)
			{
				m_mouseButtonStates[RIGHT] = false;
			}
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			m_mousePosition->SetX(e.motion.x);
			m_mousePosition->SetY(e.motion.y);
		}

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			int whichOne = e.jaxis.which;

			m_buttonStates[whichOne][e.jbutton.button] = true;
		}

		if (e.type == SDL_JOYBUTTONUP)
		{
			int whichOne = e.jaxis.which;

			m_buttonStates[whichOne][e.jbutton.button] = false;
		}

		if (e.type == SDL_JOYAXISMOTION)
		{
			int whichOne = e.jaxis.which;

			// left stick move left or right    
			if (e.jaxis.axis == 0)
			{
				if (e.jaxis.value > m_joystickDeadZone)
				{
					m_joystickValues[whichOne].first->SetX(1);
				}
				else if (e.jaxis.value < -m_joystickDeadZone)
				{
					m_joystickValues[whichOne].first->SetX(-1);
				}
				else
				{
					m_joystickValues[whichOne].first->SetX(0);
				}
			}

			// left stick move up or down    
			if (e.jaxis.axis == 1)
			{
				if (e.jaxis.value > m_joystickDeadZone)
				{
					m_joystickValues[whichOne].first->SetY(1);
				}
				else if (e.jaxis.value < -m_joystickDeadZone) 
				{ 
					m_joystickValues[whichOne].first->SetY(-1); 
				}
				else 
				{ 
					m_joystickValues[whichOne].first->SetY(0); 
				}
			}    
			
			// right stick move left or right    
			if (e.jaxis.axis == 3) {
				if (e.jaxis.value > m_joystickDeadZone) 
				{ 
					m_joystickValues[whichOne].second->SetX(1); 
				}
				else if (e.jaxis.value < -m_joystickDeadZone) 
				{
					m_joystickValues[whichOne].second->SetX(-1);
				}
				else { m_joystickValues[whichOne].second->SetX(0); }
			}    
			
			// right stick move up or down    
			if (e.jaxis.axis == 4) 
			{ 
				if (e.jaxis.value > m_joystickDeadZone) 
				{ 
					m_joystickValues[whichOne].second->SetY(1); 
				} 
				else if (e.jaxis.value < -m_joystickDeadZone) 
				{ 
					m_joystickValues[whichOne].second->SetY(-1); 
				} 
				else 
				{
					m_joystickValues[whichOne].second->SetY(0); 
				} 
			}
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
			" joystick(s)";
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
