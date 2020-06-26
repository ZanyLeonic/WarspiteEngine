#pragma once
#include "GameStateBase.h"
#include <functional>
#include <vector>
class CMenuState : public CGameStateBase
{
protected:
	typedef std::function<bool()> HButtonCallback;
	
	virtual void SetCallbacks(const std::vector<HButtonCallback>& callbacks) = 0;

	std::vector<HButtonCallback> m_callbacks;
};

