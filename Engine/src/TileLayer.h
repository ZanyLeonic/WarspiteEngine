#pragma once
#ifndef __TILELAYER_H__
#define __TILELAYER_H__

#include <vector>

#include "Game.h"
#include "Level.h"
#include "Layer.h"
#include "Vector2D.h"

class CTileLayer :
	public ILayer
{
public:
	CTileLayer(int tileSize, int mapWidth, int mapHeight, 
		const std::vector<STileset>& tilesets)
		: m_tileSize(tileSize)
	{
		m_vTilesets = tilesets;

		m_numColumns = mapWidth;
		m_numRows = mapHeight;
	};

	virtual void OnPlay();
	virtual void Destroy();

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

	STileset GetTilesetByID(int tileID);

private:
	int m_numColumns;
	int m_numRows;
	int m_tileSize;
	
	CVector2D m_lastCamPos;
	CVector2D m_vPosition;
	CVector2D m_vVelocity;

	std::vector<STileset> m_vTilesets;
	std::vector<std::vector<int>> m_tileIDs;
};

#endif