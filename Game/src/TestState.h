#pragma once
#ifndef __TESTSTATE_H__
#define __TESTSTATE_H__

#include "GameStateBase.h"
#include "EngineTypes.h"

class CTestState :
	public CGameStateBase
{
public:
	virtual bool OnPlay();
	virtual void Draw();
	virtual void OnThink();
	virtual bool OnEnd();

	virtual std::string GetStateID() const { return s_UIID; }
};

REG_STATE_TO_REF(TestState, CTestState)
#endif