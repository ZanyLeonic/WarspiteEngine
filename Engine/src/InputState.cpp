#include "InputState.h"
#include "FontManager.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include <fmt\format.h>
#include "Game.h"
#include "Button.h"
#include "StateParser.h"

CInputState::CInputState()
{
	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto.json", CEngineFileSystem::EPathType::FONTS), "Light", 28);
}

bool CInputState::OnPlay()
{
	CMenuState::OnPlay();

	SDL_SetRenderDrawColor(CBaseGame::Instance()->GetRenderer(), 255, 255, 255, 255);

	s_UIID = SID_INPUT;

	// Parse the state
	CStateParser::ParseState(CEngineFileSystem::ResolvePath("SystemMenus.json", CEngineFileSystem::EPathType::STATE).c_str(), s_UIID, &m_GameObjects, &m_TextureIDList, &m_ScriptIDList);

	CInputHandler::Instance()->AddOnKeyDown([this](SDL_Scancode e) {
			std::string keyName = std::string(SDL_GetKeyName(SDL_GetKeyFromScancode(e)));

			for (int i = 0; i < m_keysActive.size(); i++)
			{
				if (keyName == m_keysActive[i]) return;
			}

			m_keysActive.push_back(keyName);
		});

	CInputHandler::Instance()->AddOnKeyUp([this](SDL_Scancode e) {
			std::string keyName = std::string(SDL_GetKeyName(SDL_GetKeyFromScancode(e)));

			for (int i = 0; i < m_keysActive.size(); i++)
			{
				if (keyName == m_keysActive[i])
				{
					// Remove its entry
					m_keysActive.erase(m_keysActive.begin() + i);
				}
			}
		});

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->OnPlay();
	}

	m_callbacks.push_back(0);
	m_callbacks.push_back(s_InputToMainMenu);

	SetCallbacks(m_callbacks);

	return true;
}

void CInputState::Draw()
{
	CMenuState::Draw();

	if (CTextureManager::Instance()->m_textureMap["__inputRead"] != nullptr)
	{
		CTexture* t = CTextureManager::Instance()->m_textureMap["__inputRead"];
		CTextureManager::Instance()->Draw("__inputRead", 5, 50, t->GetWidth(), t->GetHeight(), CBaseGame::Instance()->GetRenderer());
	}
}

void CInputState::OnThink()
{
	CMenuState::OnThink();

	if (CTextureManager::Instance()->m_textureMap["__inputRead"] != nullptr)
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__inputRead"]->GetTexture());

	std::string keysDown = "";

	for (int i = 0; i < m_keysActive.size(); i++)
	{
		keysDown += m_keysActive[i];
		keysDown += " ";
	}

	CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("Keys down:\n{}"), keysDown), 
		"Roboto-Light-28", "__inputRead", CFontManager::EFontRenderType::BLENDED_WRAPPED, { 0, 0, 0 }, { 255,255,255 }, 480);
}

bool CInputState::OnEnd()
{
	return false;
}

void CInputState::SetCallbacks(const std::vector<HButtonCallback>& callbacks)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (dynamic_cast<CButton*>(m_GameObjects[i]))
		{
			CButton* pButton =
				dynamic_cast<CButton*>(m_GameObjects[i]);

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

bool CInputState::s_InputToMainMenu()
{
	spdlog::info("Back button clicked");
	CBaseGame::Instance()->GetStateManager()->ModifyState(CGameStateDictionary::Instance()->Create(SID_MM));
	return true;
}

//void CInputState::OnKeyDown(SDL_Scancode e)
//{
//	std::string keyName = SDL_GetKeyName(e);
//
//	for (int i = 0; i < m_keysActive.size(); i++)
//	{
//		if (keyName == m_keysActive[i]) return;
//	}
//
//	m_keysActive.push_back(keyName);
//}
//
//void CInputState::OnKeyUp(SDL_Scancode e)
//{
//	std::string keyName = SDL_GetKeyName(e);
//
//	for (int i = 0; i < m_keysActive.size(); i++)
//	{
//		if (keyName == m_keysActive[i])
//		{
//			// Remove its entry
//			m_keysActive.erase(m_keysActive.begin() + i);
//		}
//	}
//}
