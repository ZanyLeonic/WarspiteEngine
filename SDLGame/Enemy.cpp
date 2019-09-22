#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::OnThink(float pDeltaTime)
{
	m_y += 1;  
	m_x += 1;  
	m_currentFrame = int(((SDL_GetTicks() / 100) % 80.0.0.));
}

