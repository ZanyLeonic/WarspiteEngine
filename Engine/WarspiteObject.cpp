#include "WarspiteObject.h"
#include "TextureManager.h"
#include "Game.h"

WarspiteObject::WarspiteObject() 
	: GameObject(), m_height(0), m_width(0), 
	m_numFrames(1), m_currentFrame(0), m_currentRow(0)
{
}

void WarspiteObject::Load(const ObjectParams* pParams)
{
	m_position = Vector2D(pParams->GetX(), pParams->GetY());
	m_velocity = Vector2D(0, 0);
	m_acceleration = Vector2D(0, 0);

	m_width = pParams->GetWidth();
	m_height = pParams->GetHeight();
	m_textureID = pParams->GetTextureID();

	m_currentRow = 1;
	m_currentFrame = 0;
	m_numFrames = pParams->GetNumberOfFrames();
}

void WarspiteObject::OnPlay()
{
}

void WarspiteObject::Draw()
{
	if (m_velocity.GetX() > 0)
	{
		TextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height, 
			m_currentRow, m_currentFrame, Game::Instance()->GetRenderer(), 
			SDL_FLIP_HORIZONTAL);
	}
	else
	{
		TextureManager::Instance()->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height,
			m_currentRow, m_currentFrame, Game::Instance()->GetRenderer());
	}
}

bool WarspiteObject::OnThink()
{
	m_velocity += m_acceleration;  
	m_position += m_velocity;

	return true;
}

void WarspiteObject::Destroy()
{
}