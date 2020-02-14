#pragma once
#ifndef __UI_GAMESTATE_H__
#define __UI_GAMESTATE_H__

#include "MenuState.h"
#include "GameObject.h"
#include <vector>

class MainMenuState : public MenuState
{
public:
	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	virtual void SetCallbacks(const std::vector<ButtonCallback>& callbacks);

	static const std::string s_UIID;

	static bool s_menuToPlay();
	static bool s_exitFromMenu();
};

#endif