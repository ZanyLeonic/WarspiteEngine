#include "UIGameState.h"
#include <iostream>

const std::string UIGameState::s_UIID = "UI";

void UIGameState::OnThink()
{

}

void UIGameState::Draw()
{

}

bool UIGameState::OnPlay()
{
	std::cout << "Entered UIGameState\n";
	return true;
}

bool UIGameState::OnEnd()
{
	std::cout << "Exiting UIGameState\n";
	return true;
}