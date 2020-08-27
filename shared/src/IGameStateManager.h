#pragma once
#ifndef __IGAMESTATEMANAGER_H__
#define __IGAMESTATEMANAGER_H__

#include "EngineTypes.h"

class CGameStateBase;

class IGameStateManager
{
public:
	virtual CGameStateBase* GetCurrentState() = 0;

	virtual void PushState(CGameStateBase* pState) = 0;
	virtual void ModifyState(CGameStateBase* pState) = 0;

	virtual void PopState() = 0;

	virtual void Draw() = 0;
	virtual void OnThink() = 0;
};

#endif