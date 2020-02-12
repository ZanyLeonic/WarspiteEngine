#include "MainMenuGameState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include <iostream>

const std::string MainMenuGameState::s_UIID = "MENU";

bool MainMenuGameState::OnPlay()
{
	GameStateBase::OnPlay();

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

	//m_GameObjects.push_back(new Button(new ObjectParams(200, 80, 100, 32, "playbutton"), s_menuToPlay));
	//m_GameObjects.push_back(new Button(new ObjectParams(200, 120, 100, 32, "exitbutton"), s_exitFromMenu));

	std::cout << "Entered MenuGameState\n";
	return true;
}

bool MainMenuGameState::OnEnd()
{
	TextureManager::Instance()
		->Remove("playbutton");
	TextureManager::Instance()
		->Remove("exitbutton");

	GameStateBase::OnEnd();

	std::cout << "Exiting MenuGameState\n";
	return true;
}

bool MainMenuGameState::s_menuToPlay()
{
	std::cout << "Play button clicked\n";
	Game::Instance()->GetStateManager()->ModifyState(new PlayState());
	return false;
}

bool MainMenuGameState::s_exitFromMenu()
{
	std::cout << "Exit button clicked\n";
	Game::Instance()->Quit();
	return false;
}
