#include "AnimatedGraphic.h"
#include <SDL.h>

void CAnimatedGraphic::Load(CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	m_animSpeed = pParams->GetProperty<int>("animSpeed");
}

bool CAnimatedGraphic::OnThink()
{
	m_iCurrentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) % m_iNumFrames));
	CWarspiteObject::OnThink();
	
	return true;
}
