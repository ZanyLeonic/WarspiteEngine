#include "Game.h"
#include "EngineMetadata.h"
#include <iostream>
#include <chrono>

// our Game object
CGame* g_game = 0;

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
	std::cout << "This is a debug build.\n";
#endif

	std::chrono::seconds dur((long)GAME_BUILD_TIME);
	std::chrono::time_point<std::chrono::system_clock> dt(dur);
	auto ptr = std::chrono::system_clock::to_time_t(dt);
	char time[255];
	auto frm = ctime_s(time, 255, &ptr);
	
	std::cout << "Build: "<< GAME_BUILD_NUMBER << "\nUsing source: " << GAME_GIT_HASH << "\n";
	std::cout << "Build Date: " << time << std::endl;
	std::cout << "Attempting Game initialization...\n";
	std::cout << "Target FPS is " << FPS << " FPS\n";

	if (CGame::Instance()->Init(title, 100, 100, 640, 480, false))
	{
		while (CGame::Instance()->IsRunning())
		{
			frameStart = SDL_GetTicks();

			CGame::Instance()->HandleEvents();
			CGame::Instance()->OnThink();
			CGame::Instance()->Draw();

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
	CGame::Instance()->Destroy();

	return 0;
}