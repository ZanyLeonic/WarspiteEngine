#include "GameStateBase.h"
#include "Game.h"
#include "TextureManager.h"

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
		CTextureManager::Instance()->Remove(m_TextureIDList[i]);
	}

	m_GameObjects.clear();
	m_TextureIDList.clear();

	return true;
}

bool CGameStateBase::IsColliding(CVector2D v1)
{
	if (!m_GameObjects.empty())
	{
		for (size_t i = 0; i < m_GameObjects.size(); i++)
		{
			// See if you can find a colliding object.
			if (m_GameObjects[i]->GetPosition() == v1)
			{
				return true;
			}
		}
	}
	return false;
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