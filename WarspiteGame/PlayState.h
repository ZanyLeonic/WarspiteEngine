#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameState.h"
class PlayState :
	public GameState
{
public:
	virtual void OnThink();
	virtual void Draw();

	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_playID; }

private:
	static const std::string s_playID;

	std::vector<GameObject*> m_GameObjects;
};
#endif
