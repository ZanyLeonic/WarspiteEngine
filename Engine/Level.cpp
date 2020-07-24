#include "Level.h"

#include "ObjectLayer.h"

void CLevel::OnPlay()
{
	// Execute each Layers OnPlay method
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnPlay();
	}
}

void CLevel::Destroy()
{
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Destroy();
	}
	m_layers.clear();
}

void CLevel::OnThink()
{
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnThink();
	}
}

void CLevel::Draw()
{
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Draw();
	}
}

std::vector<std::vector<IGameObject*>*> CLevel::GetGameObjects()
{
	std::vector<std::vector<IGameObject*>*> out;
	
	for(size_t i = 0; i < m_layers.size(); i++)
	{
		CObjectLayer* cL = dynamic_cast<CObjectLayer*>(m_layers[i]);
		
		if (cL != nullptr)
		{
			out.push_back(cL->GetGameObjects());
		}
	}

	return out;
}