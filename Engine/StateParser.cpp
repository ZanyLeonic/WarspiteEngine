#include "StateParser.h"
#include <cstdio>
#include <rapidjson/filereadstream.h>
#include <rapidjson/writer.h>
#include "TextureManager.h"
#include "GameObjectFactory.h"
#include "Game.h"

using namespace rapidjson;

// Debug: Serializes JSON
std::string getJSON(const Value* pStateRoot)
{
	StringBuffer sb;
	Writer<StringBuffer> writer(sb);

	pStateRoot->Accept(writer);

	return sb.GetString();;
}

bool StateParser::ParseState(const char* stateFile, std::string stateID, std::vector<GameObject*>* pObjects, std::vector<std::string>* pTextureIDs)
{
	// Read our JSON document.
	rapidjson::Document jDoc;
	FILE* fp = fopen(stateFile, "rb");

	// load state file
	if (fp != NULL)
	{
		char readBuffer[4096];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		jDoc.ParseStream<0, UTF8<>, FileReadStream>(is);

		// Have we parsed the JSON correctly?
		if (jDoc.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << stateFile << "\"\n";
			std::cout << jDoc.GetParseError() << "\n";

			return false;
		}
		
		// Create a new Value to store our state (if we find it.)
		Value& state = Value(false);

		assert(jDoc["states"].IsArray());

		// Need to store the size in a variable first due to issues
		SizeType l = jDoc["states"].GetArray().Size();

		// Try and find our state the caller wants.
		for (SizeType i = 0; i < l && state.GetType() == Type::kFalseType; i++)
		{
			// if it matches...
			if (jDoc["states"][i]["id"].GetString() == stateID)
			{
				// store it in.
				state = jDoc["states"][i];
			}
		}

		// if we still have a boolean...
		if (state.IsBool())
		{
			// we didn't find it.
			fclose(fp);

			std::cout << "Could not find state \"" << stateID << "\" in file \"" << stateFile << "\"\n";

			return false;
		}

		// Find out what textures we need for the state.
		const Value& textures = state["textures"].GetArray();

		printf("textures = %s\n", getJSON(&textures).c_str());
		assert(textures.IsArray());

		// Load the textures into the TextureManager
		ParseTextures(&textures, pTextureIDs);

		// Find out what objects we need.
		const Value& objects = state["objects"].GetArray();
		assert(objects.IsArray());

		// Add the Objects to the passed m_GameObjects
		ParseObjects(&objects, pObjects);

		// Close the file handle
		fclose(fp);

		return true;
	}

	// If our file stream is null - something has gone wrong.
	std::cout << "Cannot read \"" << stateFile << "\".\n";

	return false;
}

void StateParser::ParseObjects(const rapidjson::Value* pStateRoot, std::vector<GameObject*>* pObjects)
{
	// Get the object array.
	const Value::ConstArray& t = pStateRoot->GetArray();

	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Get the current object we are working with.
		const Value& b = t[i];
		int x, y, width, height, numFrames, animSpeed, onClickCallback, onEnterCallback, onLeaveCallback;
		std::string textureID;

		// Retrieve the relevant information from the object declaration...
		x = b["x"].GetInt();
		y = b["y"].GetInt();
		width = b["width"].GetInt();
		height = b["height"].GetInt();
		numFrames = b["numFrames"].IsInt() ? b["numFrames"].GetInt() : 1;
		animSpeed = b["animSpeed"].IsInt() ? b["animSpeed"].GetInt() : 1;

		textureID = b["textureID"].GetString();

		onClickCallback = b["onClickID"].IsInt() ? b["onClickID"].GetInt() : 0;
		onEnterCallback = b["onEnterID"].IsInt() ? b["onEnterID"].GetInt() : 0;
		onLeaveCallback = b["onLeaveID"].IsInt() ? b["onLeaveID"].GetInt() : 0;

		// Attempt to create the object type.
		GameObject* pGameObject =
			GameObjectFactory::Instance()->Create(b["type"].GetString());

		// Provide the extracting info to the object.
		pGameObject->Load(new ObjectParams(x, y, width, height, textureID,
			animSpeed, numFrames, onClickCallback, onEnterCallback, onLeaveCallback));

		// Add it to the m_GameObjects
		pObjects->push_back(pGameObject);
	}
}

void StateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	// Get the texture array. (For some reason the array is in an array?)
	const Value::ConstArray& t = pStateRoot->GetArray();

	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Gets the current texture we are working with.
		const Value& b = t[i];

		// Add it to a list so it can be destroyed later.
		pTextureIDs->push_back(b["id"].GetString());

		// Load our texture into memory.
		TextureManager::Instance()->Load(b["path"].GetString(), b["id"].GetString(), Game::Instance()->GetRenderer());
	}
}


