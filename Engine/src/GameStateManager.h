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
	virtual const std::string GetCurrentStateID();

	virtual void PushState(std::unique_ptr<CGameStateBase> pState);
	virtual void ModifyState(std::unique_ptr<CGameStateBase> pState);

	virtual void PopState();

	virtual void Draw();
	virtual void OnThink();

private:
	std::vector<std::unique_ptr<CGameStateBase>> m_GameStates;
};

#endif