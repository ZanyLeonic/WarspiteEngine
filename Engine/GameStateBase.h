#pragma once
#ifndef __GAMESTATEBASE_H__
#define __GAMESTATEBASE_H__

#include "GameState.h"
#include "GameObject.h"
#include <vector>
class GameStateBase :
	public GameState
{
public:

	virtual void OnThink();
	virtual void Draw();

	virtual bool OnPlay();
	virtual bool OnEnd();

	virtual inline std::vector<GameObject*> GetGameObjects()
	{
		return m_GameObjects;
	}

	virtual inline bool ShouldBeTicking()
	{
		return m_shouldTick;
	};
	virtual inline bool ShouldBeDrawing()
	{
		return m_shouldDraw;
	};

protected:
	std::vector<GameObject*> m_GameObjects;
	std::vector<std::string> m_TextureIDList;

	bool m_shouldTick = false;
	bool m_shouldDraw = false;
};

#endif