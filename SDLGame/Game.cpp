#include "Game.h"
#include "TextureManager.h"
#include <iostream>

bool Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
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

		if (m_pWindow != 0)
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(m_pRenderer,
					255, 255, 255, 255);

				// m_textureManager.load("assets/demongirl.bmp", "demongirl", m_pRenderer);
				// TextureManager::Instance()->load("assets/sonic_test.png", "animate", m_pRenderer);
				m_bo.Load(100, 100, 40, 40, "animate");
				m_player.Load(300, 300, 40, 40, "animate");

				if (!TextureManager::TheTextureManager::Instance()->load("assets/demongirl.bmp", "demongirl", m_pRenderer))
				{
					return false;
				}

				if (!TextureManager::TheTextureManager::Instance()->load("assets/P6290084.JPG", "rhys", m_pRenderer))
				{
					return false;
				}

				if (!TextureManager::TheTextureManager::Instance()->load("assets/sonic_test.png", "animate", m_pRenderer))
				{
					return false;
				}
			}
			else
			{
				std::cout << "render init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "SDL init fail\n";
			return false; // SDL init fail
		}

		std::cout << "init success\n";
		m_bRunning = true; // everything inited successfully, start the main loop

		return true;
	}
	return false;
}

void Game::Draw()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	// TextureManager::TheTextureManager::Instance()->draw("demongirl", 0, 0, 173, 241, m_pRenderer);
	// TextureManager::TheTextureManager::Instance()->draw("rhys", 0, 0, 640, 480, m_pRenderer);
	// TextureManager::TheTextureManager::Instance()->drawFrame("animate", 300, 300, 40, 40, 1, m_currentFrame, m_pRenderer);
	m_bo.Draw(m_pRenderer);
	m_player.Draw(m_pRenderer);

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::OnThink()
{
	// m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
	m_bo.OnThink(0);
	m_player.OnThink(0);
}

void Game::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;

		default:
			break;
		}
	}
}

void Game::Destroy()
{
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
