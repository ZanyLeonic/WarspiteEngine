#pragma once
#include "GameStateBase.h"
#include <functional>
#include <vector>
class MenuState : public GameStateBase
{
protected:
	typedef std::function<bool()> ButtonCallback;
	
	virtual void SetCallbacks(const std::vector<ButtonCallback>& callbacks) = 0;

	std::vector<ButtonCallback> m_callbacks;
};

