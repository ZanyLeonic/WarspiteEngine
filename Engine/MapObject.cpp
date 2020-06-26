#include "Camera.h"
#include "MapObject.h"
#include "TextureManager.h"
#include "Game.h"

CMapObject::CMapObject()
	: CWarspiteObject()
{
}

void CMapObject::Draw()
{
	// Get the camera position to offset the drawing
	Vector2D cPos = Camera::Instance()->GetPositionT();

	// Flip the sprite automatically if the velocity is negative.
	if (m_velocity.GetX() > 0)
	{
		TextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			Game::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		TextureManager::Instance()->DrawFrame(m_textureID,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
			m_width, m_height, m_currentRow, m_currentFrame,
			Game::Instance()->GetRenderer());
	}
}