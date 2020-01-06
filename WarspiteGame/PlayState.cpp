#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include <iostream>

#include "PlayState.h"

const std::string PlayState::s_playID = "PLAY";

void PlayState::OnThink() 
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnThink();
	}
}

void PlayState::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

bool PlayState::OnPlay()
{
	std::cout << "Entering PlayState\n";

	if (!TextureManager::Instance()->Load("assets/player.png", "player", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	m_GameObjects.push_back(new Player(new ObjectParams(0, 0, 22, 27, "player")));

	return true;
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();
	TextureManager::Instance()->Remove("player");

	return true;
}