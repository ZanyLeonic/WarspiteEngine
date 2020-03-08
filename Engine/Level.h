#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>
#include <vector>

#include "Vector2D.h"
#include "Layer.h"

struct Tileset
{
	int FirstGID = 0;
	int TileWidth = 0;
	int TileHeight = 0;
	int Spacing = 0;
	int Margin = 0;
	int Width = 0;
	int Height = 0;
	int NumColumns = 0;
	std::string Name = "";
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