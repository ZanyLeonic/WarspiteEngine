#pragma once
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include "GameStateBase.h"
#include <vector>

class GameStateManager
{
public:
	GameStateBase* GetCurrentState();

	void PushState(GameStateBase* pState);
	void ModifyState(GameStateBase* pState);

	void PopState();

	void Draw();
	void OnThink();

private:
	std::vector<GameStateBase*> m_GameStates;
};

#endif

