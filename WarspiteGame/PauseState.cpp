#include "Game.h"
#include "Button.h"
#include "PauseState.h"

const std::string PauseState::s_UIID = "PAUSE";

void PauseState::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnThink();
	}
}

void PauseState::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

bool PauseState::OnPlay()
{
	if (!TextureManager::Instance()->Load("assets/ResumeButton.png",
		"resumeButton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	if (!TextureManager::Instance()->Load("assets/ExitButton.png",
		"exitButton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	Button* continueBtn = new Button(new ObjectParams(200, 80, 100, 32, "resumeButton"));
	Button* mainBtn = new Button(new ObjectParams(200, 120, 100, 32, "exitButton"));

	continueBtn->OnClick(s_continueGame);
	mainBtn->OnClick(s_exitToMenu);

	m_GameObjects.push_back(continueBtn);
	m_GameObjects.push_back(mainBtn);
}

bool PauseState::OnEnd()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}

	TextureManager::Instance()->Remove("resumeButton");
	TextureManager::Instance()->Remove("exitButton");

	return true;
}

void PauseState::s_continueGame()
{
	Game::Instance()->GetStateManager()->PopState();
}

void PauseState::s_exitToMenu()
{
	Game::Instance()->GetStateManager()->ModifyState(new MainMenuGameState());
}
