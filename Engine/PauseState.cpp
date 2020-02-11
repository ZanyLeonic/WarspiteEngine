#include "Game.h"
#include "Button.h"
#include "PauseState.h"

const std::string PauseState::s_UIID = "PAUSE";

bool PauseState::OnPlay()
{
	GameStateBase::OnPlay();

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
	Button* mainBtn = new Button(new ObjectParams(200, 200, 100, 32, "exitButton"));

	continueBtn->OnClick(s_continueGame);
	mainBtn->OnClick(s_exitToMenu);

	m_GameObjects.push_back(continueBtn);
	m_GameObjects.push_back(mainBtn);

	return true;
}

bool PauseState::OnEnd()
{
	TextureManager::Instance()->Remove("resumeButton");
	TextureManager::Instance()->Remove("exitButton");

	return GameStateBase::OnEnd();
}

bool PauseState::s_continueGame()
{
	InputHandler::Instance()->SetReleaseState(SDL_SCANCODE_ESCAPE, true);
	Game::Instance()->GetStateManager()->PopState();

	return false;
}

bool PauseState::s_exitToMenu()
{
	Game::Instance()->GetStateManager()->ModifyState(new MainMenuGameState());

	return false;
}
