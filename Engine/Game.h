#pragma once
#ifndef __Game__
#define __Game__

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>

#include "EngineTypes.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "Vector2D.h"

#define FRAME_SAMPLES 10

class CFPSCounter;

class CGame
{
public:
	static CGame* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new CGame();
			return s_pInstance;
		}
		return s_pInstance;
	}
private:
	CGame() {}
	~CGame() {}
	
	static CGame* s_pInstance;
public:
	// simply set the running variable to true
	bool Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
private:
	// Calculate current FPS
	void FPS_Calc();
public:
	void Draw();
	void OnThink();
	void HandleEvents();
	void Destroy();
	void Quit();

	// a function to access the private running variable
	bool IsRunning() { return m_bRunning; }

	SDL_Renderer* GetRenderer() const { return m_pRenderer; }
	SDL_Window* GetWindow() const { return m_pWindow; }
	CGameStateManager* GetStateManager() const { return m_pGameStateManager; }

	CVector2D GetViewportSize() const { return m_viewportSize; }

	void SetPlayer(IGameObject* pNO) { m_player = pNO; }
	IGameObject* GetPlayer() const { return m_player; }
	
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