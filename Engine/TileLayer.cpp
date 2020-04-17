#include "TextureManager.h"
#include "Camera.h"
#include "TileLayer.h"

void TileLayer::OnPlay()
{
}

void TileLayer::Destroy()
{
}

void TileLayer::OnThink()
{
	m_position += m_velocity;
}

void TileLayer::Draw()
{
	Vector2D cCamPos = Camera::Instance()->GetPositionT();
	Vector2D vPortSz = Game::Instance()->GetViewportSize();

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
			if (((j * m_tileSize)) - cCamPos.GetX() < -m_tileSize || ((j * m_tileSize)) - cCamPos.GetX() > vPortSz.GetX())
			{
				continue;
			}

			if (((i * m_tileSize)) - cCamPos.GetY() < -m_tileSize || ((i * m_tileSize)) - cCamPos.GetY() > vPortSz.GetY())
			{
				continue;
			}

			Tileset tileset = GetTilesetByID(id);

			id--;

			TextureManager::Instance()->DrawTile(tileset.Name, tileset.Margin, tileset.Spacing,
				(int)(((j * m_tileSize)) - cCamPos.GetX()), (int)(((i * m_tileSize)) - cCamPos.GetY()), m_tileSize, m_tileSize, // X, Y, width and height
				(id - (tileset.FirstGID - 1)) / tileset.NumColumns, (id - (tileset.FirstGID - 1)) % tileset.NumColumns, // Row and frame
				Game::Instance()->GetRenderer());
		}
	}
}

Tileset TileLayer::GetTilesetByID(int tileID)
{
	for (int i = 0; i < m_tilesets.size(); i++)
	{
		if (i + 1 <= m_tilesets.size() - 1)
		{
			if (tileID >= m_tilesets[i].FirstGID && tileID < m_tilesets[i + 1].FirstGID)
			{
				return m_tilesets[i];
			}
		}
		else
		{
			return m_tilesets[i];
		}

		std::cout << "Cannot find tileset, returning an empty tileset\n";
	}

	return Tileset();
}
