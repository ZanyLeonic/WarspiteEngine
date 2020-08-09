#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameStateBase.h"
#include "Level.h"

struct SLevelObject;
typedef std::shared_ptr<SLevelObject>  PLevelPtr;

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

private:
	CLevel* pLevel;
	PLevelPtr m_levelPtr;
	
	CVector2D m_screenSize;
};

REG_STATE_TO_REF(PlayState, CPlayState)
#endif