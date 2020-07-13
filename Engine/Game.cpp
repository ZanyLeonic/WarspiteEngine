#include "Game.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "Camera.h"
#include "InputHandler.h"
#include "LevelParser.h"
#include <iostream>

CGame* CGame::s_pInstance = 0;

int audioLoop(void* data)
{
	// Just create an instance to initialise it.
	CSoundManager::Instance();
	
	CGame* pG = reinterpret_cast<CGame*>(data);

	while (pG->IsRunning())
	{
		CSoundManager::Instance()->OnThink();

		// SDL_WaitThread(pG->GetAudioThread(), NULL);
	}

	return 0;
}

// Initialises the major parts of the engine
bool CGame::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	CInputHandler::Instance()->InitialiseJoysticks();

	// attempt to initalize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		int flags = 0;

		if (fullscreen)
		{
			flags = SDL_WINDOW_FULLSCREEN;
		}

		std::cout << "SDL init success\n";
		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		m_viewportSize = CVector2D((float)width, (float)height);

		if (m_pWindow != 0)
		{
			std::cout << "Window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_SOFTWARE);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer,
					255, 255, 255, 255);

				SDL_Thread* aT = SDL_CreateThread(audioLoop, "AudioThread", (void*)Instance());
				SDL_DetachThread(aT);

				m_pGameStateManager = new CGameStateManager();
				m_pGameStateManager->ModifyState(new CMainMenuState());

				// Start the script manager
				CScriptManager::Instance();
			}
			else
			{
				std::cout << "Render init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "SDL init fail\n";
			return false; // SDL init fail
		}

		std::cout << "Init success\n";
		m_bRunning = true; // everything inited successfully, start the main loop

		return true;
	}
	return false;
}

void CGame::Draw()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->Draw();

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void CGame::OnThink()
{
	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->OnThink();
	
	CCamera::Instance()->OnThink();
}

void CGame::HandleEvents()
{
	CInputHandler::Instance()->OnThink();
}

void CGame::Destroy()
{
	std::cout << "Cleaning Game instance...\n";

	CInputHandler::Instance()->Destroy();
	CSoundManager::Instance()->Destroy();
	m_pGameStateManager->PopState();

	// Destroy the Renderer before the window
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	SDL_Quit();
}

void CGame::Quit()
{
	m_bRunning = false;
}

const char* CGame::TestMethod()
{
	std::cout << "This will be called by Python..." << std::endl;
	return "...and we will obey.";
}
