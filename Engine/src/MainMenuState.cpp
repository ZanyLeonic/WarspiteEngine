#include "MainMenuState.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"
#include "WarspiteUtil.h"
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

bool CMainMenuState::s_menuToPlay()
{
	spdlog::info("Play button clicked");
	CBaseGame::Instance()->GetStateManager()->ModifyState(std::move(CGameStateDictionary::Instance()->Create(SID_PLAY)));
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
	CBaseGame::Instance()->GetStateManager()->ModifyState(std::move(CGameStateDictionary::Instance()->Create(SID_INPUT)));
	return true;
}