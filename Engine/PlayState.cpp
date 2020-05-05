#include "TextureManager.h"
#include "SoundManager.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "TestObject.h"
#include "ObjectLayer.h"
#include "PauseState.h"
#include "StateParser.h"
#include "LevelParser.h"
#include <iostream>

#include "PlayState.h"

const std::string PlayState::s_playID = "Game";

bool PlayState::OnPlay()
{
	GameStateBase::OnPlay();

	m_screenSize = Game::Instance()->GetViewportSize();

	StateParser sp;
	sp.ParseState("assets/states/PlayState.json", s_playID, &m_GameObjects, &m_TextureIDList);

	LevelParser lp;
	pLevel = lp.ParseLevel("assets/maps/help2.json");

	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_ESCAPE, [this] {
			if (!dynamic_cast<PauseState*>(Game::Instance()->GetStateManager()->GetCurrentState()))
			{
				Game::Instance()->GetStateManager()->PushState(new PauseState());
			}
		});

	InputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_ESCAPE, [this] {
			return;
		});

	InputHandler::Instance()->AddActionKeyDown(SDL_SCANCODE_0, [this] {
			SoundManager::Instance()->Test();
		});
	InputHandler::Instance()->AddActionKeyUp(SDL_SCANCODE_0, [this] {
			return;
		});

	if (pLevel)
	{
		// Give the camera the Level size
		Camera::Instance()->SetLevelSize(pLevel->m_LevelSize);

		// Execute the OnPlay method on all the GameObjects in all Object Layers
		pLevel->OnPlay();
	}
	else
	{ 
		// If a failure happens when attempting to load - this will let us know something is up.
		TextureManager::Instance()->CreateCheckboardPattern(m_screenSize, "levelLoadFail", Game::Instance()->GetRenderer());
		SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 255);
	}

	std::cout << "Entering PlayState\n";
	
	return true;
}

void PlayState::Draw()
{
	if (pLevel != 0)
	{
		pLevel->Draw();
	}
	else
	{
		TextureManager::Instance()->Draw("levelLoadFail", 0, 0, (int)m_screenSize.GetX(), (int)m_screenSize.GetY(), Game::Instance()->GetRenderer());
	}
	// gameobjects and stuff
	GameStateBase::Draw();
}

void PlayState::OnThink()
{
	if (pLevel != 0)
		pLevel->OnThink();

	GameStateBase::OnThink();
}

bool PlayState::OnEnd()
{
	std::cout << "Exiting PlayState\n";

	// Execute the OnPlay method on all the GameObjects in all Object Layers
	if (pLevel != 0)
		pLevel->Destroy();

	GameStateBase::OnEnd();

	return true;
}