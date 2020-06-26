#include "ObjectLayer.h"

void CObjectLayer::OnPlay()
{
	// Initialising objects and stuff
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}
}

void CObjectLayer::Destroy()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();
}

void CObjectLayer::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (!m_GameObjects[i]->OnThink()) return;
	}
}

void CObjectLayer::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}
