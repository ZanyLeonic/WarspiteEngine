#include "Game.h"
#include <iostream>

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
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

				m_textureManager.load("assets/demongirl.bmp", "demongirl", m_pRenderer);
				m_textureManager.load("assets/sonic_test.png", "animate", m_pRenderer);
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

void Game::render()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	m_textureManager.draw("demongirl", 0, 0, 173, 241, m_pRenderer);
	
	m_textureManager.drawFrame("animate", 300, 300, 40, 40, 1, m_currentFrame, m_pRenderer);

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::tick()
{
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
}

void Game::handleEvents()
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

void Game::clean()
{
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}
