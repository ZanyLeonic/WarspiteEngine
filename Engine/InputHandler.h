#pragma once
#ifndef __InputHandler__
#define __InputHandler__

#include <SDL.h>
#include <vector>
#include <map>
#include <functional>
#include "Vector2D.h"

enum MouseButtons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

typedef std::function<void()> KeyCallback;

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

	float GetXAxis(int joy, int stick);
	float GetYAxis(int joy, int stick);

	bool GetButtonState(int joy, int buttonNumber) { return m_buttonStates[joy][buttonNumber]; }
	bool GetMouseButtonState(int buttonNumber) { return m_mouseButtonStates[buttonNumber]; }

	void AddActionKeyDown(SDL_Scancode key, KeyCallback callBack);
	void AddActionKeyUp(SDL_Scancode key, KeyCallback callBack);

	bool IsKeyDown(SDL_Scancode key);
	void SetReleaseState(SDL_Scancode key, bool state);

	Vector2D* GetMousePosition()
	{
		return m_mousePosition;
	}

private:

	InputHandler();
	~InputHandler();

	// private functions to handle the different event types

	// handle keyboard events
	void onKeyDown();
	void onKeyUp();

	// handle mouse events
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	// handle joystick events
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);

	static void keyDownTest();

	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialised;

	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
	std::vector<std::vector<bool>> m_buttonStates;

	std::map<SDL_Scancode, KeyCallback> m_keyDownCallbacks;
	std::map<SDL_Scancode, KeyCallback> m_keyUpCallbacks;

	std::map<SDL_Scancode, bool> m_keyReleased;

	std::vector<bool> m_mouseButtonStates;

	const int m_joystickDeadZone = 10000;
	
	Vector2D* m_mousePosition = new Vector2D(0,0);

	Uint8* m_keystates = 0;

	static InputHandler* s_pInstance;
};
typedef InputHandler TheInputHandler;

#endif /* defined(__InputHandler__) */