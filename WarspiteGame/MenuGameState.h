#pragma once
#ifndef __UI_GAMESTATE_H__
#define __UI_GAMESTATE_H__

#include "GameState.h"
#include "GameObject.h"
#include <vector>

class MenuGameState : public GameState
{
public:
	virtual void OnThink();
	virtual void Draw();

	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }

private:
	static const std::string s_UIID;

	std::vector<GameObject*> m_GameObjects;
};

#endif