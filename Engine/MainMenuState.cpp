#include "MainMenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include <iostream>

const std::string MainMenuState::s_UIID = "MENU";

bool MainMenuState::OnPlay()
{
	GameStateBase::OnPlay();

	// Parse the state
	StateParser sp;
	sp.ParseState("assets/Test.json", "MainMenu", &m_GameObjects, &m_TextureIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);

	SetCallbacks(m_callbacks);

	std::cout << "Entered MainMenuState\n";
	return true;
}

bool MainMenuState::OnEnd()
{
	TextureManager::Instance()
		->Remove("playbutton");
	TextureManager::Instance()
		->Remove("exitbutton");

	GameStateBase::OnEnd();

	std::cout << "Exiting MainMenuState\n";
	return true;
}

void MainMenuState::SetCallbacks(const std::vector<ButtonCallback>& callbacks)
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (dynamic_cast<Button*>(m_GameObjects[i]))
		{
			Button* pButton =
				dynamic_cast<Button*>(m_GameObjects[i]);

			pButton->OnClick(callbacks[pButton->GetOnClickID()]);
			pButton->OnEnter(callbacks[pButton->GetOnEnterID()]);
			pButton->OnLeave(callbacks[pButton->GetOnLeaveID()]);
		}
	}
}

bool MainMenuState::s_menuToPlay()
{
	std::cout << "Play button clicked\n";
	Game::Instance()->GetStateManager()->ModifyState(new PlayState());
	return false;
}

bool MainMenuState::s_exitFromMenu()
{
	std::cout << "Exit button clicked\n";
	Game::Instance()->Quit();
	return false;
}
