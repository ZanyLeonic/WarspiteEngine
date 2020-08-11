#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameStateBase.h"
#include "Level.h"
#include "EngineTypes.h"

class CPlayState :
	public CGameStateBase
{
public:
	virtual bool OnPlay();
	virtual void Draw();
	virtual void OnThink();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }
	virtual CLevel* GetLoadedLevel() const { return pLevel; }

	CWarspiteObject* GetPlayer() const { return m_player; }
	
private:
	CLevel* pLevel;
	PLevelPtr m_levelPtr;

	CWarspiteObject* m_player;
	
	CVector2D m_screenSize;
};

REG_STATE_TO_REF(PlayState, CPlayState)
#endif