#pragma once
#include "GameStateBase.h"
#include "EngineTypes.h"
#include <vector>
class CMenuState : public CGameStateBase
{
protected:
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks) = 0;

	std::vector<HButtonCallback> m_callbacks;
};

