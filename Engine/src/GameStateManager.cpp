#include "GameStateManager.h"
#include "GameStateBase.h"
#include "GarbageCollector.h"
#include "Game.h"

const std::string CGameStateManager::GetCurrentStateID()
{
	return m_GameStates.back()->GetStateID();
}

void CGameStateManager::PushState(std::unique_ptr<CGameStateBase> pState)
{
	m_GameStates.push_back(std::move(pState));
	m_GameStates.back()->OnPlay();
}

void CGameStateManager::PopState()
{
	// Check if the queue has items before Popping
	if (!m_GameStates.empty())
	{
		// Only remove the last item if their OnEnd implementation
		// returns true.
		if (m_GameStates.back()->OnEnd())
		{
			CBaseGame::Instance()->GetGarbageCollector()->MarkStateForDeletion(std::move(m_GameStates.back()));

			// ...and remove its Pointer from the queue.
			m_GameStates.pop_back();
		}
	}
}

void CGameStateManager::Draw()
{
	if (!m_GameStates.empty() && m_GameStates.back())
	{
		m_GameStates.back()->Draw();
	}
}

void CGameStateManager::OnThink()
{
	if (!m_GameStates.empty() && m_GameStates.back())
	{
		m_GameStates.back()->OnThink();
	}
}

void CGameStateManager::ModifyState(std::unique_ptr<CGameStateBase> pState)
{
	if (!m_GameStates.empty())
	{
		// Don't do anything if the "new" state is the same.
		if (m_GameStates.back()->GetStateID() == pState->GetStateID())
		{
			return;
		}

		if (m_GameStates.back()->OnEnd())
		{
			CBaseGame::Instance()->GetGarbageCollector()->MarkStateForDeletion(std::move(m_GameStates.back()));
			m_GameStates.pop_back();
		}
	}

	// Add the new state the queue and run their OnPlay method.
	m_GameStates.push_back(std::move(pState));
	m_GameStates.back()->OnPlay();
}