#pragma once
#ifndef __BaseGame__
#define __BaseGame__

#include "IGame.h"
#include <memory>
#include <vector>

#include "EngineTypes.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "Vector2D.h"

#define FRAME_SAMPLES 10

class CFPSCounter;
struct SDL_Rect;

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

public:
	void Draw() override;
	void OnThink() override;
	void HandleEvents() override;
	void Destroy() override;
	void Quit() override;

	// a function to access the private running variable
	virtual bool IsRunning() override { return m_bRunning; }

	virtual SDL_Renderer* const GetRenderer() override { return m_pRenderer; }
	virtual SDL_Window* const GetWindow() override { return m_pWindow; }
	virtual CGameStateManager* const GetStateManager() override { return m_pGameStateManager; }

	virtual CVector2D const GetViewportSize() override { return m_viewportSize; }

	virtual void SetPlayer(IGameObject* pNO) { m_player = pNO; }
	virtual IGameObject* const GetPlayer() override { return m_player; }
	
private:
	CGameStateManager* m_pGameStateManager;

	SDL_Window* m_pWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;

	SDL_Texture* m_pTexture = nullptr; // the new SDL_Texture variable
	SDL_Rect m_sourceRectangle = {}; // the first rectangle
	SDL_Rect m_destinationRectangle = {}; // another rectangle

	IGameObject* m_player = nullptr;
	
	CVector2D m_viewportSize;
	PGamePtr m_gamePtr;

	CFPSCounter* m_fpsCounter;

	// FPS calculations
	Uint32 m_frametimes[FRAME_SAMPLES];
	Uint32 m_lastFrametime;
	Uint32 m_frameCount;
	float m_FPS;

	bool m_bRunning = false;
};
#endif /* defined(__Game__) */