#pragma once
#ifndef __BaseGame__
#define __BaseGame__

#include "IGame.h"

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "EngineTypes.h"
#include "GameStateManager.h"
#include "GarbageCollector.h"
#include "MainMenuState.h"
#include "Vector2D.h"

#define FRAME_SAMPLES 10

class CFPSCounter;
struct SDL_Rect;
class IWGame;

class CBaseGame : public IGame
{
public:
	static IGame* Instance();

private:
	CBaseGame() {}
	~CBaseGame() {}

	static CBaseGame* s_pInstance;

public:
	// simply set the running variable to true
	virtual bool Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int argc, char** argv, GameDLL_t pGameDLL);

private:
	// Calculate current FPS
	void FPS_Calc();
	int LoadGameDLL();
	void RunCallbackScript();

public:
	void Draw() override;
	void OnThink() override;
	void HandleEvents() override;
	void Destroy() override;
	void Quit() override;

	void FadeIn() override;
	void FadeOut() override;
	
	// a function to access the private running variable
	virtual bool IsRunning() { return m_bRunning; }

	SDL_Renderer* const GetRenderer() { return m_pRenderer; }
	SDL_Window* const GetWindow() { return m_pWindow; }
	CGameStateManager* const GetStateManager() { return m_pGameStateManager; }
	CGarbageCollector* const GetGarbageCollector() { return m_pGarbageCollector; }
	CCallbackHandler<HGenericCallback>* const GetCallbackHandler() { return m_pCallbackHandler; }

	CFadeObject* const GetFadeObject() { return m_pFadeObject; }

	CVector2D const GetViewportSize() { return m_viewportSize; }

	void SetPlayer(std::shared_ptr<IGameObject> pNO) { m_pPlayer = pNO; }
	std::shared_ptr<IGameObject> const GetPlayer() { return m_pPlayer; }

	void SetLoadedLevel(CLevel* level) { m_pLevel = level; }
	CLevel* GetLoadedLevel() { return m_pLevel; }
	
	int const GetArgc() { return m_argc; }
	char** const GetArgv() { return m_argv; }

	IWGame* const GetGameDLLClass() { return m_pGame; }

	bool const StartedWithMapParam() { return m_bStartedWithMapParam; }
	std::string const GetMapParamName() { return m_sMapName; }

	bool const ShowDebugTiles() { return m_bShowDebugTiles; }
	void SetDebugTilesVisibility(bool nVal) { m_bShowDebugTiles = nVal; }

private:
	CCallbackHandler<HGenericCallback>* m_pCallbackHandler;
	CGameStateManager* m_pGameStateManager;
	CGarbageCollector* m_pGarbageCollector;

	SDL_Window* m_pWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;

	SDL_Texture* m_pTexture = nullptr; // the new SDL_Texture variable
	SDL_Rect m_sourceRectangle = {}; // the first rectangle
	SDL_Rect m_destinationRectangle = {}; // another rectangle

	std::shared_ptr<IGameObject> m_pPlayer = nullptr;
	CLevel* m_pLevel = nullptr;
	
	CVector2D m_viewportSize;
	PGamePtr m_gamePtr;

	CFPSCounter* m_pFPSCounter;
	CFadeObject* m_pFadeObject;

	IWGame* m_pGame = nullptr;

	// FPS calculations
	Uint32 m_frametimes[FRAME_SAMPLES];
	Uint32 m_lastFrametime;
	Uint32 m_frameCount;
	float m_fFPS;

	int m_argc; 
	char** m_argv;

	bool m_bRunning = false;
	bool m_bStartedWithMapParam = false;

	bool m_bShowDebugTiles = true; // Whether we should show bound or player start tiles on levels

	std::string m_sMapName = "";
};
#endif /* defined(__Game__) */