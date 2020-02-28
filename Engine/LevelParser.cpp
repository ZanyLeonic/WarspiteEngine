#include "LevelParser.h"
#include "Level.h"
#include "TextureManager.h"
#include "Game.h"

#include <iostream>
#include <rapidjson/filereadstream.h>

using namespace rapidjson;

Level* LevelParser::ParseLevel(const char* levelFile)
{
	Document tLevel;
	FILE* fp = fopen(levelFile, "rb");

	if (fp != NULL)
	{
		char readBuffer[4096];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		tLevel.ParseStream(is);

		// Have we parsed the JSON correctly?
		if (tLevel.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << levelFile << "\"\n";
			std::cout << tLevel.GetParseError() << "\n";

			return 0;
		}

		fclose(fp);

		Level* pLevel = new Level();

		assert(tLevel["height"].IsInt() && tLevel["width"].IsInt() && tLevel["tilewidth"].IsInt());

		m_height = tLevel["height"].GetInt();
		m_width = tLevel["width"].GetInt();
		m_tileSize = tLevel["tilewidth"].GetInt();

		assert(tLevel["tilesets"].IsArray());
		const Value& tilesets = tLevel["tilesets"].GetArray();

		for (SizeType i = 0; i < tilesets.Size(); i++)
		{
			parseTilesets(&tilesets[i], pLevel->GetTilesets());
		}

		assert(tLevel["layers"].IsArray());
		const Value& layers = tLevel["layers"].GetArray();

		for (SizeType i = 0; i < layers.Size(); i++)
		{
			parseTileLayer(&layers[i], pLevel->GetLayers(),
				pLevel->GetTilesets());
		}

		return pLevel;
	}

	return 0;
}

void LevelParser::parseTilesets(const rapidjson::Value* pTilesetRoot, std::vector<Tileset>* pTilesets)
{
	const Value::ConstObject& obj = pTilesetRoot->GetObject();

	if (obj.HasMember("source"))
	{
		Document tileset;

		const char* fileName = obj["source"].GetString();
		FILE* tHandle = fopen(fileName, "rb");

		if (tHandle != NULL)
		{
			char readBuffer[4096];
			FileReadStream is(tHandle, readBuffer, sizeof(readBuffer));

			tileset.ParseStream(is);

			// Have we parsed the JSON correctly?
			if (tileset.HasParseError())
			{
				std::cout << "An error has occurred when loading \"" << fileName << "\"\n";
				std::cout << tileset.GetParseError() << "\n";

				return;
			}

			fclose(tHandle);

			TextureManager::Instance()->Load(tileset["image"].GetString(),
				tileset["name"].GetString(), Game::Instance()->GetRenderer());

			Tileset ts;

			ts.Width = tileset["width"].GetInt();
			ts.Height = tileset["height"].GetInt();
			ts.FirstGID = tileset["firstgid"].GetInt();
			ts.TileWidth = tileset["tilewidth"].GetInt();
			ts.TileHeight = tileset["tileheight"].GetInt();
			ts.Spacing = tileset["spacing"].GetInt();
			ts.Margin = tileset["margin"].GetInt();

			ts.Name = tileset["name"].GetString();

			ts.NumColumns = ts.Width / (ts.TileWidth + ts.Spacing);

			pTilesets->push_back(ts);
		}
	}
	else
	{
		throw "Unimplemented!";
	}
}

void LevelParser::parseTileLayer(const rapidjson::Value* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets)
{
}
