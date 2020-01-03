#include "PlayState.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

void PlayState::OnThink() 
{

}

void PlayState::Draw()
{


}

bool PlayState::OnPlay()
{
	std::cout << "Entering PlayState\n";

	if (!TextureManager::Instance()->Load("assets/player.png", "player", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	return true;
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";
	return true;
}