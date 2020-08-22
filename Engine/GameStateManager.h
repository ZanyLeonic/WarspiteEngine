#pragma once
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include <vector>
#include "EngineTypes.h"

class CGameStateBase;

class EXPORT CGameStateManager
{
public:
	CGameStateBase* GetCurrentState();

	void PushState(CGameStateBase* pState);
	void ModifyState(CGameStateBase* pState);

	void PopState();

	void Draw();
	void OnThink();

private:
	std::vector<CGameStateBase*> m_GameStates;
};

#endif