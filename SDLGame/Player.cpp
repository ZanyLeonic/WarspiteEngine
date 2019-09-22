#include "Player.h"
#include <iostream>

Player::Player()
{
}

void Player::Load(int x, int y, int width, int height, std::string textureID)
{
	GameObject::Load(x, y, width, height, textureID);
}

void Player::Draw(SDL_Renderer* pRenderer)
{
	GameObject::Draw(pRenderer);
	std::cout << "Player::Draw()";
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

void Player::Destroy()
{
	GameObject::Destroy();
	std::cout << "Player::Destroy()";
}
