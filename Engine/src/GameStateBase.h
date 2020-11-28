#pragma once
#ifndef __GAMESTATEBASE_H__
#define __GAMESTATEBASE_H__

#include "GameState.h"
#include "GameObject.h"
#include "EngineFileSystem.h"
#include "GameStateDictionary.h"
#include "EngineTypes.h"

#include <vector>

// This part is where the IDs are defined
#define SID_MM "MainMenu"
#define SID_INPUT "InputState"
#define SID_PLAY "PlayState"
#define SID_PAUSE "PauseState"

class CGameStateBase :
	public CGameState
{
public:
	~CGameStateBase();

	void OnThink() override;
	void Draw() override;

	bool OnPlay() override;
	bool OnEnd() override;

	virtual std::vector<std::shared_ptr<IGameObject>> GetGameObjects() const { return m_GameObjects; }
	virtual std::vector<std::string> GetLoadedTextures() const { return m_TextureIDList; }
	virtual std::vector<std::string> GetLoadedScripts() const { return m_ScriptIDList; }
	
	virtual bool ShouldBeTicking() const { return m_shouldTick; }
	virtual bool ShouldBeDrawing() const { return m_shouldDraw; }
	
protected:
	std::vector<std::shared_ptr<IGameObject>> m_GameObjects;
	std::vector<std::string> m_TextureIDList;
	std::vector<std::string> m_ScriptIDList;

	bool m_shouldTick = false;
	bool m_shouldDraw = false;
	
	std::string s_UIID;
};
#endif