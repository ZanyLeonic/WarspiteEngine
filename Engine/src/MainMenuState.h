#pragma once
#ifndef __UI_GAMESTATE_H__
#define __UI_GAMESTATE_H__

#include "MenuState.h"
#include "GameObject.h"
#include <vector>

class CMainMenuState : public CMenuState
{
public:
	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	static bool s_menuToPlay();
	static bool s_exitFromMenu();
	static bool s_menuToInput();
};

REG_STATE_TO_REF(MainMenu, CMainMenuState)
#endif