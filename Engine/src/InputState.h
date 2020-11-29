#pragma once
#ifndef __INPUTSTATE_H__
#define __INPUTSTATE_H__

#include "MenuState.h"
#include "EngineTypes.h"
#include <SDL2/SDL_scancode.h>

class CInputState
	: public CMenuState
{
public:
	CInputState();

	virtual bool OnPlay();
	virtual void Draw();
	virtual void OnThink();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	static bool s_InputToMainMenu();
	static std::string GetStringifiedMouseButton(int pMouseButton);

	std::vector<std::string> m_mouseActive = {};
	std::vector<std::string> m_keysActive = {};
	std::vector<std::string> m_joyActive = {};
};

REG_STATE_TO_REF(InputState, CInputState)
#endif