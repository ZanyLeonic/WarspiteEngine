#include "Game.h"

// our Game object
Game* g_game = 0;

const int FPS = 62;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;

	std::cout << "Attempting Game initialization...\n";
	if (Game::Instance()->Init("Chapter 1", 100, 100, 640, 480, false))
	{
		while (Game::Instance()->IsRunning())
		{
			frameStart = SDL_GetTicks();

			Game::Instance()->HandleEvents();
			Game::Instance()->OnThink();
			Game::Instance()->Draw();

			frameTime = SDL_GetTicks() - frameStart;

			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}

			// SDL_Delay(10);
		}
	}
	else
	{
		std::cout << "Game Init failed! - " << SDL_GetError() << "\n";
		return -1;
	}
	std::cout << "Cleaning up...\n";
	Game::Instance()->Destroy();

	return 0;
}