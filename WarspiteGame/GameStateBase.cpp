#include "GameStateBase.h"
#include "Game.h"

bool GameStateBase::OnPlay()
{
	m_shouldTick = true;
	m_shouldDraw = true;

	return true;
}

bool GameStateBase::OnEnd()
{
	// Stop accessing pointers that are about to be destroyed!
	m_shouldTick = false;
	m_shouldDraw = false;

	// Destroying everything!
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();

	return true;
}

void GameStateBase::OnThink()
{
	if (!m_shouldTick) return;
	for (int i = 0; i < m_GameObjects.size(); i++)
	{	
		if (!m_GameObjects[i]->OnThink()) return;
	}
}

void GameStateBase::Draw()
{
	if (!m_shouldDraw) return;

	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}
