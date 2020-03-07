#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameStateBase.h"
#include "Level.h"

class PlayState :
	public GameStateBase
{
public:
	virtual bool OnPlay();
	virtual void Draw();
	virtual void OnThink();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_playID; }

private:
	static const std::string s_playID;
	Level* pLevel;
};
#endif
