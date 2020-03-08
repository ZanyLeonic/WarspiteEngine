#include "ObjectLayer.h"

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
