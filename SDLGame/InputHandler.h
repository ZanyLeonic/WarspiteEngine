#pragma once
#ifndef __InputHandler__
#define __InputHandler__

#include <SDL.h>
#include <vector>
#include "Vector2D.h"

enum MouseButtons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

class InputHandler
{
public:
	static InputHandler* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new InputHandler();
		}

		return s_pInstance;
	}

	void OnThink();
	void Destroy();

	void InitialiseJoysticks();
	bool JoysticksInitialised() { return m_bJoysticksInitialised; }

	int GetXAxis(int joy, int stick);
	int GetYAxis(int joy, int stick);

	bool GetButtonState(int joy, int buttonNumber) { return m_buttonStates[joy][buttonNumber]; }
	bool GetMouseButtonState(int buttonNumber) { return m_mouseButtonStates[buttonNumber]; }

private:

	InputHandler();
	~InputHandler();

	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialised;

	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
	std::vector<std::vector<bool>> m_buttonStates;

	std::vector<bool> m_mouseButtonStates;

	const int m_joystickDeadZone = 10000;

	static InputHandler* s_pInstance;
};
typedef InputHandler TheInputHandler;

#endif /* defined(__InputHandler__) */