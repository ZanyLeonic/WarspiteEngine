#include "Game.h"

// our Game object
Game* g_game = 0;

int main(int argc, char* argv[])
{
	std::cout << "Attempting Game initialization...\n";
	if (Game::Instance()->Init("Chapter 1", 100, 100, 640, 480, false))
	{
		while (Game::Instance()->IsRunning())
		{
			Game::Instance()->HandleEvents();
			Game::Instance()->OnThink();
			Game::Instance()->Draw();

			SDL_Delay(10);
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