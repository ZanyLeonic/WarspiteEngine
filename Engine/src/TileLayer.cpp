#include "TextureManager.h"
#include "Camera.h"
#include "TileLayer.h"
#include <iostream>
#include <spdlog/spdlog.h>

void CTileLayer::OnPlay()
{
}

void CTileLayer::Destroy()
{
}

void CTileLayer::OnThink()
{
	m_vPosition += m_vVelocity;
}

void CTileLayer::Draw()
{
	CVector2D cCamPos = CCamera::Instance()->GetPositionT();
	CVector2D* vPortSz = CBaseGame::Instance()->GetViewportSize();

	for (int i = 0; i < m_numRows; i++)
	{
		for (int j = 0; j < m_numColumns; j++)
		{
			int id = m_tileIDs[i][j];

			if (id == 0)
			{
				continue;
			}

			// Don't render tiles that cannot be seen by the camera
			if (((j * m_tileSize)) - cCamPos.GetX() < -m_tileSize || ((j * m_tileSize)) - cCamPos.GetX() > vPortSz->GetX())
			{
				continue;
			}

			if (((i * m_tileSize)) - cCamPos.GetY() < -m_tileSize || ((i * m_tileSize)) - cCamPos.GetY() > vPortSz->GetY())
			{
				continue;
			}

			STileset tileset = GetTilesetByID(id);

			id--;

			assert(tileset.NumColumns > 0);

			CTextureManager::Instance()->DrawTile(tileset.Name, tileset.Margin, tileset.Spacing,
				(((j * m_tileSize)) - cCamPos.GetX()), (((i * m_tileSize)) - cCamPos.GetY()), m_tileSize, m_tileSize, // X, Y, width and height
				(id - (tileset.FirstGID - 1)) / tileset.NumColumns, (id - (tileset.FirstGID - 1)) % tileset.NumColumns, // Row and frame
				CBaseGame::Instance()->GetRenderer());
		}
	}
}

STileset CTileLayer::GetTilesetByID(int tileID)
{
	for (size_t i = 0; i < m_vTilesets.size(); i++)
	{
		if (i + (size_t)1 <= m_vTilesets.size() - 1)
		{
			if (tileID >= m_vTilesets[i].FirstGID && tileID < m_vTilesets[i + (size_t)1].FirstGID)
			{
				return m_vTilesets[i];
			}
		}
		else
		{
			return m_vTilesets[i];
		}

		spdlog::warn("Cannot find tileset for TileID ({}), returning an empty tileset", tileID);
	}

	return STileset();
}
