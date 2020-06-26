#include "Level.h"

CLevel::CLevel()
{
}

void CLevel::OnPlay()
{
	// Execute each Layers OnPlay method
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnPlay();
	}
}

void CLevel::Destroy()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Destroy();
	}
	m_layers.clear();
}

void CLevel::OnThink()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnThink();
	}
}

void CLevel::Draw()
{
	for (int i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Draw();
	}
}