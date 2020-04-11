#include "Game.h"
#include "TextureManager.h"
#include "Camera.h"
#include "InputHandler.h"
#include "LevelParser.h"
#include "GameObjectFactory.h"
#include "Button.h"
#include <iostream>

Game* Game::s_pInstance = 0;

// Initialises the major parts of the engine
bool Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	TheInputHandler::Instance()->InitialiseJoysticks();

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
		m_viewportSize = Vector2D(width, height);

		if (m_pWindow != 0)
		{
			std::cout << "Window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_SOFTWARE);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer,
					255, 255, 255, 255);

				GameObjectFactory::Instance()->RegisterType("Player", new PlayerCreator());
				GameObjectFactory::Instance()->RegisterType("Button", new ButtonCreator());
				GameObjectFactory::Instance()->RegisterType("TestObject", new TestObjectCreator());

				m_pGameStateManager = new GameStateManager();
				m_pGameStateManager->ModifyState(new MainMenuState());
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

void Game::Draw()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->Draw();

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::OnThink()
{
	// Call the current GameState functionality via the GameStateManager.
	m_pGameStateManager->OnThink();
	Camera::Instance()->OnThink();
}

void Game::HandleEvents()
{
	InputHandler::Instance()->OnThink();
}

void Game::Destroy()
{
	std::cout << "Cleaning Game instance...\n";

	InputHandler::Instance()->Destroy();
	m_pGameStateManager->PopState();

	// Destroy the Renderer before the window
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	SDL_Quit();
}

void Game::Quit()
{
	m_bRunning = false;
}
