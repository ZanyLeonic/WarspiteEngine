#include "InputHandler.h"
#include "Game.h"
#include "ScriptManager.h"
#include "ScriptWrappers.h"
#include <iostream>

CInputHandler* CInputHandler::s_pInstance = 0;

CInputHandler::CInputHandler()
{
	m_bJoysticksInitialised = false;

	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}

	// Expose the current camera to the Python API
	if (CScriptManager::Instance()->GetEngineModule().attr(INPUTOBJECT_NAME).is_none())
	{
		m_inputPtr = std::make_shared<SInputObject>(SInputObject(this));
		CScriptManager::Instance()->GetEngineModule().attr(INPUTOBJECT_NAME) = m_inputPtr;
	}
}

void CInputHandler::OnThink()
{
	SDL_Event e;
	
	// Handles events for various cases
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT:
				CGame::Instance()->Quit();
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

void CInputHandler::Destroy()
{
	if (m_bJoysticksInitialised)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void CInputHandler::InitialiseJoysticks()
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
					CVector2D(0, 0), new CVector2D(0, 0)));

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

float CInputHandler::GetXAxis(int joy, int stick)
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

float CInputHandler::GetYAxis(int joy, int stick)
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

void CInputHandler::AddActionKeyDown(SDL_Scancode key, KeyCallback callBack)
{
	if (!m_keyReleased[key])
	{
		m_keyReleased[key] = true;
	}
	m_keyDownCallbacks[key] = callBack;
}

void CInputHandler::AddActionKeyUp(SDL_Scancode key, KeyCallback callBack)
{
	if (!m_keyReleased[key])
	{
		m_keyReleased[key] = true;
	}
	m_keyUpCallbacks[key] = callBack;
}

bool CInputHandler::IsKeyDown(SDL_Scancode key)
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

void CInputHandler::SetReleaseState(SDL_Scancode key, bool state)
{
	m_keyReleased[key] = state;
}

void CInputHandler::onKeyDown()
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);

	if (m_keyDownCallbacks.size() != 0)
	{
		std::map<SDL_Scancode, KeyCallback>::iterator it;

		for (it = m_keyDownCallbacks.begin(); it != m_keyDownCallbacks.end(); it++)
		{
			if (m_keystates[it->first] == 1
				&& m_keyReleased[it->first] == true)
			{
				m_keyReleased[it->first] = false;

				try 
				{
					it->second();
				}
				catch(std::exception e)
				{
					spdlog::error("Exception occurred when trying to call callback for keycode \"{}\":", it->first);
					spdlog::error("Exception: {}", e.what());
				}
			}
		}
	}
}

void CInputHandler::onKeyUp()
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);

	if (m_keyUpCallbacks.size() != 0)
	{
		std::map<SDL_Scancode, KeyCallback>::iterator it;

		for (it = m_keyUpCallbacks.begin(); it != m_keyUpCallbacks.end(); it++)
		{
			if (m_keystates[it->first] == 0
				&& m_keyReleased[it->first] == false)
			{
				m_keyReleased[it->first] = true;

				try
				{
					it->second();
				}
				catch (std::exception e)
				{
					spdlog::error("Exception occurred when trying to call callback for keycode \"{}\":", it->first);
					spdlog::error("Exception: {}", e.what());
				}
			}
		}
	}
}

void CInputHandler::onMouseMove(SDL_Event& event)
{
	m_mousePosition->SetX((float)event.motion.x);
	m_mousePosition->SetY((float)event.motion.y);
}

void CInputHandler::onMouseButtonDown(SDL_Event& event)
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

void CInputHandler::onMouseButtonUp(SDL_Event& event)
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

void CInputHandler::onJoystickAxisMove(SDL_Event& event)
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

void CInputHandler::onJoystickButtonDown(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = true;
}

void CInputHandler::onJoystickButtonUp(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = false;
}

void CInputHandler::keyDownTest()
{
	std::cout << "Key has been pressed down!";
}
