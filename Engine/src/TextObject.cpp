#include "TextObject.h"
#include "FontManager.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"
#include "Game.h"

CTextObject::CTextObject()
{
	CFontManager::Instance()->LoadFont(CEngineFileSystem::ResolvePath("Roboto.json", CEngineFileSystem::EPathType::FONTS), "Regular", 32);
}

CTextObject::~CTextObject()
{
	CFontManager::Instance()->RemoveFont("Roboto", "Regular", 32);
}

void CTextObject::Load(CObjectParams* pParams)
{
	CWarspiteObject::Load(pParams);

	// I would make this better, but I am short on time at this moment, so this will do.
	if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() == SID_MM)
	{
		CFontManager::Instance()->RenderText("Warspite Engine", "Roboto-Regular-32", m_textureID, CFontManager::EFontRenderType::SHADED, { 0, 0, 0 }, { 255, 255, 255});
	}
	else if (CBaseGame::Instance()->GetStateManager()->GetCurrentStateID() == SID_INPUT)
	{
		CFontManager::Instance()->RenderText("Input Test", "Roboto-Regular-32", m_textureID, CFontManager::EFontRenderType::SHADED, { 0, 0, 0 }, { 255, 255, 255 });
	}

	m_width = CTextureManager::Instance()->m_textureMap[m_textureID]->GetWidth();
	m_height = CTextureManager::Instance()->m_textureMap[m_textureID]->GetHeight();
}
