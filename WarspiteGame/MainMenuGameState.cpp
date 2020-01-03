#include "MainMenuGameState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include <iostream>

const std::string MainMenuGameState::s_UIID = "MENU";

void MainMenuGameState::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnThink();
	}
}

void MainMenuGameState::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

bool MainMenuGameState::OnPlay()
{
	if (!TextureManager::Instance()->Load("assets/PlayButton.png",
		"playbutton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	if (!TextureManager::Instance()->Load("assets/ExitButton.png",
		"exitbutton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	m_GameObjects.push_back(new Button(new ObjectParams(100, 100, 100, 32, "playbutton"), s_menuToPlay));
	m_GameObjects.push_back(new Button(new ObjectParams(100, 300, 100, 32, "exitbutton"), s_exitFromMenu));

	std::cout << "Entered MenuGameState\n";
	return true;
}

bool MainMenuGameState::OnEnd()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();
	TextureManager::Instance()
		->Remove("playbutton");
	TextureManager::Instance()
		->Remove("exitbutton");

	std::cout << "Exiting MenuGameState\n";
	return true;
}

void MainMenuGameState::s_menuToPlay()
{
	std::cout << "Play button clicked\n";
	Game::Instance()->GetStateManager()->ModifyState(new PlayState());
}

void MainMenuGameState::s_exitFromMenu()
{
	std::cout << "Exit button clicked\n";
	Game::Instance()->Quit();
}
