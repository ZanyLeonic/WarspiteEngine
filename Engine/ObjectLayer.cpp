#include "ObjectLayer.h"

void ObjectLayer::OnPlay()
{
	// Initialising objects and stuff
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}
}

void ObjectLayer::Destroy()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Destroy();
	}
	m_GameObjects.clear();
}

void ObjectLayer::OnThink()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (!m_GameObjects[i]->OnThink()) return;
	}
}

void ObjectLayer::Draw()
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw();
	}
}
