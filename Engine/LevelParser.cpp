#include "LevelParser.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "TextureManager.h"
#include "ScriptManager.h"
#include "GameObjectDictionary.h"
#include "ObjectParams.h"
#include "Game.h"
#include "TileLayer.h"
#include "WarspiteUtil.h"

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

CLevel* CLevelParser::ParseLevel(const char* levelFile)
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

		std::string sL = levelFile;
		CLevel* pLevel = new CLevel(sL);

		assert(tLevel["height"].IsInt() && tLevel["width"].IsInt() && tLevel["tilewidth"].IsInt());

		m_height = tLevel["height"].GetInt();
		m_width = tLevel["width"].GetInt();
		m_tileSize = tLevel["tilewidth"].GetInt();

		std::string bgColour = tLevel.HasMember("backgroundcolor") ? tLevel["backgroundcolor"].GetString() : "#FFFFFF";

		parseBackgroundColour(&bgColour);

		// multiply by tilesize since we are using pixels
		pLevel->m_LevelSize.SetX((int)(m_width * m_tileSize));
		pLevel->m_LevelSize.SetY((int)(m_height * m_tileSize));

		assert(tLevel["tilesets"].IsArray());
		const Value& tilesets = tLevel["tilesets"].GetArray();

		for (SizeType i = 0; i < tilesets.Size(); i++)
		{
			parseTilesets(&tilesets[i], pLevel->GetTilesets());
		}

		// Does the level have properties?
		if (tLevel.HasMember("properties"))
		{
			// Storing in a variable first to prevent implicit conversion
			// (some C++ compilers do not like that.)
			const Value& properties = tLevel["properties"].GetArray();
			for (SizeType i = 0; i < properties.Size(); i++)
			{
				// Ignore any other types of properties when attempting to load textures + scripts.
				if (properties[i]["type"] == "file")
					parseFiles(&properties[i]);
			}
		}

		// If we don't have an array - something is wrong.
		assert(tLevel["layers"].IsArray());
		const Value& layers = tLevel["layers"].GetArray();

		for (SizeType i = 0; i < layers.Size(); i++)
		{
			std::string t = layers[i]["type"].GetString();

			if(t == "tilelayer")
			{
				parseTileLayer(&layers[i], pLevel->GetLayers(),
					pLevel->GetTilesets());
			}
			else if (t == "objectgroup")
			{
				parseObjectLayer(&layers[i], pLevel->GetLayers());
			}			
		}

		return pLevel;
	}

	return 0;
}

MapProperties CLevelParser::GetMapProp(const std::string prop)
{
	static const std::map<std::string, MapProperties> propStrings
	{
		{"runScript",		MapProperties::PROP_SCRIPT},
		{"textureID",		MapProperties::PROP_TEXTUREID},
		{"textureWidth",	MapProperties::PROP_TEXWIDTH},
		{"textureHeight",	MapProperties::PROP_TEXHEIGHT},
		{"numFrames",		MapProperties::PROP_NUMFRAMES},
		{"animSpeed",		MapProperties::PROP_ANIMSPEED},
		{"onClickCallback", MapProperties::PROP_ONCLICKCALL},
		{"onEnterCallback", MapProperties::PROP_ONENTERCALL},
		{"onLeaveCallback", MapProperties::PROP_ONLEAVECALL}
	};

	auto itr = propStrings.find(prop);
	if (itr != propStrings.end())
	{
		return itr->second;
	}

	return MapProperties::PROP_INVALID;
}

void CLevelParser::parseTilesets(const rapidjson::Value* pTilesetRoot, std::vector<STileset>* pTilesets)
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

			CTextureManager::Instance()->Load(t["image"].GetString(),
				t["name"].GetString(), CGame::Instance()->GetRenderer());

			STileset ts;

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

void CLevelParser::parseTileLayer(const rapidjson::Value* pTileElement, std::vector<ILayer*>* pLayers, const std::vector<STileset>* pTilesets)
{
	// Make the JSON value an object to manipulate better.
	const Value::ConstObject& obj = pTileElement->GetObject();

	// Another with a new instance of TileLayer to work with and return.
	CTileLayer* pTileLayer = new CTileLayer(m_tileSize, m_width, m_height, *pTilesets);

	// the final decoded tile data in a 2D integer array
	std::vector<std::vector<int>> data;

	// The first one to hold the decoded base64 and the rawData string 
	// to hold the base64 from the file.
	std::string decodedIDs;
	std::string rawData = obj["data"].GetString();

	// Decoding the base64 into a string
	decodedIDs = base64_decode(rawData);

	// The desired size of the buffer ? map dimensions 
	// multiplied by the size of an integer since the 
	// destination is a int vector.
	uLongf numGids = m_width * m_height * sizeof(int);

	// Our buffer to store the uncompressed map data in.
	std::vector<unsigned> gids(numGids);

	// ZLib uncompress method ? takes our buffer in the 
	// form of the type Bytef pointer, the size of the buffer, 
	// the compressed data and the compressed data size.
	uncompress((Bytef*)&gids[0], &numGids, 
		(const Bytef*)decodedIDs.c_str(), (uLong)decodedIDs.size());

	// Populating our 2D array with blank rows
	std::vector<int> layerRow(m_width);

	// Add a row corrisponding to the height of the map we are trying to load.
	for (int j = 0; j < m_height; j++)
	{
		data.push_back(layerRow);
	}

	// Then we fill the 2D array with the uncompressed tile data found in the map.
	for (int rows = 0; rows < m_height; rows++)
	{
		for (int cols = 0; cols < m_width; cols++)
		{
			data[rows][cols] = gids[rows * m_width + cols];
		}
	}

	// Finally, we provide the TileLayer instance with the tile data and then 
	// push it into the provided layers we were given.
	pTileLayer->SetTileIDs(data);
	pLayers->push_back(pTileLayer);
}

void CLevelParser::parseFiles(const rapidjson::Value* pFileRoot)
{
	// Get the correct type of the value. (should be an object)
	const Value::ConstObject& o = pFileRoot->GetObject();

	if (CWarspiteUtil::GetFileExtenstion(o["value"].GetString()) == ".py")
	{
		CScriptManager::Instance()->Load(SGameScript::file(o["name"].GetString(), o["value"].GetString()));
		return;
	}
	
	// Load the texture via the TextureManager with the info inside the object.
	CTextureManager::Instance()->Load(o["value"].GetString(), o["name"].GetString(),
		CGame::Instance()->GetRenderer());
}

void CLevelParser::parseBackgroundColour(const std::string* colourVal)
{
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;

	switch (colourVal->length())
	{
	// No alpha channel
	case 7:
		// Red
		r = std::stoi(colourVal->substr(1, 2), 0, 16);
		// Green
		g = std::stoi(colourVal->substr(3, 2), 0, 16);
		// Blue
		b = std::stoi(colourVal->substr(5, 2), 0, 16);
		break;
	
	// Alpha channel
	case 9:
		a = std::stoi(colourVal->substr(1, 2), 0, 16);
		r = std::stoi(colourVal->substr(3, 2), 0, 16);
		g = std::stoi(colourVal->substr(5, 2), 0, 16);
		b = std::stoi(colourVal->substr(7, 2), 0, 16);
		break;

	default:
		std::cout << "Warning: Unrecongised or unsupported colour value!" << std::endl << "Value: " << colourVal << std::endl;
		break;
	};

	SDL_SetRenderDrawColor(CGame::Instance()->GetRenderer(), r, g, b, a);
}

void CLevelParser::parseObjectLayer(const rapidjson::Value* pObjectVal, std::vector<ILayer*>* pLayer)
{
	// Get our JSON values to their types.
	const Value::ConstObject& o = pObjectVal->GetObject();
	const Value::ConstArray& a = o["objects"].GetArray();

	// Create our ObjectLayer we are filling with data.
	CObjectLayer* pObjectLayer = new CObjectLayer();
	
	// iterate through each object we have.
	for (SizeType i = 0; i < a.Size(); i++)
	{
		// get our current object as a JSON object to get data from.
		const Value::ConstObject& b = a[i].GetObject();
		
		int x = 0, y = 0, width = 0, height = 0,
			onClickCallback = 0, onEnterCallback = 0, onLeaveCallback = 0;
		int numFrames = 1, animSpeed = 1;
		std::string textureID, objN, scriptName, type;

		// Get the desired coordinates
		x = b["x"].GetInt();
		y = b["y"].GetInt();
		
		type = b["type"].GetString();
		objN = b["name"].GetString();

		// Create the object that is defined
		IGameObject* pGameObject = CGameObjectDictionary::Instance()
			->Create(type);
		
		// fill in any additional information (if provided.)
		if (b.HasMember("properties"))
		{
			// iterate
			const Value::ConstArray& d = b["properties"].GetArray();
			
			for (SizeType j = 0; j < d.Size(); j++)
			{
				std::string propName = d[j]["name"].GetString();

				switch (GetMapProp(propName))
				{
				case MapProperties::PROP_SCRIPT:
					scriptName = d[j]["value"].GetString();
					break;
				case MapProperties::PROP_TEXTUREID:
					textureID = d[j]["value"].GetString();
					break;
				case MapProperties::PROP_TEXWIDTH:
					width = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_TEXHEIGHT:
					height = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_NUMFRAMES:
					numFrames = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_ANIMSPEED:
					animSpeed = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_ONCLICKCALL:
					onClickCallback = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_ONENTERCALL:
					onEnterCallback = d[j]["value"].GetInt();
					break;
				case MapProperties::PROP_ONLEAVECALL:
					onLeaveCallback = d[j]["value"].GetInt();
					break;	
				default:
					// Future proofing incase new properties get added for newer engine version.
					std::cout << "Warning: Unrecongised property \"" << propName << "\"!" << std::endl;
					break;
				}
			}
		}
		
		// intialise the object with the data obtained.
		pGameObject->Load(new CObjectParams((float)x, (float)y, width, height,
			textureID, animSpeed, numFrames, onClickCallback,
			onEnterCallback, onLeaveCallback, scriptName, objN, type));
		pObjectLayer->GetGameObjects()->push_back(pGameObject);
	}

	// Add the object layer to the Level object
	pLayer->push_back(pObjectLayer);
}