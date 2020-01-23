#include "GameStateBase.h"

void GameStateBase::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (!m_shouldTick) return;
		m_GameObjects[i]->OnThink();
	}
}

void GameStateBase::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (!m_shouldDraw) return;
		m_GameObjects[i]->Draw();
	}
}

bool GameStateBase::OnPlay()
{
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