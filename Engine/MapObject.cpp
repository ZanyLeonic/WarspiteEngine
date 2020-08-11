#include "Camera.h"
#include "MapObject.h"
#include "TextureManager.h"
#include "Game.h"

CMapObject::CMapObject()
	: CWarspiteObject()
{
	m_collidable = true;
}

void CMapObject::Draw()
{
	if (bDrawObject)
	{
		// Get the camera position to offset the drawing
		CVector2D cPos = CCamera::Instance()->GetPositionT();

		// Flip the sprite automatically if the velocity is negative.
		if (m_velocity.GetX() > 0)
		{
			CTextureManager::Instance()->DrawFrame(m_textureID,
				int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
				m_width, m_height, m_currentRow, m_currentFrame,
				CGame::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
		}
		else
		{
			CTextureManager::Instance()->DrawFrame(m_textureID,
				int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()),
				m_width, m_height, m_currentRow, m_currentFrame,
				CGame::Instance()->GetRenderer());
		}
	}
}

void CMapObject::DoDraw(bool newVis)
{
	bDrawObject = newVis;
}