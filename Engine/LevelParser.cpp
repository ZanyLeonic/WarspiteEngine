#include "LevelParser.h"

Level* LevelParser::ParseLevel(const char* levelFile)
{
	return nullptr;
}

void LevelParser::parseTilesets(const rapidjson::Value* pTilesetRoot, std::vector<Tileset>* pTilesets)
{
}

void LevelParser::parseTileLayer(const rapidjson::Value* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets)
{
}
