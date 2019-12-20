#include "MenuGameState.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include <iostream>

const std::string MenuGameState::s_UIID = "MENU";

void MenuGameState::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnThink();
	}
}

void MenuGameState::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}

bool MenuGameState::OnPlay()
{
	if (!TextureManager::Instance()->Load("assets/PlayButton.png",
		"playbutton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	if (!TextureManager::Instance()->Load("assets/ExitButton.png",
		"exitbutton", Game::Instance()->GetRenderer()))
	{
		return false;
	}

	GameObject* button1 = new Button(new ObjectParams(100, 100, 100, 32, "playbutton"));
	GameObject* button2 = new Button(new ObjectParams(100, 300, 100, 32, "exitbutton"));

	m_GameObjects.push_back(button1);
	m_GameObjects.push_back(button2);

	std::cout << "Entered MenuGameState\n";
	return true;
}

bool MenuGameState::OnEnd()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();
	TextureManager::Instance()
		->Remove("playbutton");
	TextureManager::Instance()
		->Remove("exitbutton");

	std::cout << "Exiting MenuGameState\n";
	return true;
}