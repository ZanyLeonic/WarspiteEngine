#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"

SDLGameObject::SDLGameObject(const ObjectParams* pParams)
	: GameObject(pParams)
{
	m_x = pParams->GetX();
	m_y = pParams->GetY();
	m_width = pParams->GetWidth();
	m_height = pParams->GetHeight();
	m_textureID = pParams->GetTextureID();

	m_currentRow = 1;
	m_currentFrame = 1;
}

void SDLGameObject::Draw()
{
	TextureManager::Instance()->DrawFrame(m_textureID, m_x, m_y,
		m_width, m_height, m_currentRow, m_currentFrame,
		Game::Instance()->GetRenderer());
}

void SDLGameObject::OnThink()
{
}

void SDLGameObject::Destroy()
{
}
