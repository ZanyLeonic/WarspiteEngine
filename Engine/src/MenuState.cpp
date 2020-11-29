#include "MenuState.h"

#include "Button.h"
#include <spdlog/spdlog.h>

void CMenuState::SetCallbacks(const std::vector<HButtonCallback>& callbacks)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		IGameObject* pBase = m_GameObjects[i].get();
		CButton* pButton = dynamic_cast<CButton*>(pBase);

		if (pButton)
		{

			// So we don't crash when we try add something new.
			size_t cbSize = m_callbacks.size() - 1;

			if (pButton->GetOnClickID() > cbSize)
			{
				spdlog::warn("OnClickID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnClick(callbacks[pButton->GetOnClickID()]);
			}

			if (pButton->GetOnEnterID() > cbSize)
			{
				spdlog::warn("OnEnterID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnEnter(callbacks[pButton->GetOnEnterID()]);
			}

			if (pButton->GetOnLeaveID() > cbSize)
			{
				spdlog::warn("OnLeaveID event ID for Button object \"{}\" is larger than the defined callbacks!",
					m_GameObjects[i]->GetName());
				spdlog::warn("Is this JSON intended for this version of the Engine or did some forget to implement the callback?");
			}
			else
			{
				pButton->OnLeave(callbacks[pButton->GetOnLeaveID()]);
			}
		}
	}
}
