#include "Level.h"
#include "ObjectLayer.h"

CLevel::CLevel(std::string& path)
	: m_path(path)
{
	if(m_scriptLayer == nullptr) m_scriptLayer = new CObjectLayer();
}

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
	m_scriptLayer->Destroy();
}

void CLevel::OnThink()
{
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->OnThink();
	}

	// Run the script layer last
	m_scriptLayer->OnThink();
}

void CLevel::Draw()
{
	for (size_t i = 0; i < m_layers.size(); i++)
	{
		m_layers[i]->Draw();
	}

	// Run the script layer last
	m_scriptLayer->Draw();
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