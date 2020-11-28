#pragma once
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include <memory>
#include <vector>
#include "IGameStateManager.h"

class CGameStateBase;

class CGameStateManager : public IGameStateManager
{
public:
	std::shared_ptr<CGameStateBase> GetCurrentState();

	virtual void PushState(std::shared_ptr<CGameStateBase> pState);
	virtual void ModifyState(std::shared_ptr<CGameStateBase> pState);

	virtual void PopState();

	virtual void Draw();
	virtual void OnThink();

private:
	std::vector<std::shared_ptr<CGameStateBase>> m_GameStates;
};

#endif