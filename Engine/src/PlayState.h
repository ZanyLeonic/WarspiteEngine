#pragma once
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include "GameStateBase.h"
#include "Level.h"
#include "EngineTypes.h"

struct SStreamingAudioData;

class CPlayState :
	public CGameStateBase
{
public:
	bool OnPlay() override;
	void Draw() override;
	void OnThink() override;
	bool OnEnd() override;

	std::string GetStateID() const { return s_UIID; }

	bool IsColliding(CVector2D v1) override;

private:
	PLevelPtr m_levelPtr;
	CVector2D m_screenSize;

	static void s_PlayCallback(SStreamingAudioData* as);
	static void s_PauseCallback(SStreamingAudioData* as);
	static void s_StopCallback(SStreamingAudioData* as);
};

REG_STATE_TO_REF(PlayState, CPlayState)
#endif