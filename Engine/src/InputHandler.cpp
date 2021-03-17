#include "InputHandler.h"
#include "Game.h"
#include "ScriptManager.h"
#include "ScriptWrappers.h"

CInputHandler* CInputHandler::s_pInstance = 0;

CInputHandler::CInputHandler()
{
	m_bJoysticksInitialised = false;

	for (int i = 0; i < 3; i++)
	{
		m_mouseButtonStates.push_back(false);
	}

	// Expose the input interface to the Python API
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
				CBaseGame::Instance()->Quit();
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

			case SDL_MOUSEBUTTONUP:
				onMouseButtonUp(e);
				break;

			case SDL_MOUSEBUTTONDOWN:
				onMouseButtonDown(e);
				break;

			case SDL_KEYDOWN:
				onKeyDown(e);
				setAxisValues();
				break;

			case SDL_KEYUP:
				onKeyUp(e);
				setAxisValues();
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
				spdlog::error("Error while initialising joystick ({})", i);
				spdlog::error(SDL_GetError());
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
		m_bJoysticksInitialised = true;

		spdlog::info("Initialised {} joystick(s)", 
			m_joysticks.size());
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

void CInputHandler::AddActionKeyDown(SDL_Scancode key, HKeyCallback callBack)
{
	if (!m_keyReleased[key])
	{
		m_keyReleased[key] = true;
	}

	m_actionKeyDownCallbacks[key] = callBack;
	m_actionKeyUpCallbacks[key] = sBlankCallback;
}

void CInputHandler::AddActionKeyUp(SDL_Scancode key, HKeyCallback callBack)
{
	if (!m_keyReleased[key])
	{
		m_keyReleased[key] = true;
	}

	m_actionKeyUpCallbacks[key] = callBack;
	m_actionKeyDownCallbacks[key] = sBlankCallback;
}

void CInputHandler::RemoveActionKeyDown(SDL_Scancode key)
{
	m_actionKeyDownCallbacks.erase(key);
}

void CInputHandler::RemoveActionKeyUp(SDL_Scancode key)
{
	m_actionKeyUpCallbacks.erase(key);
}

void CInputHandler::AddOnKeyDown(HKeyCallback callBack)
{
	m_keyDownCallbacks.push_back(callBack);
}

void CInputHandler::AddOnKeyUp(HKeyCallback callBack)
{
	m_keyUpCallbacks.push_back(callBack);
}

void CInputHandler::AddOnJoyDown(HInputCallback callBack)
{
	m_joyDownCallbacks.push_back(callBack);
}

void CInputHandler::AddOnJoyUp(HInputCallback callBack)
{
	m_joyUpCallbacks.push_back(callBack);
}

void CInputHandler::AddOnMouseDown(HInputCallback callBack)
{
	m_mouseDownCallbacks.push_back(callBack);
}

void CInputHandler::AddOnMouseUp(HInputCallback callBack)
{
	m_mouseUpCallbacks.push_back(callBack);
}

void CInputHandler::RemoveOnKeyDown(HKeyCallback callBack)
{
	for (size_t i = 0; i < m_keyDownCallbacks.size(); i++)
	{
		if (m_keyDownCallbacks[i].target<SDL_Scancode>() == callBack.target<SDL_Scancode>())
		{
			m_keyDownCallbacks.erase(m_keyDownCallbacks.begin() + i);
			break;
		}
	}
	
	//m_keyDownCallbacks.erase(std::remove(m_keyDownCallbacks.begin(), m_keyDownCallbacks.end(), callBack), m_keyDownCallbacks.end());
}

void CInputHandler::RemoveOnKeyUp(HKeyCallback callBack)
{
	for (size_t i = 0; i < m_keyUpCallbacks.size(); i++)
	{
		if (m_keyUpCallbacks[i].target<SDL_Scancode>() == callBack.target<SDL_Scancode>())
		{
			m_keyUpCallbacks.erase(m_keyUpCallbacks.begin() + i);
			break;
		}
	}

	//m_keyUpCallbacks.erase(std::remove(m_keyUpCallbacks.begin(), m_keyUpCallbacks.end(), callBack), m_keyUpCallbacks.end());
}

void CInputHandler::RemoveOnJoyDown(HInputCallback callBack)
{
	for (size_t i = 0; i < m_joyDownCallbacks.size(); i++)
	{
		if (m_joyDownCallbacks[i].target<SDL_Event>() == callBack.target<SDL_Event>())
		{
			m_joyDownCallbacks.erase(m_joyDownCallbacks.begin() + i);
			break;
		}
	}
	// m_joyDownCallbacks.erase(std::remove(m_joyDownCallbacks.begin(), m_joyDownCallbacks.end(), callBack), m_joyDownCallbacks.end());
}

void CInputHandler::RemoveOnJoyUp(HInputCallback callBack)
{
	for (size_t i = 0; i < m_joyUpCallbacks.size(); i++)
	{
		if (m_joyUpCallbacks[i].target<SDL_Event>() == callBack.target<SDL_Event>())
		{
			m_joyUpCallbacks.erase(m_joyUpCallbacks.begin() + i);
			break;
		}
	}
	// m_joyUpCallbacks.erase(std::remove(m_joyUpCallbacks.begin(), m_joyUpCallbacks.end(), callBack), m_joyUpCallbacks.end());
}

void CInputHandler::RemoveOnMouseDown(HInputCallback callBack)
{
	for (size_t i = 0; i < m_mouseDownCallbacks.size(); i++)
	{
		if (m_mouseDownCallbacks[i].target<SDL_Event>() == callBack.target<SDL_Event>())
		{
			m_mouseDownCallbacks.erase(m_mouseDownCallbacks.begin() + i);
			break;
		}
	}
	// m_mouseDownCallbacks.erase(std::remove(m_mouseDownCallbacks.begin(), m_mouseDownCallbacks.end(), callBack), m_mouseDownCallbacks.end());
}

void CInputHandler::RemoveOnMouseUp(HInputCallback callBack)
{
	for (size_t i = 0; i < m_mouseUpCallbacks.size(); i++)
	{
		if (m_mouseUpCallbacks[i].target<SDL_Event>() == callBack.target<SDL_Event>())
		{
			m_mouseUpCallbacks.erase(m_mouseUpCallbacks.begin() + i);
			break;
		}
	}
	// m_mouseUpCallbacks.erase(std::remove(m_mouseUpCallbacks.begin(), m_mouseUpCallbacks.end(), callBack), m_mouseUpCallbacks.end());
}

// An okay stop gap - for now.
void CInputHandler::RemoveOnCallbacks()
{
	m_keyDownCallbacks.clear();
	m_keyUpCallbacks.clear();
	m_joyDownCallbacks.clear();
	m_joyUpCallbacks.clear();
	m_mouseDownCallbacks.clear();
	m_mouseUpCallbacks.clear();
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

void CInputHandler::SetAxisValue(std::string name, SDL_Scancode key, float value)
{
	m_keyVirtualAxis[key] = name;

	std::map<SDL_Scancode, float> axisVal = m_keyAxisValue[name];
	axisVal[key] = value;
	m_currentKeyAxisValue[name] = 0.f;

	m_keyAxisValue[name] = axisVal;
}

void CInputHandler::RemoveAxisValue(std::string name, SDL_Scancode key)
{
	if (!name.empty())
	{
		m_keyVirtualAxis.erase(key);
		m_keyAxisValue[name].erase(key);
		spdlog::debug("Removed keycode \"{}\" from axis \"{}\" successfully", key, name);
	}
	spdlog::warn("Something called to remove a key from an axis with no axis name defined!");
}

float CInputHandler::GetAxisValue(std::string name)
{
	return m_currentKeyAxisValue[name];
}

void CInputHandler::RemoveAxis(std::string name)
{
	if (!name.empty())
	{
		std::map<SDL_Scancode, std::string>::iterator it;
		std::vector<SDL_Scancode> toBeRemoved = {};

		// Iterate to remove all keys referencing this axis
		for (it = m_keyVirtualAxis.begin(); it != m_keyVirtualAxis.end(); it++)
		{
			if (it->second == name)
			{
				// Keep track of all the keys used.
				toBeRemoved.push_back(it->first);

				// Remove the found keypair
				m_keyVirtualAxis.erase(it);
			}
		}
		
		// Remove the values stored for these axis
		m_currentKeyAxisValue.erase(name);
		m_keyAxisValue.erase(name);

		// Don't keep track of keys from deleted axis
		for (size_t i = 0; i < toBeRemoved.size(); i++)
		{
			m_keyAxisStates.erase(toBeRemoved[i]);
		}

		spdlog::debug("Removed axis \"{}\" successfully", name);
	}
	spdlog::warn("Something called to remove an axis with no axis name defined!");
}

void CInputHandler::RemoveAllAxis()
{
	// Just clear all of our maps.
	m_keyVirtualAxis.clear();
	m_currentKeyAxisValue.clear();
	m_keyAxisValue.clear();
	m_keyAxisStates.clear();
}

void CInputHandler::onKeyDown(SDL_Event& event)
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);

	if (m_actionKeyDownCallbacks.size() != 0)
	{
		std::map<SDL_Scancode, HKeyCallback>::iterator it;

		for (it = m_actionKeyDownCallbacks.begin(); it != m_actionKeyDownCallbacks.end(); it++)
		{
			if (m_keystates[it->first] == 1
				&& m_keyReleased[it->first] == true)
			{
				m_keyReleased[it->first] = false;

				try 
				{
					it->second(it->first);
				}
				catch(std::exception e)
				{
					spdlog::error("Exception occurred when trying to call callback for keycode \"{}\":", it->first);
					spdlog::error("Exception: {}", e.what());
				}
			}
		}
	}

	if (m_keyDownCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_keyDownCallbacks.size(); i++)
		{
			m_keyDownCallbacks[i](event.key.keysym.scancode);
		}
	}
}

void CInputHandler::onKeyUp(SDL_Event& event)
{
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);

	if (m_actionKeyUpCallbacks.size() != 0)
	{
		std::map<SDL_Scancode, HKeyCallback>::iterator it;

		for (it = m_actionKeyUpCallbacks.begin(); it != m_actionKeyUpCallbacks.end(); it++)
		{
			if (m_keystates[it->first] == 0
				&& m_keyReleased[it->first] == false)
			{
				m_keyReleased[it->first] = true;

				try
				{
					it->second(it->first);
				}
				catch (std::exception e)
				{
					spdlog::error("Exception occurred when trying to call callback for keycode \"{}\":", it->first);
					spdlog::error("Exception: {}", e.what());
				}
			}
		}
	}

	if (m_keyUpCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_keyUpCallbacks.size(); i++)
		{
			m_keyUpCallbacks[i](event.key.keysym.scancode);
		}
	}
}

void CInputHandler::setAxisValues()
{
	// Get the latest state of the keys
	m_keystates = (Uint8*)SDL_GetKeyboardState(0);

	std::map<SDL_Scancode, std::string>::iterator it = m_keyVirtualAxis.begin();

	if (m_keyVirtualAxis.size() <= 0) return;

	// Iterate through the map
	while (it != m_keyVirtualAxis.end())
	{
		if (!m_keyVirtualAxis[it->first].empty())
		{
			float m_curKeyVal = m_keyAxisValue[it->second][it->first];

			// Do the method that matches the key
			switch (m_keystates[it->first])
			{
			case 0:
				// Only change it if it has been pressed.
				if (m_keyAxisStates[it->first])
				{
					m_currentKeyAxisValue[it->second] -= m_curKeyVal;
					m_keyAxisStates[it->first] = false;
				}
				break;
			case 1:
				// Only change it if it has been released.
				if (!m_keyAxisStates[it->first])
				{
					m_currentKeyAxisValue[it->second] += m_curKeyVal;
					m_keyAxisStates[it->first] = true;
				}
				break;
			};
		}
		it++;
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

	if (m_mouseDownCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_mouseDownCallbacks.size(); i++)
		{
			m_mouseDownCallbacks[i](event);
		}
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

	if (m_mouseUpCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_mouseUpCallbacks.size(); i++)
		{
			m_mouseUpCallbacks[i](event);
		}
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

	if (m_joyDownCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_joyDownCallbacks.size(); i++)
		{
			m_joyDownCallbacks[i](event);
		}
	}
}

void CInputHandler::onJoystickButtonUp(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	m_buttonStates[whichOne][event.jbutton.button] = false;

	if (m_joyUpCallbacks.size() != 0)
	{
		for (size_t i = 0; i < m_joyUpCallbacks.size(); i++)
		{
			m_joyUpCallbacks[i](event);
		}
	}
}