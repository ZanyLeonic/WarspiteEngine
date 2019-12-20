#pragma once
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include "GameState.h"
#include <vector>

class GameStateManager
{
public:

	void PushState(GameState* pState);
	void ModifyState(GameState* pState);

	void PopState();

	void Draw();
	void OnThink();

private:
	std::vector<GameState*> m_GameStates;
};

#endif

