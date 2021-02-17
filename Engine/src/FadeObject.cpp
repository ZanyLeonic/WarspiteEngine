#include "FadeObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "WarspiteUtil.h"

CFadeObject::CFadeObject()
{
	m_textureID = "_fade";
	m_width = (int)CBaseGame::Instance()->GetViewportSize().GetX();
	m_height = (int)CBaseGame::Instance()->GetViewportSize().GetY();
	m_position = CVector2D(0, 0);

	m_bCancelMovementOnOverlap = false;
	m_overlap = false;
	m_collidable = false;

	m_pFadeTexture = CTextureManager::Instance()->m_textureMap[m_textureID];
}

void CFadeObject::Draw()
{
	CTextureManager::Instance()->Draw(m_pFadeTexture, 0, 0, CBaseGame::Instance()->GetRenderer());
}

bool CFadeObject::OnThink()
{
	SDL_SetTextureAlphaMod(m_pFadeTexture->GetTexture(), (Uint8)m_fCurrentOpacity);

	if (m_bFading)
	{
		if (m_fCurrentOpacity + (m_fFadeSpeed * (int)m_eDirection) > 255)
		{
			m_fCurrentOpacity = 255;
			m_bFading = false;
		}
		else
		{
			m_fCurrentOpacity += m_fFadeSpeed;
		}
	}
	return true;
}

void CFadeObject::FadeIn()
{
	if (m_bFading) return;

	m_eDirection = EPlaybackDirection::FORWARD;
	m_bFading = true;
}

void CFadeObject::FadeOut()
{
	if (m_bFading) return;

	m_eDirection = EPlaybackDirection::BACKWARD;
	m_bFading = true;
}