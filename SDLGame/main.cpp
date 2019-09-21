#include "Game.h"

// our Game object
Game* g_game = 0;

int main(int argc, char* argv[])
{
	g_game = new Game();

	g_game->Init("Chapter 1", 100, 100, 640, 480, false);
	//g_game->init("Chapter 1", 100, 100, 640, 580, SDL_WINDOW_FULLSCREEN);

	while (g_game->IsRunning())
	{
		g_game->HandleEvents();
		g_game->Think();
		g_game->Draw();
	}
	g_game->Destroy();

	return 0;
}