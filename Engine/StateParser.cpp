#include "StateParser.h"
#include <cstdio>
#include <rapidjson/filereadstream.h>
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "Game.h"

bool StateParser::ParseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
{
	// JSON document
	rapidjson::Document jDoc;
	FILE* fp = fopen(stateFile, "r");

	// load state file
	if (fp != NULL)
	{
		char readBuffer[16384];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		jDoc.ParseStream(is);

		if (jDoc.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << stateFile << "\"\n";
			std::cout << jDoc.GetParseError() << "\n";

			return false;
		}
		
		rapidjson::Value& states = jDoc["states"];
		rapidjson::Value& state = jDoc["states"];

		assert(states.IsArray());

		for (rapidjson::SizeType i = 0; i < states.Size(); i++)
		{
			if (states[i]["id"].GetString() == stateID)
			{
				state = states[i];
			}
		}

		const rapidjson::Value& textures = state["textures"];
		assert(textures.IsArray());

		ParseTextures(&textures, pTextureIDs);

		const rapidjson::Value& objects = state["objects"];
		assert(objects.IsArray());

		ParseObjects(&objects, pObjects);

		for (rapidjson::SizeType i = 0; i < states.Size(); i++)
		{
			printf("states[%d] = %s\n", (int)i, states[i]["id"].GetString());
		}

		fclose(fp);

		return true;
	}

	return false;
}

void StateParser::ParseObjects(const rapidjson::Value* pStateRoot, std::vector<GameObject*>* pObjects)
{
	for (rapidjson::SizeType i = 0; i < pStateRoot->Size(); i++)
	{
		int x, y, width, height, numFrames, animSpeed, onClickCallback, onEnterCallback, onLeaveCallback;
		std::string textureID;

		x = pStateRoot[i]["x"].GetInt();
		y = pStateRoot[i]["y"].GetInt();
		width = pStateRoot[i]["width"].GetInt();
		height = pStateRoot[i]["height"].GetInt();
		numFrames = pStateRoot[i]["numFrames"].GetInt();
		animSpeed = pStateRoot[i]["animSpeed"].GetInt();

		textureID = pStateRoot[i]["textureID"].GetString();

		onClickCallback = pStateRoot[i]["onClickID"].GetInt();
		onEnterCallback = pStateRoot[i]["onEnterID"].GetInt();
		onLeaveCallback = pStateRoot[i]["onLeaveID"].GetInt();

		GameObject* pGameObject = 
			GameObjectFactory::Instance()->Create(pStateRoot[i]["type"].GetString());

		pGameObject->Load(new ObjectParams(x, y, width, height, textureID, 
			animSpeed, numFrames, onClickCallback, onEnterCallback, onLeaveCallback));

		pObjects->push_back(pGameObject);
	}
}

void StateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	for (rapidjson::SizeType i = 0; i < pStateRoot->Size(); i++)
	{
		TextureManager::Instance()->Load(pStateRoot[i]["path"].GetString(), pStateRoot[i]["id"].GetString(), Game::Instance()->GetRenderer());
	}
}
