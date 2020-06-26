#include "Enemy.h"
#include <SDL.h>

CEnemy::CEnemy()
	: CWarspiteObject()
{
}

bool CEnemy::OnThink()
{
	m_position.SetX(m_position.GetX() + 1);  
	m_position.SetY(m_position.GetY() + 1);
	m_currentFrame = int(((SDL_GetTicks() / 100) % 8));

	return true;
}

