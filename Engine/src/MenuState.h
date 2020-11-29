#pragma once
#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__
#include "GameStateBase.h"
#include "EngineTypes.h"
#include <vector>
class CMenuState : public CGameStateBase
{
protected:
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks);

	std::vector<HButtonCallback> m_callbacks;
};

#endif