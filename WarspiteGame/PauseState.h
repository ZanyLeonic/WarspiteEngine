#pragma once
#ifndef __UI_PLAYSTATE_H__
#define __UI_PLAYSTATE_H__

#include "GameStateBase.h"
#include "GameObject.h"
#include <vector>

class PauseState : public GameStateBase {
public:
	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	static const std::string s_UIID;

	static bool s_continueGame();
	static bool s_exitToMenu();
};

#endif
