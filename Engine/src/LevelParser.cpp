#include "LevelParser.h"
#include "Level.h"
#include "ObjectLayer.h"
#include "EngineFileSystem.h"
#include "TextureManager.h"
#include "ScriptManager.h"
#include "GameObjectDictionary.h"
#include "ObjectParams.h"
#include "Game.h"
#include "TileLayer.h"
#include "WarspiteUtil.h"

#include <spdlog/spdlog.h>
#include "etc/Base64.h"
#include <zlib.h>

#undef GetObject

using namespace rapidjson;

CLevel* CLevelParser::ParseLevel(const char* levelFile)
{
	Document tLevel;

	if (CEngineFileSystem::ReadJSON(levelFile, &tLevel))
	{
		std::string sL = levelFile;
		CLevel* pLevel = new CLevel(sL);

		assert(tLevel["height"].IsInt() && tLevel["width"].IsInt() && tLevel["tilewidth"].IsInt());

		pLevel->m_height = tLevel["height"].GetInt();
		pLevel->m_width = tLevel["width"].GetInt();
		pLevel->m_tileSize = tLevel["tilewidth"].GetInt();

		spdlog::debug("*** Level information start ***");
		spdlog::debug("Dimensions: {}x{}", pLevel->m_width, pLevel->m_height);
		spdlog::debug("Tile Width: {}", pLevel->m_tileSize);

		std::string bgColour = tLevel.HasMember("backgroundcolor") ? tLevel["backgroundcolor"].GetString() : "#FFFFFF";

		parseBackgroundColour(&bgColour);

		// multiply by tilesize since we are using pixels
		pLevel->m_LevelSize.SetX((float)pLevel->m_width * (float)pLevel->m_tileSize);
		pLevel->m_LevelSize.SetY((float)pLevel->m_height * (float)pLevel->m_tileSize);

		spdlog::debug("Level size: {}x{}", pLevel->m_LevelSize.GetX(), pLevel->m_LevelSize.GetY());

		assert(tLevel["tilesets"].IsArray());
		const Value& tilesets = tLevel["tilesets"].GetArray();

		spdlog::debug("Tilesets used: {}", tilesets.Size());

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

			spdlog::debug("Number of external files used: {}", properties.Size());

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

		spdlog::debug("Layers defined: {}", layers.Size());

		for (SizeType i = 0; i < layers.Size(); i++)
		{
			std::string t = layers[i]["type"].GetString();

			if(t == "tilelayer")
			{
				parseTileLayer(&layers[i], pLevel);
			}
			else if (t == "objectgroup")
			{
				parseObjectLayer(&layers[i], pLevel->GetLayers(), pLevel->GetTilesets());
			}			
		}

		spdlog::debug("*** Level information end ***");

		return pLevel;
	}

	return 0;
}

MapProperties CLevelParser::GetMapProp(const std::string prop)
{
	static const std::map<std::string, MapProperties> propStrings
	{
		{"runScript",		 MapProperties::PROP_SCRIPT},
		{"textureID",		 MapProperties::PROP_TEXTUREID},
		{"textureWidth",	 MapProperties::PROP_TEXWIDTH},
		{"textureHeight",	 MapProperties::PROP_TEXHEIGHT},
		{"numFrames",		 MapProperties::PROP_NUMFRAMES},
		{"animSpeed",		 MapProperties::PROP_ANIMSPEED},
		{"onClickCallback",  MapProperties::PROP_ONCLICKCALL},
		{"onEnterCallback",  MapProperties::PROP_ONENTERCALL},
		{"onLeaveCallback",  MapProperties::PROP_ONLEAVECALL},
		{"soundPath",		 MapProperties::PROP_SOUNDPATH},
		{"targetDoorID",	 MapProperties::PROP_DOORTARGET},
		{"doorWorldTexture", MapProperties::PROP_DOORWORLDTEXTURE}
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
	Document obj;

	obj.CopyFrom(*pTilesetRoot, obj.GetAllocator());
	
	if (obj.HasMember("source"))
	{
		Document tileset;

		const char* fileName = obj["source"].GetString();

		// External Tilesets
		if (CEngineFileSystem::ReadJSON(CEngineFileSystem::ResolvePath(fileName, CEngineFileSystem::EPathType::TILESET), &tileset))
		{
			const Value& t = tileset.GetObject();
			int opacity = 255;

			if (t.HasMember("opacity"))
			{
				opacity = t["opacity"].GetInt();
			}

			CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath(t["image"].GetString(),CEngineFileSystem::EPathType::TEXTURE),
				t["name"].GetString(), CBaseGame::Instance()->GetRenderer(), opacity);

			STileset ts = GatherTilesetInfo(&t);

			ts.FirstGID = obj["firstgid"].GetInt();
			
			pTilesets->push_back(ts);
		}
	}
	// Embedded Tilesets
	else
	{
		const Value& t = obj.GetObject();
		int opacity = 255;

		if (t.HasMember("opacity"))
		{
			opacity = t["opacity"].GetInt();
		}

		CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath(t["image"].GetString(), CEngineFileSystem::EPathType::TEXTURE),
			t["name"].GetString(), CBaseGame::Instance()->GetRenderer(), opacity);

		STileset ts = GatherTilesetInfo(&t);

		ts.FirstGID = t["firstgid"].GetInt();

		pTilesets->push_back(ts);
	}
}

void CLevelParser::parseTileLayer(const rapidjson::Value* pTileElement, CLevel* pLevel)
{
	spdlog::debug("Parsing TileLayer...");

	// Make the JSON value an object to manipulate better.
	const Value::ConstObject& obj = pTileElement->GetObject();

	// Another with a new instance of TileLayer to work with and return.
	CTileLayer* pTileLayer = new CTileLayer(pLevel->m_tileSize, pLevel->m_width, pLevel->m_height, *pLevel->GetTilesets());

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
	uLongf numGids = pLevel->m_width * pLevel->m_height * sizeof(int);

	// Our buffer to store the uncompressed map data in.
	std::vector<unsigned> gids(numGids);

	// ZLib uncompress method ? takes our buffer in the 
	// form of the type Bytef pointer, the size of the buffer, 
	// the compressed data and the compressed data size.
	uncompress((Bytef*)&gids[0], &numGids, 
		(const Bytef*)decodedIDs.c_str(), (uLong)decodedIDs.size());

	// Populating our 2D array with blank rows
	std::vector<int> layerRow(pLevel->m_width);

	// Add a row corrisponding to the height of the map we are trying to load.
	for (int j = 0; j < pLevel->m_height; j++)
	{
		data.push_back(layerRow);
	}

	// Then we fill the 2D array with the uncompressed tile data found in the map.
	for (int rows = 0; rows < pLevel->m_height; rows++)
	{
		for (int cols = 0; cols < pLevel->m_width; cols++)
		{
			data[rows][cols] = gids[rows * pLevel->m_width + cols];
		}
	}

	// Finally, we provide the TileLayer instance with the tile data and then 
	// push it into the provided layers we were given.
	pTileLayer->SetTileIDs(data);
	pLevel->GetLayers()->push_back(pTileLayer);
}

void CLevelParser::parseFiles(const rapidjson::Value* pFileRoot)
{
	// Get the correct type of the value. (should be an object)
	const Value::ConstObject& o = pFileRoot->GetObject();

	spdlog::debug("Processing file: \"{}\"...", o["value"].GetString());

	if (CWarspiteUtil::GetFileExtenstion(o["value"].GetString()) == ".py")
	{
		CScriptManager::Instance()->Load(SGameScript::file(o["name"].GetString(), CEngineFileSystem::ResolvePath(o["value"].GetString(), 
										 CEngineFileSystem::EPathType::SCRIPT)));
		spdlog::debug("Loaded script \"{}\"", CEngineFileSystem::ResolvePath(o["value"].GetString(),
			CEngineFileSystem::EPathType::SCRIPT));
		return;
	}
	
	// Load the texture via the TextureManager with the info inside the object.
	CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath(o["value"].GetString(), CEngineFileSystem::EPathType::TEXTURE),
                                      o["name"].GetString(),CBaseGame::Instance()->GetRenderer());
	spdlog::debug("Loaded image \"{}\"", CEngineFileSystem::ResolvePath(o["value"].GetString(),
		CEngineFileSystem::EPathType::TEXTURE));
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
		spdlog::warn("Warning: Unrecongised or unsupported colour value!");
		spdlog::warn("Value: {}", *colourVal);
		break;
	};

	SDL_SetRenderDrawColor(CBaseGame::Instance()->GetRenderer(), r, g, b, a);
}

void CLevelParser::parseObjectLayer(const rapidjson::Value* pObjectVal, std::vector<ILayer*>* pLayer, std::vector<STileset>* pTilesets)
{
	spdlog::debug("Parsing ObjectLayer...");

	// Get our JSON values to their types.
	const Value::ConstObject& o = pObjectVal->GetObject();
	const Value::ConstArray& a = o["objects"].GetArray();

	// Create our ObjectLayer we are filling with data.
	CObjectLayer* pObjectLayer = new CObjectLayer();
	
	spdlog::debug("GameObjects defined: {}", a.Size());

	// iterate through each object we have.
	for (SizeType i = 0; i < a.Size(); i++)
	{
		// get our current object as a JSON object to get data from.
		const Value::ConstObject& b = a[i].GetObject();

		// Get the desired coordinates
		CObjectParams* pOP = new CObjectParams(b["x"].GetFloat(), b["y"].GetFloat());

		if (b.HasMember("gid") && b["gid"].IsInt())
		{
			const int gid = b["gid"].GetInt();
			STileset* t = GetTilesetByID(gid, pTilesets);

			if (t->Tiles.size() > 0)
			{
				for (size_t i = 0; i < t->Tiles.size(); i++)
				{
					int aID = gid - t->FirstGID;

					if (t->Tiles[i].ID == aID)
					{
						pOP->SetFactoryID(t->Tiles[i].Type);
						pOP->SetTileID(gid);
						pOP->SetTileset(t);

						// TODO: Find the issue why the tile objects are offset on Y
						pOP->SetY(pOP->GetY() - t->TileHeight);

						break;
					}
				}
			}
		}
		else
		{
			pOP->SetFactoryID(b["type"].GetString());
		}

		pOP->SetName(b["name"].GetString());

		spdlog::debug("*** Load object start ***");

		spdlog::debug("Object name: {}", pOP->GetName());
		spdlog::debug("Factory name: {}", pOP->GetFactoryID());
		spdlog::debug("Object position: ({}, {})", pOP->GetX(), pOP->GetY());

		// Create the object that is defined
		std::shared_ptr<IGameObject> pGameObject = CGameObjectDictionary::Instance()
			->Create(pOP->GetFactoryID());

		// fill in any additional information (if provided.)
		if (b.HasMember("properties"))
		{
			// iterate
			const Value::ConstArray& d = b["properties"].GetArray();
			
			spdlog::debug("Number of properties defined: {}", d.Size());
			spdlog::debug("*** Property loading start ***");

			for (SizeType j = 0; j < d.Size(); j++)
			{
				std::string propName = d[j]["name"].GetString();

				if (d[j]["value"].IsString())
				{
					spdlog::debug("Name: \"{}\" Value: \"{}\"", propName, d[j]["value"].GetString());
				}
				else
				{
					spdlog::debug("Name: \"{}\" Value: \"{}\"", propName, d[j]["value"].GetInt());
				}

				switch (GetMapProp(propName))
				{
				case MapProperties::PROP_SCRIPT:
					pOP->SetScript(d[j]["value"].GetString());
					break;
				case MapProperties::PROP_TEXTUREID:
					pOP->SetTextureID(d[j]["value"].GetString());
					break;
				case MapProperties::PROP_TEXWIDTH:
					pOP->SetWidth(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_TEXHEIGHT:
					pOP->SetHeight(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_NUMFRAMES:
					pOP->SetNumFrames(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_ANIMSPEED:
					pOP->SetAnimSpeed(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_ONCLICKCALL:
					pOP->SetOnClick(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_ONENTERCALL:
					pOP->SetOnEnter(d[j]["value"].GetInt());
					break;
				case MapProperties::PROP_ONLEAVECALL:
					pOP->SetOnLeave(d[j]["value"].GetInt());
					break;	
				case MapProperties::PROP_SOUNDPATH:
					pOP->SetSoundPath(d[j]["value"].GetString());
					break;
				case MapProperties::PROP_DOORTARGET:
					pOP->SetDoorTargetID(d[j]["value"].GetString());
					break;
				case MapProperties::PROP_DOORWORLDTEXTURE:
					pOP->SetDoorWorldTexture(d[j]["value"].GetString());
					break;
				default:
					// Future proofing incase new properties get added for newer engine version.
					spdlog::warn("Warning: Unrecognised property \"{}\"!", propName);
					break;
				}
			}
			spdlog::debug("*** Property loading end ***");
		}

		// Check if the pointer is valid
		if (pGameObject != 0)
		{
			// intialise the object with the data obtained.
			pGameObject->Load(pOP);
			pObjectLayer->GetGameObjects()->push_back(pGameObject);
		}
		
		spdlog::debug("*** Load object end ***");
	}

	// Add the object layer to the Level object
	pLayer->push_back(pObjectLayer);
}

STileset* CLevelParser::GetTilesetByID(int tileID, std::vector<STileset>* pTilesets)
{
	std::vector<STileset> tilesets = *pTilesets;

	for (size_t i = 0; i < tilesets.size(); i++)
	{
		if (i + (size_t)1 <= tilesets.size() - 1)
		{
			if (tileID >= tilesets[i].FirstGID && tileID < tilesets[i + (size_t)1].FirstGID)
			{
				return &pTilesets->at(i);
			}
		}
		else
		{
			return &pTilesets->at(i);
		}
	}

	spdlog::warn("Cannot find tileset for TileID ({}), returning an empty tileset", tileID);
	return new STileset();
}

STileset CLevelParser::GatherTilesetInfo(const rapidjson::Value* pTilesetRoot)
{
	STileset ts;
	Value::ConstObject t = pTilesetRoot->GetObject();

	ts.Width = t["imagewidth"].GetInt();
	ts.Height = t["imageheight"].GetInt();

	ts.TileWidth = t["tilewidth"].GetInt();
	ts.TileHeight = t["tileheight"].GetInt();
	ts.Spacing = t["spacing"].GetInt();
	ts.Margin = t["margin"].GetInt();

	ts.Name = t["name"].GetString();

	ts.NumColumns = ts.Width / (ts.TileWidth + ts.Spacing);

	if (t.HasMember("tiles") && t["tiles"].IsArray())
	{
		const Value::ConstArray& d = t["tiles"].GetArray();

		for (SizeType i = 0; i < d.Size(); i++)
		{
			STileEntity te;

			te.ID = d[i]["id"].GetInt();
			te.Type = d[i]["type"].GetString();

			ts.Tiles.push_back(te);
		}
	}

	return ts;
}