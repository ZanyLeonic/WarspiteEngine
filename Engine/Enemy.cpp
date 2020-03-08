#include "Enemy.h"

Enemy::Enemy()
	: WarspiteObject()
{
}

bool Enemy::OnThink()
{
	m_position.SetX(m_position.GetX() + 1);  
	m_position.SetY(m_position.GetY() + 1);
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));

	return true;
}

