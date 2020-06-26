#include "MainMenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include <iostream>

const std::string MainMenuState::s_UIID = "MainMenu";

bool MainMenuState::OnPlay()
{
	MenuState::OnPlay();

	// Parse the state
	StateParser sp;
	sp.ParseState("assets/states/SystemMenus.json", s_UIID, &m_GameObjects, &m_TextureIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);

	SetCallbacks(m_callbacks);

	std::cout << "Entered MainMenuState\n";
	return true;
}

bool MainMenuState::OnEnd()
{
	MenuState::OnEnd();

	std::cout << "Exiting MainMenuState\n";
	return true;
}

void MainMenuState::SetCallbacks(const std::vector<ButtonCallback>& callbacks)
{
	// MenuState::SetCallbacks(callbacks);

	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (dynamic_cast<CButton*>(m_GameObjects[i]))
		{
			CButton* pButton =
				dynamic_cast<CButton*>(m_GameObjects[i]);

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
