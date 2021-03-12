#include "Enemy.h"
#include <SDL.h>

CEnemy::CEnemy()
	: CWarspiteObject()
{
}

bool CEnemy::OnThink()
{
	m_vPosition.SetX(m_vPosition.GetX() + 1);  
	m_vPosition.SetY(m_vPosition.GetY() + 1);
	m_iCurrentFrame = int(((SDL_GetTicks() / 100) % 8));

	return true;
}

