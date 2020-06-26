#pragma once

#ifndef __LEVELPARSER_H__
#define __LEVELPARSER_H__

#include <rapidjson/document.h>
#include <vector>
#include <string>

class Level;
struct Tileset;
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

	void parseTextures(const rapidjson::Value* pTextureRoot);

	void parseBackgroundColour(const std::string* colourVal);

	void parseObjectLayer(const rapidjson::Value* pObjectVal,
		std::vector<Layer*>* pLayer);

	int m_tileSize;
	int m_width;
	int m_height;
};
#endif