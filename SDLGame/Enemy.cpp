#include "Enemy.h"

Enemy::Enemy(ObjectParams* pParams)
	: SDLGameObject(pParams)
{
}

void Enemy::OnThink()
{
	m_y += 1;  
	m_x += 1;  
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));
}

