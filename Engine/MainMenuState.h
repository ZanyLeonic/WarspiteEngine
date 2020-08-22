#pragma once
#ifndef __UI_GAMESTATE_H__
#define __UI_GAMESTATE_H__

#include "MenuState.h"
#include "GameObject.h"
#include <vector>

class EXPORT CMainMenuState : public CMenuState
{
public:
	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks);

	static bool s_menuToPlay();
	static bool s_exitFromMenu();
};

REG_STATE_TO_REF(MainMenu, CMainMenuState)
#endif