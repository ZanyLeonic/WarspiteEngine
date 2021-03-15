#ifdef _ENGINE_
#include "TextureManager.h"
#include "Game.h"
#include "Camera.h"
#elif _GAME_
#include "ITextureManager.h"
#include "WGame.h"
#include "IGame.h"
#include "ICamera.h"
#endif

#include <spdlog/spdlog.h>
#include <string>
#include "WarspiteObject.h"

CWarspiteObject::CWarspiteObject() 
	: IGameObject(), m_iHeight(0), m_iWidth(0), 
	m_iNumFrames(1), m_iCurrentFrame(0), m_iCurrentRow(0)
{
#ifdef _GAME_
	pTex = (ITextureManager*)CGame::Instance()->GetPointers()[ESingletonIDs::TEXTUREMANAGER];
	pGame = (IGame*)CGame::Instance()->GetPointers()[ESingletonIDs::GAME];
#endif

	for (int i = 0; i < 9; i++)
	{
		if (i == 0)
			m_aCollisionChannels.push_back(true);
		else
			m_aCollisionChannels.push_back(false);
	}
}

void CWarspiteObject::Load(CObjectParams* pParams)
{
	m_sObjectName = pParams->GetName();
	m_sFactoryID = pParams->GetFactoryID();
	
	m_vPosition = CVector2D(pParams->GetX(), pParams->GetY());
	m_vVelocity = CVector2D(0, 0);
	m_vAcceleration = CVector2D(0, 0);

	m_iWidth = pParams->GetProperty<int>("textureWidth");
	m_iHeight = pParams->GetProperty<int>("textureHeight");
	m_sTextureID = pParams->GetProperty<std::string>("textureID");

	m_iCurrentRow = 1;
	m_iCurrentFrame = 0;
	m_iNumFrames = pParams->GetProperty<int>("numFrames");
}

CVector2D CWarspiteObject::GetViewportPosition()
{
	// Return the position of the object on the screen
	CVector2D vNewPos = m_vPosition;
#ifdef _ENGINE_
	CVector2D vCamPos = CCamera::Instance()->GetPositionT();
#elif _GAME_
	ICamera* pCamera = (ICamera*)CGame::Instance()->GetPointers()[ESingletonIDs::CAMERA];
	CVector2D vCamPos = pCamera->GetPositionT();
#endif

	vNewPos.SetX(vNewPos.GetX() - vCamPos.GetX());
	vNewPos.SetY(vNewPos.GetY() - vCamPos.GetY());

	return vNewPos;
}

void CWarspiteObject::OnOverlapStart()
{
	m_bIsOverlapping = true;

	spdlog::debug("[{}] Overlapping start!", m_sObjectName);
}

void CWarspiteObject::OnOverlapEnd()
{
	m_bIsOverlapping = false;

	spdlog::debug("[{}] Overlapping end!", m_sObjectName);
}

void CWarspiteObject::SetCollisionOnChannel(ECollisionChannel pChannel, bool pState)
{
	if (pChannel == ECollisionChannel::NONE) return;

	m_aCollisionChannels[int(pChannel) - 1] = pState;
}

bool CWarspiteObject::CollidesOnChannel(ECollisionChannel pChannel)
{
	if (pChannel == ECollisionChannel::NONE) return false;

	return m_aCollisionChannels[int(pChannel) - 1];
}

bool CWarspiteObject::InteractAction(IGameObject* pOther)
{
	spdlog::info("[{}] Interaction triggered by \"{}\"", GetName(), pOther->GetName());

	return true;
}

void CWarspiteObject::OnPlay()
{
}

void CWarspiteObject::Draw()
{
#ifdef _ENGINE_
	if (m_vVelocity.GetX() > 0)
	{
		CTextureManager::Instance()->DrawFrame(m_sTextureID, (int)m_vPosition.GetX(),
			(int)m_vPosition.GetY(), m_iWidth, m_iHeight,
			m_iCurrentRow, m_iCurrentFrame, CBaseGame::Instance()->GetRenderer(),
			SDL_FLIP_HORIZONTAL);
	}
	else
	{
		CTextureManager::Instance()->DrawFrame(m_sTextureID, (int)m_vPosition.GetX(),
			(int)m_vPosition.GetY(), m_iWidth, m_iHeight,
			m_iCurrentRow, m_iCurrentFrame, CBaseGame::Instance()->GetRenderer());
	}
#elif _GAME_
	if (m_vVelocity.GetX() > 0)
	{
		pTex->DrawFrame(m_sTextureID, (int)m_vPosition.GetX(),
			(int)m_vPosition.GetY(), m_iWidth, m_iHeight,
			m_iCurrentRow, m_iCurrentFrame, pGame->GetRenderer(),
			0.0, nullptr, EWarRendererFlip::FLIP_HORIZONTAL);
	}
	else
	{
		pTex->DrawFrame(m_sTextureID, (int)m_vPosition.GetX(),
			(int)m_vPosition.GetY(), m_iWidth, m_iHeight,
			m_iCurrentRow, m_iCurrentFrame, pGame->GetRenderer());
	}
#endif
}

bool CWarspiteObject::OnThink()
{
	m_vVelocity += m_vAcceleration;  
	m_vPosition += m_vVelocity;

	return true;
}

void CWarspiteObject::Destroy()
{
} 