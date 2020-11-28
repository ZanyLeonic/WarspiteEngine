#include "GarbageCollector.h"
#include "GameStateBase.h"

void CGarbageCollector::OnThink()
{
	if (m_StatesDeletionQueue.empty()) return;

	for (int i = 0; i < m_StatesDeletionQueue.size(); i++)
	{
		std::shared_ptr<CGameStateBase> l = m_StatesDeletionQueue[i];

		if (l->StateFinishedDrawing() && l->StateFinishedTicking())
		{
			m_StatesDeletionQueue.erase(m_StatesDeletionQueue.begin() + i);
		}
	}
}

void CGarbageCollector::Destroy()
{
}

void CGarbageCollector::MarkStateForDeletion(std::shared_ptr<CGameStateBase> state)
{
	m_StatesDeletionQueue.push_back(state);
}
