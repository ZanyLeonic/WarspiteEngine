#include "Game.h"
#include "GitVersion.h"
#include "StateParser.h"

// our Game object
Game* g_game = 0;

const int FPS = 62;
const int DELAY_TIME = 1000 / FPS;

int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;

	char title[420];

#ifndef WARDEBUG
	snprintf(title, sizeof(title), "Engine (Build: %d git: %s)", GAME_BUILD_NUMBER, GAME_GIT_HASH);
#else
	snprintf(title, sizeof(title), "Engine (DEBUG) (Build: %d git: %s)", GAME_BUILD_NUMBER, GAME_GIT_HASH);
	std::cout << "This is debug build.\n";
#endif
	
	std::cout << "Build: "<< GAME_BUILD_NUMBER << "\nUsing source: " << GAME_GIT_HASH << "\n";
	std::cout << "Attempting Game initialization...\n";
	std::cout << "Target FPS is " << FPS << " FPS\n";

	if (Game::Instance()->Init(title, 100, 100, 1280, 720, false))
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
		std::cout << "Engine Error (Game Init failed!) - " << SDL_GetError() << "\n";
		return -1;
	}
	std::cout << "Cleaning up...\n";
	Game::Instance()->Destroy();

	return 0;
}