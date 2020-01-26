#include "GameStateManager.h"

void GameStateManager::PushState(GameState* pState)
{
	m_GameStates.push_back(pState);
	m_GameStates.back()->OnPlay();
}

void GameStateManager::PopState()
{
	// Check if the queue has items before popping
	if (!m_GameStates.empty())
	{
		// Only remove the last item if their OnEnd implementation
		// returns true.
		if (m_GameStates.back()->OnEnd())
		{
			// Delete the actual object in memory
			delete m_GameStates.back();
			// ...and remove its pointer from the queue.
			m_GameStates.pop_back();
		}
	}
}

void GameStateManager::Draw()
{
	if (!m_GameStates.empty())
	{
		m_GameStates.back()->Draw();
	}
}

void GameStateManager::OnThink()
{
	if (!m_GameStates.empty())
	{
		m_GameStates.back()->OnThink();
	}
}

void GameStateManager::ModifyState(GameState* pState)
{
	StateDeleted = true;
	if (!m_GameStates.empty())
	{
		// Don't do anything if the "new" state is the same.
		if (m_GameStates.back()->GetStateID() == pState->GetStateID())
		{
			return;
		}

		if (m_GameStates.back()->OnEnd())
		{
			delete m_GameStates.back();
			m_GameStates.pop_back();
		}
	}

	// Add the new state the queue and run their OnPlay method.
	m_GameStates.push_back(pState);
	m_GameStates.back()->OnPlay();

	StateDeleted = false;
}