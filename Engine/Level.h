#pragma once
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <string>
#include <vector>

#include "Vector2D.h"
#include "Layer.h"
#include "WarspiteUtil.h"

struct STileset
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

class CLevel
{
public:
	~CLevel() {};

	void OnPlay();
	void Destroy();

	void OnThink();
	void Draw();

	std::vector<STileset>* GetTilesets()
	{
		return &m_tilesets;
	}

	std::vector<ILayer*>* GetLayers()
	{
		return &m_layers;
	}

	std::string GetName() const
	{
		return WarspiteUtil::GetFileName(m_path);
	}
	
	CVector2D m_LevelSize;

private:
	friend class CLevelParser;
	CLevel(std::string& path) : m_path(path) {}

	std::vector<STileset> m_tilesets;
	std::vector<ILayer*> m_layers;

	std::string m_path;
};
#endif