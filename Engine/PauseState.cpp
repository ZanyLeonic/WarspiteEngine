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

void CPauseState::SetCallbacks(const std::vector<HButtonCallback>& callbacks)
{
	// MenuState::SetCallbacks(callbacks);

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


bool CPauseState::s_continueGame()
{
	CBaseGame::Instance()->GetStateManager()->PopState();

	return false;
}

bool CPauseState::s_exitToMenu()
{
	CBaseGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_MM));

	return false;
}
