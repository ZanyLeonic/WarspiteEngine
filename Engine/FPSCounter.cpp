#include "FPSCounter.h"
#include "FontManager.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"
#include "Game.h"

CFPSCounter::CFPSCounter()
{
	m_textureID = "__engineFPS";
	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto.json", CEngineFileSystem::EPathType::FONTS), "Regular", 16);
}

void CFPSCounter::Load(const CObjectParams* pParams)
{
	// Do nothing.
}

void CFPSCounter::Draw()
{
	if (m_fps != m_lfps)
	{
		if (CTextureManager::Instance()->m_textureMap[m_textureID] != nullptr)
			SDL_DestroyTexture(CTextureManager::Instance()->m_textureMap[m_textureID]->GetTexture());

		CFontManager::Instance()->RenderText(fmt::format(FMT_STRING("FPS: {}"), (int)m_fps), "Roboto-Regular-16", m_textureID, CFontManager::EFontRenderType::SOLID, { 255, 255, 0 });
		
		m_width = CTextureManager::Instance()->m_textureMap[m_textureID]->GetWidth();
		m_height = CTextureManager::Instance()->m_textureMap[m_textureID]->GetHeight();
		m_lfps = m_fps;
	}

	CTextureManager::Instance()->Draw(m_textureID, 0, 0, m_width, m_height, CGame::Instance()->GetRenderer());
}

void CFPSCounter::SetFPSValue(float fps)
{
	m_fps = fps;
}
