#include "GarbageCollector.h"
#include "GameStateBase.h"

void CGarbageCollector::OnThink()
{
	if (m_StatesDeletionQueue.empty()) return;

	for (size_t i = 0; i < m_StatesDeletionQueue.size(); i++)
	{
		if (m_StatesDeletionQueue[i]->StateFinishedTicking())
		{
			m_StatesDeletionQueue[i].reset(nullptr);
			m_StatesDeletionQueue.erase(m_StatesDeletionQueue.begin() + i);
		}
	}
}

void CGarbageCollector::Destroy()
{
}

void CGarbageCollector::MarkStateForDeletion(std::unique_ptr<CGameStateBase> state)
{
	m_StatesDeletionQueue.push_back(std::move(state));
}