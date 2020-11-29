#include "Game.h"
#include "Button.h"
#include "PauseState.h"
#include "StateParser.h"

bool CPauseState::OnPlay()
{
	CGameStateBase::OnPlay();

	s_UIID = SID_PAUSE;
	
	CStateParser sp;
	sp.ParseState(CEngineFileSystem::ResolvePath("SystemMenus.json", CEngineFileSystem::EPathType::STATE).c_str(), s_UIID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_continueGame);
	m_callbacks.push_back(s_exitToMenu);

	SetCallbacks(m_callbacks);

	return true;
}

bool CPauseState::OnEnd()
{
	return CGameStateBase::OnEnd();
}

bool CPauseState::s_continueGame()
{
	CBaseGame::Instance()->GetStateManager()->PopState();

	return false;
}

bool CPauseState::s_exitToMenu()
{
	CBaseGame::Instance()->GetStateManager()->ModifyState(std::move(CGameStateDictionary::Instance()->Create(SID_MM)));

	return false;
}