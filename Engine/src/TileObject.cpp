#include "TileObject.h"
#include "Game.h"
#include "TextureManager.h"
#include "Level.h"
#include "Camera.h"

void CTileObject::Load(const CObjectParams* pParams)
{
	CMapObject::Load(pParams);

	m_gid = pParams->GetTileID();
	m_tileset = pParams->GetTileset();
}

void CTileObject::Draw()
{
	if ((bDrawObject || CBaseGame::Instance()->ShowDebugTiles())
		&& m_gid != -1)
	{
		CVector2D cPos = CCamera::Instance()->GetPositionT();

		CTextureManager::Instance()->DrawTile(m_tileset->Name, m_tileset->Margin, m_tileset->Spacing,
			int(m_position.GetX() - cPos.GetX()), int(m_position.GetY() - cPos.GetY()), 
			32, 32, // X, Y, width and height
			(m_gid - (m_tileset->FirstGID )) / m_tileset->NumColumns, (m_gid - (m_tileset->FirstGID )) % m_tileset->NumColumns, // Row and frame
			CBaseGame::Instance()->GetRenderer());
	}
}
