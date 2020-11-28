#include "GameStateBase.h"
#include "Game.h"
#include "TextureManager.h"

bool CGameStateBase::OnPlay()
{
	m_shouldTick = true;
	m_shouldDraw = true;

	return true;
}

bool CGameStateBase::OnEnd()
{
	// Stop accessing pointers that are about to be destroyed!
	m_shouldTick = false;
	m_shouldDraw = false;

	// Destroying everything!
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}

	for (size_t i = 0; i < m_TextureIDList.size(); i++)
	{
		CTextureManager::Instance()->Remove(m_TextureIDList[i]);
	}

	m_GameObjects.clear();
	m_TextureIDList.clear();

	return true;
}

CGameStateBase::~CGameStateBase()
{
	m_GameObjects.clear();
}

void CGameStateBase::OnThink()
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{	
		if (m_GameObjects.size() < 15 && m_shouldTick)
			m_GameObjects[i]->OnThink();
		else
		{
			spdlog::warn("Warning! Invalid State caught!");
			return;
		}
	}

	m_bTickingFinished = !m_shouldTick;
}

void CGameStateBase::Draw()
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i] && m_shouldDraw)
			m_GameObjects[i]->Draw();
	}

	m_bDrawingFinished = !m_shouldDraw;
}