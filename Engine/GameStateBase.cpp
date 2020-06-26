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
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}

	for (int i = 0; i < m_TextureIDList.size(); i++)
	{
		CTextureManager::Instance()->Remove(m_TextureIDList[i]);
	}

	m_GameObjects.clear();
	m_TextureIDList.clear();

	return true;
}

void CGameStateBase::OnThink()
{
	if (!m_shouldTick) return;
	for (int i = 0; i < m_GameObjects.size(); i++)
	{	
		if (!m_GameObjects[i]->OnThink()) return;
	}
}

void CGameStateBase::Draw()
{
	if (!m_shouldDraw) return;

	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}
