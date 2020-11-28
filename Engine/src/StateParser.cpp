#include "StateParser.h"
#include <iostream>
#include <string>
#include "TextureManager.h"
#include "ScriptManager.h"
#include "GameObjectDictionary.h"
#include "ObjectParams.h"
#include "Game.h"
#include <spdlog/spdlog.h>

using namespace rapidjson;

bool CStateParser::ParseState(const char* stateFile, std::string stateID, std::vector<std::shared_ptr<IGameObject>>* pObjects, 
	std::vector<std::string>* pTextureIDs, std::vector<std::string>* pScriptRefs)
{
	// Read our JSON document.
	Document jDoc;

	spdlog::debug("Reading file \"{}\"...", stateFile);

	// load state file
	if (CEngineFileSystem::ReadJSON(stateFile, &jDoc))
	{
		// Create a new Value to store our state (if we find it.)
		Value iState = Value(false);
		Value& state = iState;

		assert(jDoc["states"].IsArray());

		spdlog::debug("*** State information start ***");
		spdlog::debug("Number of states: {}", jDoc["states"].Size());
		spdlog::debug("Looking for state \"{}\"", stateID);

		// Try and find our state the caller wants.
		for (SizeType i = 0; i < jDoc["states"].Size() && state.GetType() == Type::kFalseType; i++)
		{
			spdlog::debug("Found named state \"{}\"", jDoc["states"][i]["id"].GetString());

			// if it matches...
			if (jDoc["states"][i]["id"].GetString() == stateID)
			{
				// store it in.
				state = jDoc["states"][i];
				spdlog::debug("Found state in file!");
			}
		}

		// if we still have a boolean...
		if (state.IsBool())
		{
			// we didn't find it.

			spdlog::error("Could not find state \"{}\" in file \"{}\"", stateID, stateFile);

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

		const Value& scripts = state["scripts"].GetArray();
		assert(scripts.IsArray());

		// Load the scripts into the ScriptManager
		ParseScripts(&scripts, pScriptRefs);
		
		spdlog::debug("Parsed state file \"{}\"!", stateFile);
		spdlog::debug("*** State information end ***");

		return true;
	}

	// If our file stream is null - something has gone wrong.
	spdlog::error("Cannot read \"{}\"", stateFile);

	return false;
}

void CStateParser::ParseObjects(const rapidjson::Value* pStateRoot, std::vector<std::shared_ptr<IGameObject>>* pObjects)
{
	spdlog::debug("*** Parsing Objects start ***");
	// Get the object array.
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of objects defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Get the current object we are working with.
		const Value& b = t[i];

		if (!b.HasMember("type"))
		{
			spdlog::error("Missing object type attribute for {} item - ignoring...", i);
			continue;
		}

		if (b["type"].GetString() == "")
		{
			spdlog::error("Missing object type for {} item - ignoring...", i);
			continue;
		}

		if (!b.HasMember("x"))
		{
			spdlog::error("Missing X position for object of type \"{}\" - ignoring...", b["type"].GetString());
			continue;
		}

		if (!b.HasMember("y"))
		{
			spdlog::error("Missing Y position for object of type \"{}\" - ignoring...", b["type"].GetString());
			continue;
		}

		spdlog::debug("*** Object creation start ***");
		spdlog::debug("Creating object of \"{}\" at ({}, {})", b["type"].GetString(), 
			b["x"].GetFloat(), b["y"].GetFloat());

		// Attempt to create the object type.
		std::shared_ptr<IGameObject> pGameObject =
			CGameObjectDictionary::Instance()->Create(b["type"].GetString());

		CObjectParams* pOP = new CObjectParams(b["x"].GetFloat(), b["y"].GetFloat());
		// Retrieve the relevant information from the object declaration...
		// Required
		if (b.HasMember("width"))
			pOP->SetWidth(b["width"].GetInt());
		else
		{
			spdlog::warn("No width declared for object type \"{}\"", b["type"].GetString());
			pOP->SetWidth(0);
		}
		
		if (b.HasMember("height"))
			pOP->SetHeight(b["height"].GetInt());
		else
		{
			spdlog::warn("No height declared for object type \"{}\"", b["type"].GetString());
			pOP->SetHeight(0);
		}

		if (b.HasMember("textureid"))
			pOP->SetTextureID(b["textureid"].GetString());	
		else
		{
			spdlog::warn("No textureID declared for object type \"{}\"", b["type"].GetString());
			pOP->SetTextureID("");
		}

		pOP->SetName(b.HasMember("name") ? b["name"].GetString() : "");
		pOP->SetFactoryID(b["type"].GetString());

		spdlog::debug("Name: {}", pOP->GetName());
		spdlog::debug("FactoryID: {}", pOP->GetFactoryID());
		spdlog::debug("TextureID: {}", pOP->GetTextureID());
		spdlog::debug("Object Dimensions: {}x{}", pOP->GetWidth(), pOP->GetHeight());

		// Optional
		pOP->SetNumFrames(b.HasMember("numFrames") ? b["numFrames"].GetInt() : 1);
		pOP->SetAnimSpeed(b.HasMember("animSpeed") ? b["animSpeed"].GetInt() : 1);

		pOP->SetOnClick(b.HasMember("onClickId") ? b["onClickId"].GetInt() : 0);
		pOP->SetOnEnter(b.HasMember("onEnterId") ? b["onEnterId"].GetInt() : 0);
		pOP->SetOnLeave(b.HasMember("onLeaveId") ? b["onLeaveId"].GetInt() : 0);

		pOP->SetScript(b.HasMember("script") ? b["script"].GetString() : "");
		pOP->SetSoundPath(b.HasMember("soundPath") ? b["soundPath"].GetString() : "");

		spdlog::debug("Number of Frames: {}", pOP->GetNumberOfFrames());
		spdlog::debug("Animation Speed: {}", pOP->GetAnimSpeed());
		
		spdlog::debug("OnClickID: {}", pOP->GetOnClickID());
		spdlog::debug("OnEnterID: {}", pOP->GetOnEnterID());
		spdlog::debug("OnLeaveID: {}", pOP->GetOnLeaveID());

		spdlog::debug("Associated Script: {}", pOP->GetScript());
		spdlog::debug("Assocaited Sound: {}", pOP->GetSoundPath());

		// Provide the extracting info to the object.
		pGameObject->Load(pOP);

		// Add it to the m_GameObjects
		pObjects->push_back(pGameObject);

		spdlog::debug("Pushed GameObject with info!");
		spdlog::debug("*** Object creation finished ***");
	}
	spdlog::debug("*** Parsing Objects finished ***");
}

void CStateParser::ParseTextures(const rapidjson::Value* pStateRoot, std::vector<std::string>* pTextureIDs)
{
	spdlog::debug("*** Parsing Textures start ***");
	// Get the texture array. (For some reason the array is in an array?)
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of textures defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Gets the current texture we are working with.
		const Value& b = t[i];

		// Add it to a list so it can be destroyed later.
		pTextureIDs->push_back(b["id"].GetString());

		spdlog::debug("id: \"{}\"", b["id"].GetString());
		spdlog::debug("path: \"{}\"", b["path"].GetString());

		// Load our texture into memory.
		CTextureManager::Instance()->Load(CEngineFileSystem::ResolvePath(b["path"].GetString(),
																   CEngineFileSystem::EPathType::TEXTURE),
									b["id"].GetString(), CBaseGame::Instance()->GetRenderer());
	}
	spdlog::debug("*** Parsing Textures finished ***");
}

void CStateParser::ParseScripts(const rapidjson::Value* pStateRoot, std::vector<std::string>* pScriptsID)
{
	spdlog::debug("*** Parsing Scripts start ***");
	// Get the script array. (For some reason the array is in an array?)
	const Value::ConstArray& t = pStateRoot->GetArray();

	spdlog::debug("Number of scripts defined: {}", t.Size());
	for (SizeType i = 0; i < t.Size(); i++)
	{
		// Gets the current script we are working with.
		const Value& b = t[i];

		spdlog::debug("id: \"{}\"", b["id"].GetString());
		spdlog::debug("path: \"{}\"", b["path"].GetString());

		// Add it to a list so it can be destroyed later.
		pScriptsID->push_back(b["id"].GetString());

		// Load our script path and name into memory so it can be loaded later
		CScriptManager::Instance()->Load(SGameScript::file(b["id"].GetString(), 
			CEngineFileSystem::ResolvePath(b["path"].GetString(), CEngineFileSystem::EPathType::SCRIPT)));
	}
	spdlog::debug("*** Parsing Scripts finished ***");
}