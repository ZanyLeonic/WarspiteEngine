#include "FadeObject.h"
#include "TextureManager.h"
#include "Game.h"
#include "WarspiteUtil.h"
#include <spdlog/spdlog.h>

CFadeObject::CFadeObject()
{
	m_sTextureID = "_fade";
	m_sObjectName = "FadeObject";

	m_iWidth = (int)CBaseGame::Instance()->GetViewportSize()->GetX();
	m_iHeight = (int)CBaseGame::Instance()->GetViewportSize()->GetY();
	m_vPosition = CVector2D(0, 0);

	m_bCancelMovementOnOverlap = false;
	m_bOverlap = false;
	m_bCollides = false;

	m_pFadeTexture = CTextureManager::Instance()->m_textureMap[m_sTextureID];
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
		bool fCond = false;

		switch (m_eDirection)
		{
		case EPlaybackDirection::FORWARD:
			fCond = m_fCurrentOpacity + (m_fFadeSpeed * (int)m_eDirection) > m_fTargetOpacity;
			break;
		case EPlaybackDirection::BACKWARD:
			fCond = m_fCurrentOpacity + (m_fFadeSpeed * (int)m_eDirection) < m_fTargetOpacity;
			break;
		}

		callCallbacks(false);

		if (fCond)
		{
			m_fCurrentOpacity = m_fTargetOpacity;
			m_bFading = false;
			callCallbacks(true);
		}
		else
		{
			m_fCurrentOpacity += (m_fFadeSpeed * (int)m_eDirection);
		}
	}
	return true;
}

void CFadeObject::FadeIn()
{
	if (m_bFading) return;

	m_eDirection = EPlaybackDirection::FORWARD;
	m_fTargetOpacity = 255.f;
	m_bFading = true;
}

void CFadeObject::FadeOut()
{
	if (m_bFading) return;

	m_eDirection = EPlaybackDirection::BACKWARD;
	m_fTargetOpacity = 0.f;
	m_bFading = true;
}

void CFadeObject::AddCallback(std::string id, HFadeComplete callback)
{
	m_callbacks[id] = callback;
}

void CFadeObject::RemoveCallback(std::string id)
{
	m_callbacks.erase(id);
}

void CFadeObject::callCallbacks(bool fadeCompleted)
{
	std::map<std::string, HFadeComplete>::iterator it = m_callbacks.begin();

	for (std::pair<std::string, HFadeComplete> e : m_callbacks) {
		// spdlog::debug("[{}] Calling registered callback ({})", m_objectName, e.first);
		e.second(m_eDirection, fadeCompleted);
	}
}