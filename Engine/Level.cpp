#include "Level.h"

void Level::OnThink()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnThink();
	}
}

void Level::Draw()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Draw();
	}
}