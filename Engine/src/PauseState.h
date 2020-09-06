#pragma once
#ifndef __UI_PLAYSTATE_H__
#define __UI_PLAYSTATE_H__

#include "MenuState.h"
#include <vector>

class CPauseState : public CMenuState {
public:
	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks);

	static bool s_continueGame();
	static bool s_exitToMenu();
};

REG_STATE_TO_REF(PauseState, CPauseState)
#endif