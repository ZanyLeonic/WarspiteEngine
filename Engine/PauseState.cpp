#include "Game.h"
#include "Button.h"
#include "PauseState.h"
#include "StateParser.h"

const std::string PauseState::s_UIID = "PauseMenu";

bool PauseState::OnPlay()
{
	GameStateBase::OnPlay();

	StateParser sp;
	sp.ParseState("assets/states/SystemMenus.json", s_UIID, &m_GameObjects, &m_TextureIDList);

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_continueGame);
	m_callbacks.push_back(s_exitToMenu);

	SetCallbacks(m_callbacks);

	return true;
}

bool PauseState::OnEnd()
{
	return GameStateBase::OnEnd();
}

void PauseState::SetCallbacks(const std::vector<ButtonCallback>& callbacks)
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


bool PauseState::s_continueGame()
{
	Game::Instance()->GetStateManager()->PopState();

	return false;
}

bool PauseState::s_exitToMenu()
{
	Game::Instance()->GetStateManager()->ModifyState(new MainMenuState());

	return false;
}
