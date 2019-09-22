#include "Player.h"
#include <iostream>

Player::Player()
{
}

void Player::OnThink(float pDeltaTime)
{
	//GameObject::OnThink();
	//std::cout << "Player::OnThink()";
	//m_x = 10;
	//m_y = 20;
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
	m_x -= 1;
}
