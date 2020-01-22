#pragma once
#ifndef __UI_PLAYSTATE_H__
#define __UI_PLAYSTATE_H__

#include "GameState.h"
#include "GameObject.h"
#include <vector>

class PauseState : public GameState {
public:
	virtual void OnThink();
	virtual void Draw();

	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	static const std::string s_UIID;

	static void s_continueGame();
	static void s_exitToMenu();

	std::vector<GameObject*> m_GameObjects;
};

#endif
