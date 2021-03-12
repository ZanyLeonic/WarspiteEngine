#include "Camera.h"
#include "MapObject.h"
#include "TextureManager.h"
#include "Game.h"

CMapObject::CMapObject()
	: CWarspiteObject()
{
	m_bCollides = true;
}

void CMapObject::Draw()
{
	if (bDrawObject)
	{
		// Get the camera position to offset the drawing
		CVector2D cPos = CCamera::Instance()->GetPositionT();

		// Flip the sprite automatically if the velocity is negative.
		if (m_vVelocity.GetX() > 0)
		{
			CTextureManager::Instance()->DrawFrame(m_sTextureID,
				int(m_vPosition.GetX() - cPos.GetX()), int(m_vPosition.GetY() - cPos.GetY()),
				m_iWidth, m_iHeight, m_iCurrentRow, m_iCurrentFrame,
				CBaseGame::Instance()->GetRenderer(), SDL_FLIP_HORIZONTAL);
		}
		else
		{
			CTextureManager::Instance()->DrawFrame(m_sTextureID,
				int(m_vPosition.GetX() - cPos.GetX()), int(m_vPosition.GetY() - cPos.GetY()),
				m_iWidth, m_iHeight, m_iCurrentRow, m_iCurrentFrame,
				CBaseGame::Instance()->GetRenderer());
		}
	}
}

void CMapObject::DoDraw(bool newVis)
{
	bDrawObject = newVis;
}