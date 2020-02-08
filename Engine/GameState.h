#pragma once
#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include <string>

class GameState
{
public:
	virtual void OnThink() = 0;
	virtual void Draw() = 0;
	
	virtual bool OnPlay() = 0;
	virtual bool OnEnd() = 0;

	virtual std::string GetStateID() const = 0;
};

#endif
