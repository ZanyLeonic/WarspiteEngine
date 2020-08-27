#pragma once
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include <vector>
#include "IGameStateManager.h"

class CGameStateBase;

class CGameStateManager : public IGameStateManager
{
public:
	CGameStateBase* GetCurrentState();

	virtual void PushState(CGameStateBase* pState);
	virtual void ModifyState(CGameStateBase* pState);

	virtual void PopState();

	virtual void Draw();
	virtual void OnThink();

private:
	std::vector<CGameStateBase*> m_GameStates;
};

#endif