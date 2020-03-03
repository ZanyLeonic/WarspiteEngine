#include "LevelParser.h"
#include "Level.h"
#include "TileLayer.h"
#include "TextureManager.h"
#include "Game.h"

#include <iostream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/writer.h>
#include "etc/Base64.h"
#include <zlib.h>

using namespace rapidjson;

// Debug: Serializes JSON
std::string getJSONs(const Value* pStateRoot)
{
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	pStateRoot->Accept(writer);

	return sb.GetString();
}

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

	int fg = obj["firstgid"].GetInt();

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

			const Value& t = tileset.GetObject();

			TextureManager::Instance()->Load(t["image"].GetString(),
				t["name"].GetString(), Game::Instance()->GetRenderer());

			Tileset ts;

			std::cout << getJSONs(&t) << std::endl;

			ts.Width = t["imagewidth"].GetInt();
			ts.Height = t["imageheight"].GetInt();
			ts.FirstGID = fg;
			ts.TileWidth = t["tilewidth"].GetInt();
			ts.TileHeight = t["tileheight"].GetInt();
			ts.Spacing = t["spacing"].GetInt();
			ts.Margin = t["margin"].GetInt();

			ts.Name = t["name"].GetString();

			ts.NumColumns = ts.Width / (ts.TileWidth + ts.Spacing);

			pTilesets->push_back(ts);
		}
	}
	else
	{
		// throw "Unimplemented!";
	}
}

void LevelParser::parseTileLayer(const rapidjson::Value* pTileElement, std::vector<Layer*>* pLayers, const std::vector<Tileset>* pTilesets)
{
	const Value::ConstObject& obj = pTileElement->GetObject();

	TileLayer* pTileLayer = new TileLayer(m_tileSize, *pTilesets);

	// tiles
	std::vector<std::vector<int>> data;

	std::string decodedIDs;
	std::string rawData = obj["data"].GetString();

	decodedIDs = base64_decode(rawData);

	uLongf numGids = m_width * m_height * sizeof(int);
	std::vector<unsigned> gids(numGids);
	uncompress((Bytef*)&gids[0], &numGids, 
		(const Bytef*)decodedIDs.c_str(), decodedIDs.size());

	std::vector<int> layerRow(m_width);

	for (int j = 0; j < m_height; j++)
	{
		data.push_back(layerRow);
	}

	for (int rows = 0; rows < m_height; rows++)
	{
		for (int cols = 0; cols < m_width; cols++)
		{
			data[rows][cols] = gids[rows * m_width + cols];
		}

		pTileLayer->SetTileIDs(data);

		pLayers->push_back(pTileLayer);
	}
}
