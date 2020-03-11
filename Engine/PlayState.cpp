#include "TextureManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "TestObject.h"
#include "PauseState.h"
#include "StateParser.h"
#include "LevelParser.h"
#include <iostream>

#include "PlayState.h"

const std::string PlayState::s_playID = "Game";

bool PlayState::OnPlay()
{
	GameStateBase::OnPlay();

	StateParser sp;
	sp.ParseState("assets/states/PlayState.json", s_playID, &m_GameObjects, &m_TextureIDList);

	LevelParser lp;
	pLevel = lp.ParseLevel("assets/maps/help2.json");

	if (pLevel)
	{
		// Give the camera the Level size
		Camera::Instance()->SetLevelSize(pLevel->m_LevelSize);
	}

	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			Game::Instance()->GetStateManager()->PushState(new PauseState());
	});

	std::cout << "Entering PlayState\n";
	
	return true;
}

void PlayState::Draw()
{
	pLevel->Draw();
	// gameobjects and stuff
	GameStateBase::Draw();
}

void PlayState::OnThink()
{
	pLevel->OnThink();
	GameStateBase::OnThink();
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	GameStateBase::OnEnd();

	return true;
}