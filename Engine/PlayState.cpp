#include "TextureManager.h"
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

	// Execute the OnPlay method on all the GameObjects in all Object Layers
	std::vector<Layer*> objs = *pLevel->GetLayers();

	for (int i = 0; i < objs.size(); i++)
	{
		ObjectLayer* obl = dynamic_cast<ObjectLayer*>(objs[i]);
		
//		if (typeid(*obl) != typeid(ObjectLayer*)) continue;

		if (!obl) continue;

		std::vector<GameObject*>& sGo = *obl->GetGameObjects();

		for (int j = 0; j < sGo.size(); j++)
		{
			sGo[j]->OnPlay();
		}
	}

	TextureManager::Instance()->CreateCheckboardPattern(Vector2D(256, 256), Game::Instance()->GetRenderer());
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 255, 255, 255, 255);


	std::cout << "Entering PlayState\n";
	
	return true;
}

void PlayState::Draw()
{
	TextureManager::Instance()->Draw("test", 0, 0, 256, 256, Game::Instance()->GetRenderer());

	//pLevel->Draw();
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