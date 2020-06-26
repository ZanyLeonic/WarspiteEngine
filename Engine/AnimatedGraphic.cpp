#include "AnimatedGraphic.h"
#include <SDL.h>

void CAnimatedGraphic::Load(const CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	m_animSpeed = pParams->GetAnimSpeed();
}

bool CAnimatedGraphic::OnThink()
{
	m_currentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames));
	CWarspiteObject::OnThink();
	
	return true;
}
