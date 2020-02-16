#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "TestObject.h"
#include "PauseState.h"
#include "StateParser.h"
#include <iostream>

#include "PlayState.h"

const std::string PlayState::s_playID = "Game";

bool PlayState::OnPlay()
{
	GameStateBase::OnPlay();

	StateParser sp;
	sp.ParseState("assets/states/PlayState.json", s_playID, &m_GameObjects, &m_TextureIDList);

	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			Game::Instance()->GetStateManager()->PushState(new PauseState());
	});

	std::cout << "Entering PlayState\n";
	
	return true;
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	GameStateBase::OnEnd();

	return true;
}