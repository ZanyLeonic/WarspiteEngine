#include "InputState.h"
#include "FontManager.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Game.h"
#include "Button.h"
#include "StateParser.h"

#include <fmt/format.h>

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

	CInputHandler::Instance()->AddOnMouseDown([this](SDL_Event e) {
		std::string mouseName = GetStringifiedMouseButton(e.button.button);
		
		if (mouseName == "") return;

		for (int i = 0; i < m_mouseActive.size(); i++)
		{
			if (mouseName == m_mouseActive[i]) return;
		}

		m_mouseActive.push_back(mouseName);

		});

	CInputHandler::Instance()->AddOnMouseUp([this](SDL_Event e) {
		std::string mouseName = GetStringifiedMouseButton(e.button.button);

		if (mouseName == "") return;

		for (int i = 0; i < m_mouseActive.size(); i++)
		{
			if (mouseName == m_mouseActive[i])
			{
				// Remove its entry
				m_mouseActive.erase(m_mouseActive.begin() + i);
			}
		}
		});

	CInputHandler::Instance()->AddOnJoyDown([this](SDL_Event e) {
		std::string joyBtn = fmt::format(FMT_STRING("Joy Button {}"), e.jbutton.button);

		for (int i = 0; i < m_joyActive.size(); i++)
		{
			if (joyBtn == m_joyActive[i]) return;
		}

		m_joyActive.push_back(joyBtn);
		});

	CInputHandler::Instance()->AddOnJoyUp([this](SDL_Event e) {
		std::string joyBtn = fmt::format(FMT_STRING("Joy Button {}"), e.jbutton.button);

		for (int i = 0; i < m_joyActive.size(); i++)
		{
			if (joyBtn == m_joyActive[i])
			{
				// Remove its entry
				m_joyActive.erase(m_joyActive.begin() + i);
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

	if (CTextureManager::Instance()->m_textureMap["__mouseRead"] != nullptr)
	{
		CTexture* f = CTextureManager::Instance()->m_textureMap["__mouseRead"];

		CTextureManager::Instance()->Draw("__mouseRead", 5, 50, f->GetWidth(), f->GetHeight(), CBaseGame::Instance()->GetRenderer());
	}

	if (CTextureManager::Instance()->m_textureMap["__joyRead"] != nullptr)
	{
		CTexture* c = CTextureManager::Instance()->m_textureMap["__joyRead"];

		CTextureManager::Instance()->Draw("__joyRead", 5, 78, c->GetWidth(), c->GetHeight(), CBaseGame::Instance()->GetRenderer());
	}

	if (CTextureManager::Instance()->m_textureMap["__inputRead"] != nullptr)
	{
		CTexture* t = CTextureManager::Instance()->m_textureMap["__inputRead"];

		CTextureManager::Instance()->Draw("__inputRead", 5, 134, t->GetWidth(), t->GetHeight(), CBaseGame::Instance()->GetRenderer());
	}
}

void CInputState::OnThink()
{
	CMenuState::OnThink();

	if (CTextureManager::Instance()->m_textureMap["__mouseRead"] != nullptr)
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__mouseRead"]->GetTexture());

	if (CTextureManager::Instance()->m_textureMap["__joyRead"] != nullptr)
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__joyRead"]->GetTexture());

	if (CTextureManager::Instance()->m_textureMap["__inputRead"] != nullptr)
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__inputRead"]->GetTexture());

	CVector2D pMousePos = *CInputHandler::Instance()->GetMousePosition();

	CVector2D pAxisOne = CVector2D(CInputHandler::Instance()->GetXAxis(0, 1), CInputHandler::Instance()->GetYAxis(0, 1));
	CVector2D pAxisTwo = CVector2D(CInputHandler::Instance()->GetXAxis(0, 2), CInputHandler::Instance()->GetYAxis(0, 2));

	std::string keysDown = "";

	for (int i = 0; i < m_mouseActive.size(); i++)
	{
		keysDown += m_mouseActive[i];
		keysDown += " ";
	}

	for (int i = 0; i < m_keysActive.size(); i++)
	{
		keysDown += m_keysActive[i];
		keysDown += " ";
	}

	for (int i = 0; i < m_joyActive.size(); i++)
	{
		keysDown += m_joyActive[i];
		keysDown += " ";
	}

	CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("Mouse Position: ({}, {})"), pMousePos.GetX(), pMousePos.GetY()), 
		"Roboto-Light-28", "__mouseRead", CFontManager::EFontRenderType::BLENDED_WRAPPED, { 0, 0, 0 }, { 255,255,255 }, 480);

	CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("Left Thumbstick: ({}, {})\nRight Thumbstick: ({}, {})"),
		pAxisOne.GetX(), pAxisOne.GetY(), pAxisTwo.GetX(), pAxisTwo.GetY()), "Roboto-Light-28", "__joyRead", CFontManager::EFontRenderType::BLENDED_WRAPPED, 
		{ 0, 0, 0 }, { 255,255,255 }, 480);

	CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("Input:\n{}"), keysDown), 
		"Roboto-Light-28", "__inputRead", CFontManager::EFontRenderType::BLENDED_WRAPPED, { 0, 0, 0 }, { 255,255,255 }, 480);
}

std::string CInputState::GetStringifiedMouseButton(int pMouseButton)
{
	switch (pMouseButton)
	{
	case SDL_BUTTON_LEFT:
		return "Left Mouse";
	case SDL_BUTTON_MIDDLE:
		return "Middle Mouse";
	case SDL_BUTTON_RIGHT:
		return "Right Mouse";
	default:
		return "";
	};
}

bool CInputState::OnEnd()
{
	CInputHandler::Instance()->RemoveOnCallbacks();

	if (CTextureManager::Instance()->m_textureMap["__mouseRead"] != nullptr)
	{ 
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__mouseRead"]->GetTexture());
		CTextureManager::Instance()->m_textureMap["__mouseRead"] = nullptr;
	}

	if (CTextureManager::Instance()->m_textureMap["__joyRead"] != nullptr)
	{
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__joyRead"]->GetTexture());
		CTextureManager::Instance()->m_textureMap["__joyRead"] = nullptr;
	}

	if (CTextureManager::Instance()->m_textureMap["__inputRead"] != nullptr)
	{
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap["__inputRead"]->GetTexture());
		CTextureManager::Instance()->m_textureMap["__inputRead"] = nullptr;
	}

	return true;
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
