#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"

SDLGameObject::SDLGameObject(const ObjectParams* pParams)
	: GameObject(pParams), m_position(pParams->GetX(), pParams->GetY()),
	m_velocity(0, 0), m_acceleration(0, 0)
{
	m_width = pParams->GetWidth();
	m_height = pParams->GetHeight();
	m_textureID = pParams->GetTextureID();

	m_currentRow = 1;
	m_currentFrame = 1;
}

void SDLGameObject::Draw()
{
	TextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(), 
		(int)m_position.GetY(), m_width, m_height, m_currentRow, 
		m_currentFrame, Game::Instance()->GetRenderer());
}

void SDLGameObject::OnThink()
{
	m_velocity += m_acceleration;  
	m_position += m_velocity;
}

void SDLGameObject::Destroy()
{
}
