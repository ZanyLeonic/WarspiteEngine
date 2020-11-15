#pragma once
#ifndef __INPUTSTATE_H__
#define __INPUTSTATE_H__

#include "MenuState.h"
#include "EngineTypes.h"
#include <SDL_Scancode.h>

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
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks);

	static bool s_InputToMainMenu();

	//void OnKeyDown(SDL_Scancode e);
	//void OnKeyUp(SDL_Scancode e);

	std::vector<std::string> m_keysActive = {};
};

REG_STATE_TO_REF(InputState, CInputState)
#endif