#include "FPSCounter.h"
#include "FontManager.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"
#include "Game.h"

#include <fmt/format.h>

CFPSCounter::CFPSCounter()
{
	m_sTextureID = "__engineFPS";
	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto.json", CEngineFileSystem::EPathType::FONTS), "Regular", 16);
}

CFPSCounter::~CFPSCounter()
{
	if (CTextureManager::Instance()->m_textureMap[m_sTextureID] != nullptr)
		SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetTexture());
	CFontManager::Instance()->RemoveFont("Roboto", "Regular", 16);
}

void CFPSCounter::Load(CObjectParams* pParams)
{
	// Do nothing.
}

void CFPSCounter::Draw()
{
	if (m_fps != m_lfps)
	{
		if (CTextureManager::Instance()->m_textureMap[m_sTextureID] != nullptr)
			SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetTexture());

		CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("FPS: {}"), (int)m_fps), "Roboto-Regular-16", m_sTextureID, CFontManager::EFontRenderType::SOLID, { 255, 255, 0 });
		
		m_iWidth = CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetWidth();
		m_iHeight = CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetHeight();
		m_lfps = m_fps;
	}

	CTextureManager::Instance()->Draw(m_sTextureID, 0, 0, m_iWidth, m_iHeight, CBaseGame::Instance()->GetRenderer());
}

void CFPSCounter::SetFPSValue(float fps)
{
	m_fps = fps;
}
