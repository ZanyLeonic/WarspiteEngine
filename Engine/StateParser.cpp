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

		jDoc.ParseStream(is);

		fclose(fp);

		// Have we parsed the JSON correctly?
		if (jDoc.HasParseError())
		{
			std::cout << "An error has occurred when loading \"" << stateFile << "\"\n";
			std::cout << jDoc.GetParseError() << "\n";

			return false;
		}
		
		// Create a new Value to store our state (if we find it.)
		Value iState = Value(false);
		Value& state = iState;

		assert(jDoc["states"].IsArray());

		// Try and find our state the caller wants.
		for (SizeType i = 0; i < jDoc["states"].Size() && state.GetType() == Type::kFalseType; i++)
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

			std::cout << "Could not find state \"" << stateID << "\" in file \"" << stateFile << "\"\n";

			return false;
		}

		// Find out what textures we need for the state.
		const Value& textures = state["textures"].GetArray();
		assert(textures.IsArray());

		// Load the textures into the TextureManager
		ParseTextures(&textures, pTextureIDs);

		// Find out what objects we need.
		const Value& objects = state["objects"].GetArray();
		assert(objects.IsArray());

		// Add the Objects to the passed m_GameObjects
		ParseObjects(&objects, pObjects);

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
		// Required
		x = b["x"].GetInt();
		y = b["y"].GetInt();
		width = b["width"].GetInt();
		height = b["height"].GetInt();
		textureID = b["textureID"].GetString();

		// Optional
		numFrames = b.HasMember("numFrames") ? b["numFrames"].GetInt() : 1;
		animSpeed = b.HasMember("animSpeed") ? b["animSpeed"].GetInt() : 1;

		onClickCallback = b.HasMember("onClickID") ? b["onClickID"].GetInt() : 0;
		onEnterCallback = b.HasMember("onEnterID") ? b["onEnterID"].GetInt() : 0;
		onLeaveCallback = b.HasMember("onLeaveID") ? b["onLeaveID"].GetInt() : 0;

		// Attempt to create the object type.
		GameObject* pGameObject =
			GameObjectFactory::Instance()->Create(b["type"].GetString());

		// Provide the extracting info to the object.
		pGameObject->Load(new ObjectParams((float)x, (float)y, width, height, textureID,
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


