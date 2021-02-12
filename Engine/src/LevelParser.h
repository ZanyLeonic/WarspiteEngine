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
	static CLevel* ParseLevel(const char* levelFile);

private:
	static MapProperties GetMapProp(const std::string prop);
	
	static void parseTilesets(const rapidjson::Value* pTilesetRoot,
		std::vector<STileset>* pTilesets);

	static void parseTileLayer(const rapidjson::Value* pTileElement,
		CLevel* pLevel);

	static void parseFiles(const rapidjson::Value* pFileRoot);

	static void parseBackgroundColour(const std::string* colourVal);

	static void parseObjectLayer(const rapidjson::Value* pObjectVal,
		std::vector<ILayer*>* pLayer, std::vector<STileset>* pTilesets);

	static STileset* GetTilesetByID(int tileID, std::vector<STileset>* pTilesets);
};
#endif