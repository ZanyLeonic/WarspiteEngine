#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameStateBase.h"
#include "Level.h"
#include "ScriptWrappers.h"

class CPlayState :
	public CGameStateBase
{
public:
	virtual bool OnPlay();
	virtual void Draw();
	virtual void OnThink();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_playID; }
	virtual CLevel* GetLoadedLevel() const { return pLevel; }

private:
	static const std::string s_playID;
	CLevel* pLevel;
	PLevelPtr te;

	CVector2D m_screenSize;
};
#endif
