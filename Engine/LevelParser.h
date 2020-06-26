#pragma once

#ifndef __LEVELPARSER_H__
#define __LEVELPARSER_H__

#include <rapidjson/document.h>
#include <vector>
#include <string>

class CLevel;
struct STileset;
class ILayer;

class CLevelParser
{
public:
	CLevel* ParseLevel(const char* levelFile);

private:
	void parseTilesets(const rapidjson::Value* pTilesetRoot,
		std::vector<STileset>* pTilesets);

	void parseTileLayer(const rapidjson::Value* pTileElement,
		std::vector<ILayer*>* pLayers,
		const std::vector<STileset>* pTilesets);

	void parseTextures(const rapidjson::Value* pTextureRoot);

	void parseBackgroundColour(const std::string* colourVal);

	void parseObjectLayer(const rapidjson::Value* pObjectVal,
		std::vector<ILayer*>* pLayer);

	int m_tileSize;
	int m_width;
	int m_height;
};
#endif