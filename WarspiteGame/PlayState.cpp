#include "PlayState.h"
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
	return true;
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";
	return true;
}