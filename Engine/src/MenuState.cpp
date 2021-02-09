#include "MenuState.h"

#include "UIBase.h"
#include <spdlog/spdlog.h>
#include <limits>

void CMenuState::SetCallbacks(const std::vector<HButtonCallback>& callbacks)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		IGameObject* pBase = m_GameObjects[i].get();
		CUIBase* pUIBase = dynamic_cast<CUIBase*>(pBase);

		if (pUIBase)
		{
			assert(!((m_callbacks.size() - 1) > std::numeric_limits<int>::max() && "Callback list larger than signed int max"));

			// So we don't crash when we try add something new.
			int cbSize = static_cast<int>(m_callbacks.size() - 1);

			if (pUIBase->GetOnClickID() > cbSize)
			{
				spdlog::warn("OnClickID event ID forUI object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pUIBase->OnClick(callbacks[pUIBase->GetOnClickID()]);
			}

			if (pUIBase->GetOnEnterID() > cbSize)
			{
				spdlog::warn("OnEnterID event ID for UI object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pUIBase->OnEnter(callbacks[pUIBase->GetOnEnterID()]);
			}

			if (pUIBase->GetOnLeaveID() > cbSize)
			{
				spdlog::warn("OnLeaveID event ID for UI object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pUIBase->OnLeave(callbacks[pUIBase->GetOnLeaveID()]);
			}
		}
	}
}