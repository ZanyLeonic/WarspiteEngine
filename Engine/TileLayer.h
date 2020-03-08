#pragma once
#ifndef __TILELAYER_H__
#define __TILELAYER_H__

#include <vector>

#include "Game.h"
#include "Level.h"
#include "Layer.h"
#include "Vector2D.h"

class TileLayer :
	public Layer
{
public:

	TileLayer(int tileSize, int mapWidth, int mapHeight, 
		const std::vector<Tileset>& tilesets)
		: m_tileSize(tileSize)
	{
		m_tilesets = tilesets;

		m_numColumns = mapWidth;
		m_numRows = mapHeight;
	};

	virtual void OnThink();
	virtual void Draw();

	void SetTileIDs(const std::vector<std::vector<int>>& data)
	{
		m_tileIDs = data;
	}

	void SetTileSize(int tileSize)
	{
		m_tileSize = tileSize;
	}

	Tileset GetTilesetByID(int tileID);

private:
	int m_numColumns;
	int m_numRows;
	int m_tileSize;

	Vector2D m_position;
	Vector2D m_velocity;

	std::vector<Tileset>& m_tilesets = std::vector<Tileset>();
	std::vector<std::vector<int>> m_tileIDs;
};

#endif

