#include "TextureManager.h"
#include "Camera.h"
#include "TileLayer.h"

void TileLayer::OnThink()
{
	m_position += m_velocity;
}

void TileLayer::Draw()
{
	int x, y, x2, y2 = 0;

	x = m_position.GetX() / m_tileSize;
	y = m_position.GetY() / m_tileSize;

	x2 = int(m_position.GetX()) % m_tileSize;
	y2 = int(m_position.GetY()) % m_tileSize;

	for (int i = 0; i < m_numRows; i++)
	{
		for (int j = 0; j < m_numColumns; j++)
		{
			int id = m_tileIDs[i + y][j + x];

			if (id == 0)
			{
				continue;
			}
			
			Vector2D cCamPos = Camera::Instance()->GetPosition();
			Vector2D vPortSz = Game::Instance()->GetViewportSize();

			// Don't render tiles that cannot be seen by the camera
			if (((j * m_tileSize) - x2) - cCamPos.GetX() < -m_tileSize || ((j * m_tileSize) - x2) - cCamPos.GetX() > vPortSz.GetX())
			{
				continue;
			}

			if (((i * m_tileSize) - y2) - cCamPos.GetY() < -m_tileSize || ((i * m_tileSize) - y2) - cCamPos.GetY() > vPortSz.GetY())
			{
				continue;
			}

			Tileset tileset = GetTilesetByID(id);

			id--;

			TextureManager::Instance()->DrawTile(tileset.Name, tileset.Margin, tileset.Spacing,
				((j * m_tileSize) - x2) - cCamPos.GetX(), ((i * m_tileSize) - y2) - cCamPos.GetY(), m_tileSize, m_tileSize, // X, Y, width and height
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
		
		return Tileset();
	}
}
