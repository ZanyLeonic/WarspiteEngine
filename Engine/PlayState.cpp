#include "TextureManager.h"
#include "Game.h"
#include "Player.h"
#include "TestObject.h"
#include "PauseState.h"
#include <iostream>

#include "PlayState.h"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::OnPlay()
{
	GameStateBase::OnPlay();

	std::cout << "Entering PlayState\n";

	if (!TextureManager::Instance()->Load("assets/dev/Black_22x27.bmp", "testObj", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	if (!TextureManager::Instance()->Load("assets/player.png", "player", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	////m_GameObjects.push_back(new TestObject(new ObjectParams(100, 90, 22, 27, "")));
	//m_GameObjects.push_back(new TestObject(new ObjectParams(100, 100, 22, 27, "testObj")));
	//m_GameObjects.push_back(new Player(new ObjectParams(0, 0, 22, 27, "player")));
	//
	//InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
	//	Game::Instance()->GetStateManager()->PushState(new PauseState());
	//	});

	return true;
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	GameStateBase::OnEnd();
	TextureManager::Instance()->Remove("player");

	return true;
}