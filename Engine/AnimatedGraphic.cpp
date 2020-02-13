#include "AnimatedGraphic.h"

void AnimatedGraphic::Load(const ObjectParams* pParams)
{
	SDLGameObject::Load(pParams);

	m_animSpeed = pParams->GetAnimSpeed();
}

bool AnimatedGraphic::OnThink()
{
	m_currentFrame = int(((SDL_GetTicks() / (1000 / m_animSpeed)) % m_numFrames));
	SDLGameObject::OnThink();
	
	return true;
}
