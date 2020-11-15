#include "MainMenuState.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include <spdlog/spdlog.h>

bool CMainMenuState::OnPlay()
{
	CMenuState::OnPlay();
	
	s_UIID = SID_MM;
	
	SDL_SetRenderDrawColor(CBaseGame::Instance()->GetRenderer(), 255, 255, 255, 255);

	// Parse the state
	CStateParser::ParseState(CEngineFileSystem::ResolvePath("SystemMenus.json", CEngineFileSystem::EPathType::STATE).c_str(), s_UIID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	for(size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}
	
	m_callbacks.push_back(0);
	m_callbacks.push_back(s_menuToPlay);
	m_callbacks.push_back(s_exitFromMenu);
	m_callbacks.push_back(s_menuToInput);

	SetCallbacks(m_callbacks);

	spdlog::info("Entered MainMenuState");
	return true;
}

bool CMainMenuState::OnEnd()
{
	CMenuState::OnEnd();

	m_callbacks.clear();

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

			// So we don't crash when we try add something new.
			size_t cbSize = m_callbacks.size() - 1;

			if (pButton->GetOnClickID() > cbSize)
			{
				spdlog::warn("OnClickID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnClick(callbacks[pButton->GetOnClickID()]);
			}

			if (pButton->GetOnEnterID() > cbSize)
			{
				spdlog::warn("OnEnterID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnEnter(callbacks[pButton->GetOnEnterID()]);
			}

			if (pButton->GetOnLeaveID() > cbSize)
			{
				spdlog::warn("OnLeaveID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnLeave(callbacks[pButton->GetOnLeaveID()]);
			}
		}
	}
}

bool CMainMenuState::s_menuToPlay()
{
	spdlog::info("Play button clicked");
	CBaseGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_PLAY));
	return true;
}

bool CMainMenuState::s_exitFromMenu()
{
	spdlog::info("Exit button clicked");
	CBaseGame::Instance()->Quit();
	return true;
}

bool CMainMenuState::s_menuToInput()
{
	spdlog::info("Input button clicked");
	CBaseGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_INPUT));
	return true;
}