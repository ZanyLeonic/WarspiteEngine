#include "TextObject.h"
#include "FontManager.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"
#include "Game.h"

CTextObject::CTextObject()
	: CMapObject()
{
}

CTextObject::~CTextObject()
{
	CFontManager::Instance()->RemoveFont(m_sFont, m_sVariant, m_iFontSize);
}

void CTextObject::Load(CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	m_sText = pParams->GetProperty<std::string>("text");
	m_sFont = pParams->GetProperty<std::string>("font");
	m_sVariant = pParams->GetProperty<std::string>("variant");
	m_iFontSize = pParams->GetProperty<int>("fontSize");
	m_sFontString = m_sFont + "-" + m_sVariant + "-" + std::to_string(m_iFontSize);

	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath(m_sFont, CEngineFileSystem::EPathType::FONTS), m_sVariant, m_iFontSize);
	CFontManager::Instance()->RenderText(m_sText, m_sFontString, m_sTextureID, CFontManager::EFontRenderType::SHADED, { 0, 0, 0 }, { 255, 255, 255});

	m_iWidth = CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetWidth();
	m_iHeight = CTextureManager::Instance()->m_textureMap[m_sTextureID]->GetHeight();
}
