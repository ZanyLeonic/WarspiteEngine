#include "GameStateBase.h"
#ifdef _ENGINE_
#include "TextureManager.h"
#include "Game.h"
#elif _GAME_
#include "ITextureManager.h"
#include "WGame.h"
#include "IGame.h"
#include "Vector2D.h"
#endif

CGameStateBase::CGameStateBase()
{
#ifdef _GAME_
	pTex = (ITextureManager*)CGame::Instance()->GetPointers()[ESingletonIDs::TEXTUREMANAGER];
#endif
}

bool CGameStateBase::OnPlay()
{
	m_bShouldTick = true;
	m_bShouldDraw = true;

	return true;
}

bool CGameStateBase::OnEnd()
{
	// Stop accessing pointers that are about to be destroyed!
	m_bShouldTick = false;
	m_bShouldDraw = false;

	// Destroying everything!
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
		m_GameObjects[i].reset(nullptr);
		m_GameObjects.erase(m_GameObjects.begin() + i);
	}

	for (size_t i = 0; i < m_TextureIDList.size(); i++)
	{
#ifdef _ENGINE_
		CTextureManager::Instance()->Remove(m_TextureIDList[i]);
#elif _GAME_
		pTex->Remove(m_TextureIDList[i]);
#endif
	}

	m_GameObjects.clear();
	m_TextureIDList.clear();

	return true;
}

SCollisionData CGameStateBase::IsColliding(CVector2D v1)
{
	SCollisionData r;
	if (!m_GameObjects.empty())
	{
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			// See if you can find a colliding object.
			if (m_GameObjects[i]->GetPosition() == v1)
			{
				r.m_otherObject = m_GameObjects[i].get();
				r.m_result = m_GameObjects[i]->ShouldCollide() ? ECollisionResult::COLLIDED : ECollisionResult::NONE;
				r.m_location = m_GameObjects[i]->GetPosition();
				
				return r;
			}
		}
	}
	return r;
}

CGameStateBase::~CGameStateBase()
{
	m_GameObjects.clear();
}

void CGameStateBase::OnThink()
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{	
		if (m_bShouldTick)
			m_GameObjects[i]->OnThink();
		else
			break;
	}

	m_bTickingFinished = !m_bShouldTick;
}

void CGameStateBase::Draw()
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_bShouldDraw)
			m_GameObjects[i]->Draw();
		else
			break;
	}

	m_bDrawingFinished = !m_bShouldDraw;
}