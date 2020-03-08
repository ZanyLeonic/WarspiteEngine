#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>
#include <vector>

#include "Vector2D.h"
#include "Layer.h"

struct Tileset
{
	int FirstGID;
	int TileWidth;
	int TileHeight;
	int Spacing;
	int Margin;
	int Width;
	int Height;
	int NumColumns;
	std::string Name;
};

class Level
{
public:
	~Level() {};

	void OnThink();
	void Draw();

	std::vector<Tileset>* GetTilesets()
	{
		return &m_tilesets;
	}

	std::vector<Layer*>* GetLayers()
	{
		return &m_layers;
	};

	Vector2D m_LevelSize;

private:

	friend class LevelParser;
	Level();

	std::vector<Tileset> m_tilesets;
	std::vector<Layer*> m_layers;
};

#endif