#include "TextObject.h"
#include "FontManager.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"

CTextObject::CTextObject()
{
	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto.json", CEngineFileSystem::EPathType::FONTS), "Regular", 32);
}

void CTextObject::Load(const CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	CFontManager::Instance()->RenderText("Warspite Engine", "Roboto-Regular-32", m_textureID, CFontManager::EFontRenderType::SHADED, { 0, 0, 0}, { 255, 255, 255});

	m_width = CTextureManager::Instance()->m_textureMap[m_textureID]->GetWidth();
	m_height = CTextureManager::Instance()->m_textureMap[m_textureID]->GetHeight();
}
