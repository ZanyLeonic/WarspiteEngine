#include "MainMenuState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include <iostream>

bool CMainMenuState::OnPlay()
{
	CMenuState::OnPlay();
	
	s_UIID = SID_MM;
	
	// Parse the state
	CStateParser sp;
	sp.ParseState(CEngineFileSystem::ResolvePath("SystemMenus.json", EPathType::STATE).c_str(), s_UIID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	for(size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}
	
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);

	SetCallbacks(m_callbacks);

	std::cout << "Entered MainMenuState\n";
	return true;
}

bool CMainMenuState::OnEnd()
{
	CMenuState::OnEnd();

	std::cout << "Exiting MainMenuState\n";
	return true;
}

void CMainMenuState::SetCallbacks(const std::vector<HButtonCallback>& callbacks)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
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

bool CMainMenuState::s_menuToPlay()
{
	std::cout << "Play button clicked\n";
	CGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_PLAY));
	return false;
}

bool CMainMenuState::s_exitFromMenu()
{
	std::cout << "Exit button clicked\n";
	CGame::Instance()->Quit();
	return false;
}