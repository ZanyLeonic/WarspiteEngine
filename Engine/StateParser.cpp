#include "StateParser.h"
#include <cstdio>
#include <rapidjson/filereadstream.h>

StateParser* StateParser::s_pInstance = 0;

bool StateParser::ParseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
{
	// JSON document
	rapidjson::Document jDoc;
	FILE* fp = fopen(stateFile, "r");

	// load state file
	if (fp != NULL)
	{
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		jDoc.ParseStream(is);

		if (jDoc.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << stateFile << "\"\n";
			std::cout << jDoc.GetParseError() << "\n";

			return false;
		}
		
		const rapidjson::Value& states = jDoc["states"];

		assert(states.IsArray());

		for (rapidjson::SizeType i = 0; i < states.Size(); i++)
		{
			printf("states[%d] = %s\n", (int)i, states[i]["id"].GetString());
		}

		const rapidjson::Value& textures = states["textures"];
		assert(textures.IsArray());

		ParseTextures(&textures, pTextureIDs);

		const rapidjson::Value& objects = states["objects"];
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

	}
}

void StateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	for (rapidjson::SizeType i = 0; i < pStateRoot->Size(); i++)
	{
		
	}
}
