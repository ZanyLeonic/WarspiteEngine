#pragma once

#ifndef __LEVELPARSER_H__
#define __LEVELPARSER_H__

#include <rapidjson/document.h>
#include <vector>
#include <string>

enum class MapProperties
{
	PROP_INVALID,
	PROP_SCRIPT,
	PROP_TEXTUREID,
	PROP_TEXWIDTH,
	PROP_TEXHEIGHT,
	PROP_NUMFRAMES,
	PROP_ANIMSPEED,
	PROP_ONCLICKCALL,
	PROP_ONENTERCALL,
	PROP_ONLEAVECALL,
	PROP_SOUNDPATH
};

class CLevel;
struct STileset;
class ILayer;

class CLevelParser
{
public:
	CLevel* ParseLevel(const char* levelFile);

private:
	static MapProperties GetMapProp(const std::string prop);
	
	void parseTilesets(const rapidjson::Value* pTilesetRoot,
		std::vector<STileset>* pTilesets);

	void parseTileLayer(const rapidjson::Value* pTileElement,
		std::vector<ILayer*>* pLayers,
		const std::vector<STileset>* pTilesets);

	void parseFiles(const rapidjson::Value* pFileRoot);

	void parseBackgroundColour(const std::string* colourVal);

	void parseObjectLayer(const rapidjson::Value* pObjectVal,
		std::vector<ILayer*>* pLayer);

	int m_tileSize;
	int m_width;
	int m_height;
};
#endif