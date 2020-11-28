#pragma once
#ifndef __IGAMESTATEMANAGER_H__
#define __IGAMESTATEMANAGER_H__

#include <memory>
#include "EngineTypes.h"

class CGameStateBase;

class IGameStateManager
{
public:
	virtual std::shared_ptr<CGameStateBase> GetCurrentState() = 0;

	virtual void PushState(std::shared_ptr<CGameStateBase> pState) = 0;
	virtual void ModifyState(std::shared_ptr<CGameStateBase> pState) = 0;

	virtual void PopState() = 0;

	virtual void Draw() = 0;
	virtual void OnThink() = 0;
};

#endif