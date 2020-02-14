#include "StateParser.h"
#include <cstdio>
#include <rapidjson/filereadstream.h>
#include <rapidjson/writer.h>
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "Game.h"

using namespace rapidjson;

std::string getJSON(const Value* pStateRoot)
{
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	pStateRoot->Accept(writer);

	return sb.GetString();;
}

bool StateParser::ParseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
{
	// JSON document
	rapidjson::Document jDoc;
	FILE* fp = fopen(stateFile, "rb");

	// load state file
	if (fp != NULL)
	{
		char readBuffer[16384];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		jDoc.ParseStream<0, UTF8<>, FileReadStream>(is);

		if (jDoc.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << stateFile << "\"\n";
			std::cout << jDoc.GetParseError() << "\n";

			return false;
		}
		
		Value& state = jDoc["states"];

		assert(jDoc["states"].IsArray());

		SizeType l = jDoc["states"].Size();

		for (SizeType i = 0; i < l; i++)
		{
			if (jDoc["states"][i]["id"].GetString() == stateID)
			{
				state = jDoc["states"][i];
			}
		}

		const Value& textures = state["textures"];

		printf("textures = %s\n", getJSON(&textures).c_str());
		assert(textures.IsArray());

		ParseTextures(&textures, pTextureIDs);

		const Value& objects = state["test"];
		assert(objects.IsArray());

		ParseObjects(&objects, pObjects);

		//for (SizeType i = 0; i < states.Size(); i++)
		//{
		//	printf("states[%d] = %s\n", (int)i, states[i]["id"].GetString());
		//}

		fclose(fp);

		return true;
	}

	return false;
}

void StateParser::ParseObjects(const rapidjson::Value* pStateRoot, std::vector<GameObject*>* pObjects)
{
	for (SizeType i = 0; i < pStateRoot->Size(); i++)
	{
		if (pStateRoot->IsObject())
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
}

void StateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	printf("pStateRoot = %s\n", getJSON(pStateRoot).c_str());

	for (SizeType i = 0; i < pStateRoot->Size(); i++)
	{
		const Value& b = pStateRoot[i];

		printf("b[%d] = %s\n", i, getJSON(&b).c_str());

//		TextureManager::Instance()->Load(b["path"].GetString(), b["id"].GetString(), Game::Instance()->GetRenderer());
	}
}


