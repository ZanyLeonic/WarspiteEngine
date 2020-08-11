#include "MainMenuState.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include <spdlog/spdlog.h>

bool CMainMenuState::OnPlay()
{
	CMenuState::OnPlay();
	
	s_UIID = SID_MM;
	
	// Parse the state
	CStateParser sp;
	sp.ParseState(CEngineFileSystem::ResolvePath("SystemMenus.json", CEngineFileSystem::EPathType::STATE).c_str(), s_UIID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	for(size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}
	
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);

	SetCallbacks(m_callbacks);

	spdlog::info("Entered MainMenuState");
	return true;
}

bool CMainMenuState::OnEnd()
{
	CMenuState::OnEnd();

	spdlog::info("Exiting MainMenuState");
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
	spdlog::info("Play button clicked");
	CGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_PLAY));
	return false;
}

bool CMainMenuState::s_exitFromMenu()
{
	spdlog::info("Exit button clicked");
	CGame::Instance()->Quit();
	return false;
}