#pragma once

#ifndef __LEVELPARSER_H__
#define __LEVELPARSER_H__

#include <rapidjson/document.h>
#include <vector>

class Level;
class Tileset;
class Layer;

class LevelParser
{
public:
	Level* ParseLevel(const char* levelFile);

private:
	void parseTilesets(const rapidjson::Value* pTilesetRoot,
		std::vector<Tileset>* pTilesets);

	void parseTileLayer(const rapidjson::Value* pTileElement,
		std::vector<Layer*>* pLayers,
		const std::vector<Tileset>* pTilesets);

	int m_tileSize;
	int m_width;
	int m_height;
};
#endif