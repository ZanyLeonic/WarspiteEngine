#include "FadeObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "WarspiteUtil.h"

CFadeObject::CFadeObject()
{
	m_textureID = "_fade";
	m_width = CBaseGame::Instance()->GetViewportSize().GetX();
	m_height = CBaseGame::Instance()->GetViewportSize().GetY();
	m_position = CVector2D(0, 0);

	m_bCancelMovementOnOverlap = false;
	m_overlap = false;
	m_collidable = false;

	m_pFadeTexture = CTextureManager::Instance()->m_textureMap[m_textureID];
	SDL_SetTextureBlendMode(m_pFadeTexture->GetTexture(), SDL_BLENDMODE_BLEND);
}

bool CFadeObject::OnThink()
{
	SDL_SetTextureAlphaMod(m_pFadeTexture->GetTexture(), m_fCurrentTime);

	if (m_bFading)
	{
		m_fCurrentTime = CWarspiteUtil::FLerp(m_fStartOpacity, m_fTargetOpacity, (m_fFadeTimeLeft / 100));
		
		if (m_fCurrentTime != m_fTargetOpacity)
		{
			m_fFadeTimeLeft += 0.1f;
		}
		else
		{
			m_bFading = false;
		}
	}
	return true;
}

void CFadeObject::FadeIn()
{
	m_fStartOpacity = 0.f;
	m_fTargetOpacity = 255.f;
	m_fCurrentTime = 0.f;
	m_fFadeTimeLeft = 0.f;

	m_bFading = true;
}

void CFadeObject::FadeOut()
{
	m_fStartOpacity = 255.f;
	m_fTargetOpacity = 0.f;
	m_fCurrentTime = 0.f;
	m_fFadeTimeLeft = 0.f;

	m_bFading = true;
}