#include "ObjectLayer.h"
#include "Level.h"

Level::Level()
{

}

void Level::OnPlay()
{
	// Execute each Layers OnPlay method
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnPlay();
	}
}

void Level::Destroy()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Destroy();
	}
	m_layers.clear();
}

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